/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"
#include "Collision.h"

namespace basecross {

	//====================================================================
	// class Player
	// プレイヤークラス
	//====================================================================

	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_speed(4.0f),
		m_accel(4.0f),
		m_friction(.8f),
		m_frictionThreshold(.05f),
		m_jumpHeight(2.5f),
		m_gravity(-4.0f),
		m_fallTerminal(-4.0f),

		m_firePos(Vec3(-.4f, .8f, .25f)),
		m_moveVel(Vec3(0, 0, 0)),
		m_collideCountInit(3),
		m_collideCount(m_collideCountInit),
		m_stateType(air),
		m_isCharging(false),
		m_isOverCharge(false),
		m_chargePerc(0.0f),	
		m_chargeSpeed(1.2f),
		m_chargeReduceSpeed(-.4f),

		m_HP_max(4),
		m_invincibleTimeMax(1.2f)
	{}

	Vec2 Player::GetInputState() const {
		Vec2 ret;
		//コントローラの取得
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		ret.x = 0.0f;
		ret.y = 0.0f;
		WORD wButtons = 0;
		if (cntlVec[0].bConnected) {
			ret.x = cntlVec[0].fThumbLX;
			ret.y = cntlVec[0].fThumbLY;
		}
		//キーボードの取得(キーボード優先)
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPushKeyTbl['W']) { ret.y = 1.0f; }
		if (KeyState.m_bPushKeyTbl['S']) { ret.y = -1.0f; }
		if (KeyState.m_bPushKeyTbl['A']) { ret.x = -1.0f; }
		if (KeyState.m_bPushKeyTbl['D']) { ret.x = 1.0f; }

		return ret;
	}


	Vec3 Player::GetMoveVector() const {
		Vec3 angle(0, 0, 0);
		//入力の取得
		float moveX = GetInputState().x;
		float moveZ = GetInputState().y;

		if (moveX + moveZ != 0) {
			auto ptrTransform = GetComponent<Transform>();
			auto ptrCamera = OnGetDrawCamera();

			//進行方向の向きを計算
			auto front = ptrTransform->GetPosition() - ptrCamera->GetEye();
			front.y = 0;
			front.normalize();
			//進行方向向きからの角度を算出
			float frontAngle = atan2(front.z, front.x);
			
			//コントローラの向き計算
			Vec2 moveVec(moveX, moveZ);
			float moveSize = moveVec.length();
			//コントローラの向きから角度を計算
			float cntlAngle = atan2(-moveX, moveZ);
			//トータルの角度を算出
			float totalAngle = frontAngle + cntlAngle;
			ptrTransform->SetRotation(0, -totalAngle-XMConvertToRadians(90), 0);

			//角度からベクトルを作成
			angle = Vec3(cos(totalAngle), 0, sin(totalAngle));
			//正規化する
			angle.normalize();
			//移動サイズを設定。
			angle *= moveSize;
			//Y軸は変化させない
			angle.y = 0;
		}
		return angle;
	}

	void Player::MovePlayer() {
		auto trans = GetComponent<Transform>();

		//摩擦(地上のみ)
		if (m_stateType == stand && GetMoveVector() == Vec3(0)) {
			m_moveVel.x -= m_moveVel.x * m_friction * (1000.0f / 60.0f) * _delta;
			m_moveVel.z -= m_moveVel.z * m_friction * (1000.0f / 60.0f) * _delta;
			if (abs(m_moveVel.x) <= m_frictionThreshold) m_moveVel.x = 0;
			if (abs(m_moveVel.z) <= m_frictionThreshold) m_moveVel.z = 0;
		}

		//加速
		m_moveVel += GetMoveVector() * m_accel * _delta;

		//アニメーション関係
		Animate();

		Gravity();

		//最高速度
		auto angle = Vec3(m_moveVel.x, 0, m_moveVel.z);
		if (angle.length() > 0) {
			angle.normalize();
			if (angle.x > 0) {
				if (m_moveVel.x > angle.x * m_speed) m_moveVel.x = angle.x * m_speed;
			}
			else {
				if (m_moveVel.x < angle.x * m_speed) m_moveVel.x = angle.x * m_speed;
			}
			if (angle.z > 0) {
				if (m_moveVel.z > angle.z * m_speed) m_moveVel.z = angle.z * m_speed;
			}
			else {
				if (m_moveVel.z < angle.z * m_speed) m_moveVel.z = angle.z * m_speed;
			}
		}

		//落下の終端速度
		if (m_moveVel.y < m_fallTerminal) m_moveVel.y = m_fallTerminal;

		trans->SetPosition((m_moveVel * _delta) + trans->GetPosition());
	}

	void Player::OnCreate() {

		AddTag(L"Player");

		//初期位置などの設定
		auto ptr = AddComponent<Transform>();
		ptr->SetScale(0.50f, 0.50f, 0.50f);
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(Vec3(0, 1.0f, 0));

		//CollisionSphere衝突判定を付ける
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(false);

		//各パフォーマンスを得る
		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネントの設定
		auto ptrDraw = AddComponent<PNTBoneModelDraw>();
		Mat4x4 meshMat;
		meshMat.affineTransformation(
			Vec3(.1f, .1f, .1f), //(.1f, .1f, .1f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -.5f, 0.0f)
		);

		ptrDraw->SetMeshResource(L"PLAYER");
		ptrDraw->SetMeshToTransformMatrix(meshMat);
		ptrDraw->SetOwnShadowActive(true);

		RegisterAnim();

		auto ptrShadow = AddComponent<Shadowmap>();

		m_HP = m_HP_max;

		auto ptrCamera = dynamic_pointer_cast<MainCamera>(OnGetDrawCamera());
		if (ptrCamera) {
			//MyCameraである
			//MyCameraに注目するオブジェクト（プレイヤー）の設定
			ptrCamera->SetTargetObject(GetThis<GameObject>());
			ptrCamera->SetTargetToAt(Vec3(0, 0.5f, 0));
		}

	}

	void Player::OnUpdate() {
		_delta = App::GetApp()->GetElapsedTime();


		auto key = App::GetApp()->GetInputDevice().GetKeyState();
		auto pad = App::GetApp()->GetInputDevice().GetControlerVec();

		//コントローラチェックして入力があればコマンド呼び出し
		m_InputHandler.PushHandle(GetThis<Player>());

		MovePlayer();
		MoveCamera();

		m_collideCount--;
		if (m_stateType == stand && m_collideCount <= 0) m_stateType = air;

		//Bボタンでチャージ
		Charging(pad[0].wButtons & XINPUT_GAMEPAD_B || key.m_bPushKeyTbl[VK_LCONTROL] == true);

		SwitchFireAnim(GetDrawPtr()->GetCurrentAnimationTime());

		//Bボタンで射出
		if (pad[0].wReleasedButtons & XINPUT_GAMEPAD_B || key.m_bUpKeyTbl[VK_LCONTROL] == true) {
			auto trans = GetComponent<Transform>();
			auto pos = trans->GetPosition();
			auto fwd = trans->GetForward();
			pos.y += m_firePos.y;
			pos.x += m_firePos.x * fwd.z;
			pos.z += m_firePos.z * fwd.x;
			GetStage()->AddGameObject<FireProjectile>(pos, -fwd, m_chargePerc);

			wstringstream wss;

			auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
			wss << pos.x << " : " << pos.y << " : " << pos.z << " : " << endl;
			wss << fwd.x << " : " << fwd.y << " : " << fwd.z << " : " << endl;

			auto scene = App::GetApp()->GetScene<Scene>();
			scene->SetDebugString(L"Player\n" + wss.str());

			m_chargePerc = 0.0f;
			m_isOverCharge = false;
		}

		GetDrawPtr()->UpdateAnimation(_delta);

	}

	void Player::OnUpdate2() {
		//ShowDebug();
	}

	void Player::ShowDebug() {
		wstringstream wss;
		auto pos = RoundOff(GetComponent<Transform>()->GetPosition(), 3);
		auto rot = RoundOff(GetComponent<Transform>()->GetRotation(), 3);

		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wss << "stateType : " << m_stateType << endl;
		wss << "input : " << GetMoveVector().x << " / " << GetMoveVector().y << " / " << GetMoveVector().z << endl;
		wss << "move : " << m_moveVel.x << " / " << m_moveVel.y << " / " << m_moveVel.z << endl;
		wss << "rotate : " << rot.x << " / " << rot.y << " / " << rot.z << endl;
		wss << "anim : " << GetDrawPtr()->GetCurrentAnimation() << " animtime : " << GetDrawPtr()->GetCurrentAnimationTime() << endl;
		wss << "fire : " << m_isCharging << " " << m_chargePerc << endl;
		wss << "hp : " << m_HP << " / " << m_HP_max << endl;
		wss << "fps : " << App::GetApp()->GetStepTimer().GetFramesPerSecond() << " delta : " << _delta << endl;

		auto scene = App::GetApp()->GetScene<Scene>();
		scene->SetDebugString(L"Player\n" + wss.str());
	}

	void Player::OnPushA() {
		if (m_stateType != stand) return;	//立ち状態以外ではジャンプしない
		m_moveVel.y = m_jumpHeight;
		m_stateType = air;
	}

	void Player::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		m_collideCount = m_collideCountInit;
		if (m_stateType == air && Other->FindTag(L"FixedBox")) {
			SetAnim(AddFire() + L"Land");
			m_stateType = stand;
		}
		//メモ　地形オブジェクトのタグをWallとFloorに分けて接地判定を実装したい
	}

	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if ((Other->FindTag(L"GimmickButton")))
		{

			auto group = GetStage()->GetSharedObjectGroup(L"Switch");
			auto& vec = group->GetGroupVector();
			for (auto& v : vec) {
				auto shObj = v.lock();
				if (Other == shObj) {
					auto Switchs = dynamic_pointer_cast<GimmickButton>(shObj);
					Switchs->SetButton(true);
				}
			}
		}

	}
	void Player::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
		if ((Other->FindTag(L"GimmickButton")))
		{
			auto group = GetStage()->GetSharedObjectGroup(L"Switch");
			auto& vec = group->GetGroupVector();
			for (auto& v : vec) {
				auto shObj = v.lock();
				if (shObj) {
					auto Switchs = dynamic_pointer_cast<GimmickButton>(shObj);
					Switchs->SetButton(false);
				}
			}
		}
	}

	void Player::MoveCamera()
	{
		//auto ptrCamera = dynamic_pointer_cast<Camera>(OnGetDrawCamera());
		//auto pos = GetComponent<Transform>()->GetPosition();
		//Vec3 Camera = ptrCamera->GetEye();
		//float differenceX = pos.x - Camera.x;
		//if (differenceX >= 0.5f)
		//{
		//	ptrCamera->SetEye(Camera.x + (differenceX - 0.5f), -0.3f, Camera.z);
		//	ptrCamera->SetAt(pos.x - differenceX, -0.3f, pos.z);
		//}
		//else if (differenceX <= -0.5f)
		//{
		//	ptrCamera->SetEye(Camera.x + (differenceX + 0.5f), -0.3f, Camera.z);
		//	ptrCamera->SetAt(pos.x - differenceX, -0.3f, pos.z);
		//}
	}

	void Player::Animate() {
		if ((GetDrawPtr()->GetCurrentAnimation() == L"Land" || GetDrawPtr()->GetCurrentAnimation() == L"Fire_Land") && GetDrawPtr()->GetCurrentAnimationTime() > .23f) {
			SetAnim(AddFire() + L"Idle");
		}
		if (m_stateType == stand) {
			if (abs(GetMoveVector().x) > 0)
				SetAnim(AddFire() + L"Run");
			else
				if (GetDrawPtr()->GetCurrentAnimation() != L"Land" && GetDrawPtr()->GetCurrentAnimation() != L"Fire_Land") {
					SetAnim(AddFire() + L"Idle");
				}
		}
		if (m_stateType == air)
			SetAnim(AddFire() + L"Jumping");
	}

	void Player::Gravity() {
		m_moveVel.y += m_gravity * _delta;
		if (m_stateType == stand && m_moveVel.y < m_gravity * .1f) m_moveVel.y = m_gravity * .1f;
	}

	Vec3 Player::RoundOff(Vec3 number, int point) {
		Vec3 r = number *= pow(10, point);
		r.x = round(r.x);
		r.y = round(r.y);
		r.z = round(r.z);
		r /= pow(10, point);
		return r;
	}

	//アニメーション登録・初期化
	void Player::RegisterAnim() {
		auto ptrDraw = GetDrawPtr();
		auto anim_fps = 30.0f;

		//移動関連
		ptrDraw->AddAnimation(L"Idle", 30, 60, true, anim_fps);
		ptrDraw->AddAnimation(L"Run", 100, 12, true, anim_fps);
		ptrDraw->AddAnimation(L"Jump_Start", 240, 2, false, anim_fps);
		ptrDraw->AddAnimation(L"Jumping", 242, 28, false, anim_fps);
		ptrDraw->AddAnimation(L"Land", 270, 7, false, anim_fps);
		ptrDraw->AddAnimation(L"PushObject", 280, 19, false, anim_fps);
		//攻撃
		ptrDraw->AddAnimation(L"Attack1", 120, 9, false, anim_fps);
		ptrDraw->AddAnimation(L"Attack2", 132, 8, false, anim_fps);
		ptrDraw->AddAnimation(L"Attack3", 145, 25, false, anim_fps);
		ptrDraw->AddAnimation(L"Attack4", 175, 12, false, anim_fps);
		ptrDraw->AddAnimation(L"Attack5", 188, 40, false, anim_fps);
		//火炎放射+行動
		ptrDraw->AddAnimation(L"Fire_Idle", 367, 33, true, 16.5f);//通常立ちモーションと合わせるため
		ptrDraw->AddAnimation(L"Fire_Run", 310, 12, true, anim_fps);
		ptrDraw->AddAnimation(L"Fire_RunBack", 410, 14, true, anim_fps);
		ptrDraw->AddAnimation(L"Fire_JumpStart", 330, 2, false, anim_fps);
		ptrDraw->AddAnimation(L"Fire_Jumping", 332, 28, false, anim_fps);
		ptrDraw->AddAnimation(L"Fire_Land", 360, 7, false, anim_fps);
		//やられ・死亡
		ptrDraw->AddAnimation(L"GetHit_Air", 510, 24, false, anim_fps);
		ptrDraw->AddAnimation(L"GetHit_Stand", 550, 22, false, anim_fps);
		ptrDraw->AddAnimation(L"Died", 580, 45, false, anim_fps);
		ptrDraw->ChangeCurrentAnimation(L"Idle");
	}


	//火炎放射しているアニメとしていないアニメの切り替え
	void Player::SwitchFireAnim(const float time) {
		const float animTime = time;
		auto draw = GetComponent<PNTBoneModelDraw>();
		if (m_isCharging) {
			vector<wstring> target = { (L"Idle"), (L"Run"), (L"Jump_Start"), (L"Jumping"), (L"Land") };
			for (auto& anim : target) {
				if (draw->GetCurrentAnimation() == anim) {
					wstring changeanim = L"Fire_" + anim;
					GetDrawPtr()->ChangeCurrentAnimation(changeanim, animTime);
					return;
				}
			}
		}
		if (!m_isCharging) {
			vector<wstring> target = { (L"Fire_Idle"), (L"Fire_Run"), (L"Fire_RunBack"), (L"Fire_Jump_Start"), (L"Fire_Jumping"), (L"Fire_Land") };
			for (auto& anim : target) {
				if (draw->GetCurrentAnimation() == anim) {
					wstring changeanim = anim.replace(0, 5, L"");
					GetDrawPtr()->ChangeCurrentAnimation(changeanim, animTime);
					return;
				}
			}
		}
	}

	//====================================================================
	// class PlayerStateCtrl
	// プレイヤーの移動操作中ステート
	//====================================================================

	//shared_ptr<PlayerStateCtrl> PlayerStateCtrl::Instance() {
	//	static shared_ptr<PlayerStateCtrl> instance = new PlayerStateCtrl;
	//	return instance;
	//}

	//====================================================================
	// class AttackCollision
	// プレイヤーの攻撃判定
	//====================================================================

	AttackCollision::AttackCollision(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<GameObject>& player) :
		GameObject(StagePtr),
		m_player(player),
		m_distFromPlayer(Vec3(.1f, 0, 0)),
		m_isMoveHit(false)
	{}

	void AttackCollision::OnCreate() {
		auto trans = GetComponent<Transform>();
		trans->SetScale(0.10f, 0.10f, 0.10f);	//直径25センチの球体
		trans->SetRotation(0.0f, 0.0f, 0.0f);
		trans->SetPosition(Vec3(0, 0, 0));

		auto coll = AddComponent<TriggerSphere>();
		coll->SetDrawActive(false);//debug

	}


	void AttackCollision::OnUpdate() {
		FollowPlayer();

	}

	void AttackCollision::FollowPlayer() {
		auto trans = GetComponent<Transform>();
		//weakからsharedに格上げ
		auto ptrPlayer = m_player.lock();

		if (ptrPlayer != nullptr) {
			auto matPlayer = ptrPlayer->GetComponent<Transform>()->GetWorldMatrix();
			matPlayer.scaleIdentity();
			Mat4x4 matrix;
			matrix.affineTransformation(
				Vec3(1),
				Vec3(0),
				Vec3(0),
				m_distFromPlayer);
			matrix *= matPlayer;

			auto posPlayer = matrix.transInMatrix();
			trans->SetPosition(posPlayer);
			trans->SetQuaternion(matrix.quatInMatrix());
		}
	}

	void AttackCollision::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		//
		if (!m_isMoveHit && Other->FindTag(L"Enemy")) {
			//MessageBox(0, 0, 0, MB_ICONINFORMATION);

			//攻撃
			m_isMoveHit = true;
		}
	}

	void AttackCollision::OnCollisionExit(shared_ptr<GameObject>& Other) {
		//
		if (m_isMoveHit && Other->FindTag(L"Enemy")) {
			//攻撃
		}
	}

	//====================================================================
	// class FireProjectile
	// プレイヤーの飛び道具
	//====================================================================

	FireProjectile::FireProjectile(const shared_ptr<Stage>& StagePtr,
		const Vec3 dist, const Vec3 angle, const float power) :
		GameObject(StagePtr),
		m_dist(dist),
		m_angle(angle),
		m_power(power),
		m_speed(3.0f),
		m_rangeMax(.8f)
	{}

	void FireProjectile::OnCreate() {

		auto trans = GetComponent<Transform>();
		trans->SetScale(0.20f, 0.20f, 0.20f);
		trans->SetRotation(0.0f, 0.0f, 0.0f);
		trans->SetPosition(m_dist);

		auto coll = AddComponent<TriggerSphere>();
		coll->SetDrawActive(false);//debug
		coll->SetAfterCollision(AfterCollision::None);

		//描画コンポーネントの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetBlendState(BlendState::AlphaBlend);

		ptrDraw->SetTextureResource(L"FIRE");

		AddTag(L"Attack");

		m_range = m_rangeMax;
	}

	void FireProjectile::OnUpdate() {
		auto delta = App::GetApp()->GetElapsedTime();

		auto trans = GetComponent<Transform>();

		trans->SetPosition(trans->GetPosition() + (m_angle * m_speed * m_power * delta));

		m_range -= delta;
		GetComponent<PNTStaticDraw>()->SetDiffuse(Col4(1, 1, 1, m_range * 2 / m_rangeMax));

		if (m_range <= 0) {
			GetStage()->RemoveGameObject<FireProjectile>(GetThis<FireProjectile>());
		}
	}

	//====================================================================
	// class SpriteCharge
	// プレイヤーの長押しゲージ
	//====================================================================

	void SpriteCharge::OnCreate() {
		Col4 color(1, 1, 1, 1);

		m_Vertices = {
			{Vec3(0, 0, 0.0f), color, Vec2(0, 0)},
			{Vec3(m_width, 0, 0.0f), color, Vec2(1, 0)},
			{Vec3(0, m_height, 0.0f), color, Vec2(0, 1)},
			{Vec3(m_width, m_height, 0.0f), color, Vec2(1, 1)},
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3,
		};
		m_DrawComp = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
		m_DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
		m_DrawComp->SetTextureResource(L"CHARGE");
		m_DrawComp->SetDrawActive(false);
		SetDrawLayer(1);
		SetAlphaActive(true);

		GetComponent<Transform>()->SetPosition(windowWidth * -.45, windowHeight * .45, 0);
	}

	void SpriteCharge::OnUpdate() {
		auto draw = GetComponent<PCTSpriteDraw>();

		auto player = m_player.lock();
		if (player->IsCharging()) {
			draw->SetDrawActive(true);
			float perc = player->GetChargePerc();
			m_Vertices[1].position.x = m_width * perc;
			m_Vertices[3].position.x = m_width * perc;
			m_Vertices[1].textureCoordinate.x = perc;
			m_Vertices[3].textureCoordinate.x = perc;

			draw->UpdateVertices(m_Vertices);
		}
		else {
			draw->SetDrawActive(false);
		}
	}
}
//end basecross

//長押し始めるとゲージ出現、チャージ開始
//長押し終えるとゲージ非表示、レーザー射出　チャージの結果を参照して射程決定