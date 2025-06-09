/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"
#include "Player.h"

namespace basecross {

	//====================================================================
	// class Player
	// プレイヤークラス
	//====================================================================

	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_initPos(Vec3(0.0f, 3.0f, 0.0f)),
		m_initRot(Vec3(0.0f, 0.0f, 0.0f)),
		m_initSca(Vec3(3.0f, 3.0f, 3.0f)),

		m_collideCount(m_collideCountInit)
	{
	}

	Player::Player(const shared_ptr<Stage>& StagePtr,
		const Vec3& pos, const Vec3& rot, const Vec3& sca) :
		GameObject(StagePtr),
		m_initPos(pos),
		m_initRot(rot),
		m_initSca(sca),

		m_collideCount(m_collideCountInit),
		m_moveAngle(rot.y)
	{
	}

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


	Vec3 Player::GetMoveVector() {
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
			//プレイヤーに角度を反映
			m_moveAngle = -totalAngle - XMConvertToRadians(90);

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

		//加速
		m_moveVel += GetMoveVector() * m_accel * _delta;

		//アニメーション関係
		Animate();

		//最高速度
		SpeedLimit();

		GetComponent<Transform>()->SetRotation(0, m_moveAngle, 0);
	}

	//最高速度
	void Player::SpeedLimit() {
		auto angle = Vec3(m_moveVel.x, 0, m_moveVel.z);
		if (angle.length() > 0) {
			angle.normalize();
			float limit = m_speed;
			if (m_isAir) limit *= m_airSpeedPerc;

			if (angle.x > 0) {
				if (m_moveVel.x > angle.x * limit) m_moveVel.x = angle.x * limit;
			}
			else {
				if (m_moveVel.x < angle.x * limit) m_moveVel.x = angle.x * limit;
			}
			if (angle.z > 0) {
				if (m_moveVel.z > angle.z * limit) m_moveVel.z = angle.z * limit;
			}
			else {
				if (m_moveVel.z < angle.z * limit) m_moveVel.z = angle.z * limit;
			}
		}
		//落下の終端速度
		if (m_moveVel.y < m_fallTerminal) m_moveVel.y = m_fallTerminal;
	}

	void Player::OnCreate() {
		AddTag(L"Player");

		//敵を掴む判定用オブジェクト
		m_pGrab = GetStage()->AddGameObject<PlayerGrab>(GetThis<Player>());

		//ステージマネージャ
		m_stageMgr = GetStage()->GetSharedGameObject<StageGenerator>(L"StageManager");

		m_Effect = m_stageMgr.lock()->GetEfkInterface();
		//初期位置などの設定
		auto ptr = AddComponent<Transform>();
		ptr->SetScale(m_initSca);
		ptr->SetRotation(m_initRot);
		ptr->SetPosition(m_initPos);

		//CollisionSphere衝突判定を付ける
		auto ptrColl = AddComponent<CollisionCapsule>();
		ptrColl->SetDrawActive(false);//debug
		ptrColl->SetMakedRadius(.8f);
		ptrColl->SetMakedHeight(1.25f);

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
			Vec3(.4f, .4f, .4f), //(.1f, .1f, .1f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -XM_PIDIV2, 0.0f)
		);

		ptrDraw->SetMeshResource(L"PLAYER");
		ptrDraw->SetMeshToTransformMatrix(meshMat);
		ptrDraw->SetBlendState(BlendState::AlphaBlend);
		ptrDraw->SetOwnShadowActive(true);

		RegisterAnim();

		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(ptrDraw->GetMeshResource());
		ptrShadow->SetMeshToTransformMatrix(meshMat);

		//飛び道具オブジェクト生成
		for (auto& e : m_proj) {
			e = GetStage()->AddGameObject<FireProjectile>();
		}

		//ステートマシン
		m_state = shared_ptr<PlayerStateMachine>(new PlayerStateMachine(GetThis<Player>()));

		m_HP = m_HP_max;
	}

	void Player::OnUpdate() {
		_delta = App::GetApp()->GetElapsedTime();

		//コントローラチェックして入力があればコマンド呼び出し
		//m_InputHandler.PushHandle(GetThis<Player>());

		//ステージ外に落下すると強制的に死ぬ(デバッグ？)
		if (GetComponent<Transform>()->GetPosition().y <= -20.0f) {
			m_HP = 0;
		}

		//無敵時間
		if (m_invincibleTime > 0 && m_state->GetStateType() != PlayerStateMachine::player_died) {
			//無敵時間を減算
			m_invincibleTime -= _delta;
			GetDrawPtr()->SetBlendState(BlendState::NonPremultiplied);
			//点滅する
			if (m_invincibleTime > m_invincibleTimeMax * .9f)
				GetDrawPtr()->SetDiffuse(Col4(10, 1, 1, 1));
			else
				GetDrawPtr()->SetDiffuse(Col4(1, 1, 1, 0.3f));
		}
		else {
			GetDrawPtr()->SetDiffuse(Col4(1, 1, 1, 1));
			GetDrawPtr()->SetBlendState(BlendState::AlphaBlend);
		}

		if (GetDrawPtr()->GetCurrentAnimation() == L"Died") m_invincibleTime = m_invincibleTimeMax;

		//チャージも運搬も出来ない状況
		if ((m_state->GetStateType() == PlayerStateMachine::player_stand ||
			 m_state->GetStateType() == PlayerStateMachine::player_release) == false) {
			m_isCarrying = false;
			m_pGrab.lock()->SetCollActive(false);
			m_isCharging = false;
			m_isOverCharge = false;
			m_chargePerc = 0.0f;
		}

		////演出アニメを利用しないステート
		//if (m_stateType != start && m_stateType != goalstandby && m_stateType != goal) {
		//	m_animTime = 0.0f;
		//}

		WalkSound();

		//空中判定の更新
		if (m_collideCount <= 0) {
			m_isAir = true;
		}
		else {
			m_collideCount--;
		}

		m_state->Update(_delta);

		if (m_grabTime > 0) m_grabTime -= _delta;
		if (m_grabTime < 0) m_grabTime = 0;

		SwitchAnim(GetDrawPtr()->GetCurrentAnimationTime(), m_isCharging & !m_isCarrying, L"Fire");
		SwitchAnim(GetDrawPtr()->GetCurrentAnimationTime(), m_isCarrying, L"Grab");

		GetComponent<Transform>()->SetPosition((m_moveVel * _delta) + GetComponent<Transform>()->GetPosition());

		GetDrawPtr()->UpdateAnimation(_delta);
	}

	void Player::OnUpdate2() {
		//ShowDebug();
	}

	void Player::GoalStandbyBehavior() {

		if (m_animTime == 0) {
			GetStage()->AddGameObject<GoalFade>();
			PlaySnd(L"RevCymbal", 1.0f, 0.0f);
		}
		m_animTime += _delta;

		SetAnim(L"Idle");
		Friction();

		//Goal床を参照してその手前にワープ
		if (m_animTime > 1.5f && !m_goalPosMoved) {
			m_goalPosMoved = true;
			shared_ptr<Transform> trans = m_goal->GetComponent<Transform>();

			m_moveVel = Vec3(0);
			m_goalRotate = trans->GetQuaternion();
			m_goalRotate = RotateQuat(m_goalRotate, Vec3(0, 1, 0), XM_PIDIV2);
			GetComponent<Transform>()->SetQuaternion(m_goalRotate);

			Vec3 fwd = GetComponent<Transform>()->GetForward();
			float face = atan2(fwd.z, fwd.x);
			addpos1 = Vec3((m_distToGoal * cosf(face)), 0, (m_distToGoal * sinf(face)));
			pos1 = Vec3(trans->GetPosition().x, GetComponent<Transform>()->GetPosition().y, trans->GetPosition().z);
			GetComponent<Transform>()->SetPosition(addpos1 + pos1);
		}
		if (m_goalPosMoved) {
			m_animTime = 0.0f;
			ChangeState(PlayerStateMachine::player_goal);
		}

	}

	void Player::GoalBehavior() {

		m_animTime += _delta;
		//しばらく歩いてからエレベータに入ったところで180°振り向く
		if (m_animTime > 0.0f && m_animTime <= 3.0f) {
			SetAnim(L"Walk");
			Vec3 fwd = ForwardConvert(Vec3(1, 0, 0));
			m_moveVel += fwd * m_accel * .003f;
		}
		else if (m_animTime > 3.0f && m_animTime <= 3.5f) {
			SetAnim(L"Idle");

			Easing<float> rot;
			float start = 0;
			float end = XM_PIDIV2;

			GetComponent<Transform>()->SetQuaternion(RotateQuat(m_goalRotate, Vec3(0, 1, 0), rot.EaseInOut(EasingType::Cubic, start, end, m_animTime - 3.0f, .5f)));
		}
		else if (m_animTime > 4.0f) {
			SetAnim(L"Idle");
		}
		FrictionMovie();
		SpeedLimit();

	}

	void Player::ShowDebug() {
		wstringstream wss;
		auto pos = RoundOff(GetComponent<Transform>()->GetPosition(), 3);
		auto rot = GetComponent<Transform>()->GetQuaternion();

		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wss << endl << endl << endl << "stateType : " << m_state->GetStateType() << endl;
		wss << "move : " << m_moveVel.x << " / " << m_moveVel.y << " / " << m_moveVel.z << endl;
		wss << "pos : " << pos.x << " / " << pos.y << " / " << pos.z << endl;
		wss << "rotate : " << rot.w << " / " << rot.x << " / " << rot.y << " / " << rot.z << endl;
		wss << "anim : " << GetDrawPtr()->GetCurrentAnimation() << " animtime : " << GetDrawPtr()->GetCurrentAnimationTime() << endl;
		wss << "fire : " << m_isCharging << " " << m_chargePerc << endl;
		wss << "carry : " << m_isCarrying << endl;
		wss << "ColCnt : " << m_collideCount << "Airborne: " << m_isAir << endl;
		wss << "grab : " << m_pGrab.lock()->IsHit() << " : " << m_grabTime << endl;
		wss << "fps : " << App::GetApp()->GetStepTimer().GetFramesPerSecond() << " delta : " << _delta << endl;

		if (m_goal) {
			auto gpos = m_goal->GetComponent<Transform>()->GetPosition();
			auto grot = m_goal->GetComponent<Transform>()->GetQuaternion();
			wss << "goal : " << gpos.x << " / " << gpos.y << " / " << gpos.z << endl;
			wss << "goalrot : " << grot.w << " / " << grot.x << " / " << grot.y << " / " << grot.z << endl;
		}

		auto scene = App::GetApp()->GetScene<Scene>();
		scene->SetDebugString(L"Player\n" + wss.str());
	}

	void Player::Jump() {
		if (m_isCarrying == true) return;	//敵を持った状態でもジャンプしない

		//ジャンプ時のエフェクト
		auto plPos = GetComponent<Transform>()->GetPosition();
		plPos.y -= 4;
		plPos.x += m_moveVel.x * _delta;
		plPos.z += m_moveVel.z * _delta;
		auto ShEfkInterface = m_stageMgr.lock()->GetEfkInterface();
		m_Effect->PlayEffect(m_EfkJump, L"PlayerJump", GetComponent<Transform>()->GetPosition(), 0.0f);
		m_Effect->SetScale(m_EfkJump, Vec3(.35f));

		PlaySnd(L"PlayerJump", 1, 0);

		m_moveVel.y = m_jumpHeight;
		m_collideCount = 0;
		ChangeState(PlayerStateMachine::player_air);
	}

	void Player::OnCollisionExcute(shared_ptr<GameObject>& Other) {

		//Goal床に触れたらgoalstandbyステートに移行
		if (Other->FindTag(L"Goal") && !m_isAir) {
			m_goal = Other;
			ChangeState(PlayerStateMachine::player_goalstandby);
		}

		//被弾判定
		if (Other->FindTag(L"Enemy"))
		{
			shared_ptr<Enemy> enemy = dynamic_pointer_cast<Enemy>(Other);//Enemyクラスに変換

			if (m_invincibleTime <= 0 && enemy->GetOverHeat() == false) { //オーバーヒート時は被弾しない
				GetHit(Other);
			}
		}
		//被弾判定
		if (Other->FindTag(L"EnemyBullet") && m_invincibleTime <= 0)
			GetHit(Other);

		if (Other->FindTag(L"Floor"))
		{
			m_collideCount = m_collideCountInit;
			if (m_isAir) {
				m_isAir = false;
				PlayerStateMachine::PlayerStateType type = m_state->GetStateType();

				if (type == PlayerStateMachine::player_died) {
					PlaySnd(L"PlayerLand", 1.0f, 0);

					//着地時のエフェクト
					auto plPos = GetComponent<Transform>()->GetPosition();
					plPos.y -= 4;
					plPos.x += m_moveVel.x * _delta;
					plPos.z += m_moveVel.z * _delta;
					auto ShEfkInterface = m_stageMgr.lock()->GetEfkInterface();
					m_Effect->PlayEffect(m_EfkLand, L"PlayerLand", plPos, 1);
					m_Effect->SetScale(m_EfkLand, Vec3(.25f));
					return;
				}
				else
				{
					SetAnim(L"Land");
					PlaySnd(L"PlayerLand", 1.0f, 0);
					ChangeState(PlayerStateMachine::player_stand);

					//着地時のエフェクト
					auto plPos = GetComponent<Transform>()->GetPosition();
					plPos.y -= 4;
					plPos.x += m_moveVel.x * _delta;
					plPos.z += m_moveVel.z * _delta;
					auto ShEfkInterface = m_stageMgr.lock()->GetEfkInterface();
					m_Effect->PlayEffect(m_EfkLand, L"PlayerLand", plPos, 1);
					m_Effect->SetScale(m_EfkLand, Vec3(.25f));
				}
			}
		}
	}

	void Player::GrabEnemy() {
		auto key = App::GetApp()->GetInputDevice().GetKeyState();
		auto pad = App::GetApp()->GetInputDevice().GetControlerVec();
		auto grab = m_pGrab.lock();

		//掴み判定を開始
		if (m_grabTime == 0 && (pad[0].wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER || key.m_bPressedKeyTbl['Q'] == true)) {
			m_grabTime = m_grabTimeMax;
		}

		//非ヒット時に入力されたら判定を出す
		if (!(grab->IsHit())) {
			grab->SetCollActive(m_grabTime > 0);
		}

		//当たったら消す
		if ((grab->IsHit()) && (pad[0].wReleasedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER || key.m_bUpKeyTbl['Q'] == true)) {
			grab->SetCollActive(false);
		}

		//当たったフラグを更新
		if (grab->IsHit()) {
			m_isCarrying = true;
			m_grabTime = 0.0f;
		}
		else {
			m_isCarrying = false;
		}
	}

	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (Other->FindTag(L"GoalBefore")) {
			auto group = GetStage()->GetSharedObjectGroup(L"StageDoor");
			auto& vec = group->GetGroupVector();
			for (auto& v : vec) {
				auto shObj = v.lock();

				if (shObj) {
					auto Switchs = dynamic_pointer_cast<Elevator>(shObj);
					Switchs->SetButton(true);
				}
			}
		}
	}
	void Player::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
	}

	//歩行音
	void Player::WalkSound() {
		m_walkSndTime += _delta;
		if (GetDrawPtr()->GetCurrentAnimation() == AddPrefix() + L"Run" &&
			m_walkSndTime >= .25f) {
			PlaySnd(L"PlayerWalk", .6f, 0.0f);
			m_walkSndTime = 0.0f;
		}
		else if (GetDrawPtr()->GetCurrentAnimation() == L"Walk" && m_walkSndTime >= .5f) {
			PlaySnd(L"PlayerWalk", .6f, 0.0f);
			m_walkSndTime = 0.0f;
		}
		else {
			m_walkSndTime == 0.0f;
		}
	}

	//操作中のアニメーションを切り替える(特殊なもの以外)
	void Player::Animate() {
		if ((GetDrawPtr()->GetCurrentAnimation() == L"Land" || GetDrawPtr()->GetCurrentAnimation() == L"Fire_Land") && GetDrawPtr()->GetCurrentAnimationTime() > .13f) {
			SetAnim(AddPrefix() + L"Idle");
		}
		if (!m_isAir) {
			if (abs(GetMoveVector().x) > 0)
				SetAnim(AddPrefix() + L"Run");
			else
				if (GetDrawPtr()->GetCurrentAnimation() != L"Land" && GetDrawPtr()->GetCurrentAnimation() != L"Fire_Land") {
					SetAnim(AddPrefix() + L"Idle");
				}
		}
		if (m_isAir && m_moveVel.y > 0)
			SetAnim(L"Jumping");
		if (m_isAir && (m_moveVel.y <= 0 || (GetDrawPtr()->GetCurrentAnimation() != L"Jumping" && GetDrawPtr()->GetCurrentAnimationTime() >= .13f)))
			SetAnim(L"Falling");

	}

	//重力
	void Player::Gravity() {
		if (m_doPhysicalProcess == false) return;
		m_moveVel.y += m_gravity * _delta;
		
		if (!m_isAir && m_moveVel.y < m_gravity * .1f) m_moveVel.y = m_gravity * .1f;
	}

	//摩擦(地上のみ)
	void Player::Friction() {
		if (m_doPhysicalProcess == false || m_isAir == true) return;
		//静摩擦
		if (GetMoveVector() == Vec3(0) || m_state->GetStateType() == PlayerStateMachine::player_died || m_animTime > 0.0f) {
			m_moveVel.x -= m_moveVel.x * m_friction * (1000.0f / 60.0f) * _delta;
			m_moveVel.z -= m_moveVel.z * m_friction * (1000.0f / 60.0f) * _delta;
			if (abs(m_moveVel.x) <= m_frictionThreshold) m_moveVel.x = 0;
			if (abs(m_moveVel.z) <= m_frictionThreshold) m_moveVel.z = 0;
			return;
		}
		//動摩擦
		else {
			m_moveVel.x -= m_moveVel.x * m_frictionDynamic * (1000.0f / 60.0f) * _delta;
			m_moveVel.z -= m_moveVel.z * m_frictionDynamic * (1000.0f / 60.0f) * _delta;
		}
	}

	//GetMoveVectorを使わない摩擦(演出用)
	void Player::FrictionMovie() {
		m_moveVel.x -= m_moveVel.x * m_friction * (1000.0f / 60.0f) * _delta;
		m_moveVel.z -= m_moveVel.z * m_friction * (1000.0f / 60.0f) * _delta;
		if (abs(m_moveVel.x) <= m_frictionThreshold) m_moveVel.x = 0;
		if (abs(m_moveVel.z) <= m_frictionThreshold) m_moveVel.z = 0;
	}

	//四捨五入(デバッグ用)
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
		ptrDraw->AddAnimation(L"Idle", 10, 60, true, anim_fps);
		ptrDraw->AddAnimation(L"Run", 80, 15, true, anim_fps);
		ptrDraw->AddAnimation(L"Jump_Start", 300, 5, true, anim_fps);
		ptrDraw->AddAnimation(L"Jumping", 320, 15, false, anim_fps);
		ptrDraw->AddAnimation(L"Falling", 350, 20, true, anim_fps);
		ptrDraw->AddAnimation(L"Land", 336, 4, false, anim_fps);
		ptrDraw->AddAnimation(L"Walk", 100, 29, true, anim_fps);
		ptrDraw->AddAnimation(L"Release", 242, 8, false, anim_fps);
		//敵を持つ+行動
		ptrDraw->AddAnimation(L"Grab_Idle", 375, 60, true, anim_fps);
		ptrDraw->AddAnimation(L"Grab_Run", 260, 15, true, anim_fps);
		//火炎放射+行動
		ptrDraw->AddAnimation(L"Fire_Idle", 170, 60, true, anim_fps);
		ptrDraw->AddAnimation(L"Fire_Run", 140, 19, true, 38.0f);//アニメーションを合わせるため
		//ptrDraw->AddAnimation(L"Fire_JumpStart", 0, 1, true, anim_fps);//330, 2, false, anim_fps);
		//ptrDraw->AddAnimation(L"Fire_Jumping", 0, 1, true, anim_fps);//332, 28, false, anim_fps);
		//ptrDraw->AddAnimation(L"Fire_Land", 336, 4, false, anim_fps);//360, 7, false, anim_fps);
		//やられ・死亡
		ptrDraw->AddAnimation(L"GetHit_Air", 280, 10, false, anim_fps);
		ptrDraw->AddAnimation(L"GetHit_Stand", 280, 10, false, anim_fps);
		ptrDraw->AddAnimation(L"Died", 300, 10, false, anim_fps * .6);
		ptrDraw->ChangeCurrentAnimation(L"Idle");
	}

	void Player::GetHit(shared_ptr<GameObject>& target) {

		//敵とプレイヤーの間にヒットスパーク
		auto plPos = GetComponent<Transform>()->GetPosition();
		auto shPos = target->GetComponent<Transform>()->GetPosition();
		auto ShEfkInterface = m_stageMgr.lock()->GetEfkInterface();
		m_Effect->PlayEffect(m_EfkHit, L"PlayerLand", Lerp::CalculateLerp(plPos, shPos, .0f, 1.0f, .5f, Lerp::rate::Linear), 0.0f);

		//m_EfkPlay[1] = ObjectFactory::Create<EfkPlay>(m_EfkHit, Lerp::CalculateLerp(plPos, shPos, .0f, 1.0f, .5f, Lerp::rate::Linear), 0.0f);

		ChangeState(PlayerStateMachine::player_hit);

		PlaySnd(L"PlayerDamage", 1.0f, 0);
		auto trans = GetComponent<Transform>();
		auto fwd = trans->GetForward();
		m_moveVel.x = fwd.x * 30.0f;
		m_moveVel.z = fwd.z * 30.0f;


		m_HP -= 1;
		m_invincibleTime = m_invincibleTimeMax;

	}

	//死亡
	void Player::Died() {
		SetAnim(L"Died");
		m_invincibleTime = m_invincibleTimeMax;
	}

	//飛び道具を発射
	void Player::Projectile() {
		Charging(false);

		//位置・回転周りの処理
		auto trans = GetComponent<Transform>();
		auto pos = trans->GetPosition();
		auto fwd = -1 * trans->GetForward();
		auto face = atan2f(fwd.z, fwd.x);

		Vec3 firepos = ForwardConvert(m_firePos);
		firepos = firepos * trans->GetScale();

		//エフェクトのプレイ
		auto ShEfkInterface = m_stageMgr.lock()->GetEfkInterface();
		m_Effect->PlayEffect(m_EfkMuzzle, L"Muzzle", pos + firepos, 0.0f);

		m_Effect->SetRotation(m_EfkMuzzle, Vec3(0, 1, 0), -face);
		m_Effect->SetScale(m_EfkMuzzle, Vec3(.25f));
		PlaySnd(L"PlayerProj", .6f, 0);

		//敵運搬中の処理
		if (m_isCarrying == true) {
			m_pGrab.lock()->ThrowTarget(m_chargePerc);
			ResetCharge();
			return;
		}
		//飛び道具発射
		for (auto& e : m_proj) {
			if (e->GetUsed() == true) continue;	//既に存在する弾はいじらない
			else {
				e->Invoke(pos + firepos, fwd, m_chargePerc);
				break;
			}
		}
		ResetCharge();
	}

	//火炎放射しているアニメとしていないアニメの切り替え
	void Player::SwitchAnim(const float time, const float condition, const wstring prefix) {
		const float animTime = time;
		auto draw = GetComponent<PNTBoneModelDraw>();
		if (condition) {
			vector<wstring> target = { (L"Idle"), (L"Run") };
			for (auto& anim : target) {
				if (draw->GetCurrentAnimation() == anim) {
					wstring changeanim = prefix + L"_" + anim;
					GetDrawPtr()->ChangeCurrentAnimation(changeanim, animTime);
					return;
				}
			}
		}
		if (!condition) {
			vector<wstring> target = { (prefix + L"_Idle"), (prefix + L"_Run") };
			for (auto& anim : target) {
				if (draw->GetCurrentAnimation() == anim) {
					wstring changeanim = anim.replace(0, static_cast<int>(prefix.length()) + 1, L"");
					GetDrawPtr()->ChangeCurrentAnimation(changeanim, animTime);
					return;
				}
			}
		}
	}

	const bool Player::IsCarryingEnemy() {
		if (m_pGrab.lock()) {
			return m_pGrab.lock()->IsHit();
		}
		else {
			return false;
		}
	}

	//====================================================================
	// class PlayerGrab
	// プレイヤーの掴み判定
	//====================================================================
	void PlayerGrab::OnCreate() {
		auto trans = GetComponent<Transform>();
		trans->SetScale(m_scale);
		trans->SetRotation(0.0f, 0.0f, 0.0f);

		m_collPtr = AddComponent<CollisionSphere>();
		m_collPtr->SetAfterCollision(AfterCollision::None);
		m_collPtr->SetDrawActive(false);//debug
		m_collPtr->AddExcludeCollisionTag(L"Player");
		m_collPtr->AddExcludeCollisionTag(L"Attack");
		m_collPtr->AddExcludeCollisionTag(L"EnemyFloor");

		AddTag(L"PlayerGrab");
	}

	void PlayerGrab::OnUpdate() {
		auto plPtr = m_player.lock();

		//debug
		if (false) {
			bool update = m_collPtr->GetUpdateActive();
			m_collPtr->SetDrawActive(update);
		}

		//判定を出さないときはヒット判定も消す
		if (m_isHit && GetComponent<CollisionSphere>()->GetUpdateActive() == false) {
			ClearTarget();
		}

		if (m_target) {
			m_target->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<PlayerGrab>()));
			Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);
			m_target->GetComponent<Transform>()->SetPosition(pos);
			if (!m_target->GetOverHeat()) {
				ClearTarget();
			}
		}

		if (plPtr) {
			auto trans = GetComponent<Transform>();
			auto plPos = plPtr->GetComponent<Transform>()->GetPosition();
			//差分座標の回転
			auto fwd = plPtr->GetComponent<Transform>()->GetForward();
			auto plRot = atan2f(fwd.z, fwd.x);
			Vec3 distPlus;
			distPlus.x = (cosf(plRot) * m_dist.x) - (sinf(plRot) * m_dist.z);
			distPlus.y = m_dist.y;
			distPlus.z = (cosf(plRot) * m_dist.z) + (sinf(plRot) * m_dist.x);
			trans->SetPosition(plPos + distPlus);

			//プレイヤーの向きに回転を合わせる
			Vec3 r = Vec3(0);
			r.y = -plRot;
			trans->SetRotation(r);
		}
	}

	void PlayerGrab::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Enemy") && !m_target) {
			auto enemy = dynamic_pointer_cast<Enemy>(Other);
			if (enemy->GetOverHeat() == true && !m_isHit) {
				m_target = enemy;
				m_isHit = true;
				return;
			}
		}
	}

	void PlayerGrab::ClearTarget() {
		m_isHit = false;
		m_target->GetComponent<Transform>()->ClearParent();

		//最後にターゲットを解放
		m_target = nullptr;
	}

	void PlayerGrab::ThrowTarget(float charge) {
		m_isHit = false;
		if (m_target == nullptr) {
			return;
		}
		m_target->SetThorwLenght(charge);
		m_target->SetThrowFlag(true);
		m_target->GetComponent<Transform>()->ClearParent();

		//最後にターゲットを解放
		m_target = nullptr;
	}

	//====================================================================
	// class ChargeAura
	// プレイヤーがチャージ中に足元に出る筒
	//====================================================================


	//====================================================================
	// class FireProjectile
	// プレイヤーの飛び道具
	//====================================================================

	FireProjectile::FireProjectile(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_speedAdd(24.0f),
		m_speedBase(24.0f),
		m_rangeMax(.4f),
		m_stopped(false),
		m_lifeEnded(false),
		m_speed(0),
		m_power(0)
	{
	}

	void FireProjectile::OnCreate() {

		//ステージマネージャ
		m_stageMgr = GetStage()->GetSharedGameObject<StageGenerator>(L"StageManager");

		m_Effect = m_stageMgr.lock()->GetEfkInterface();

		auto trans = GetComponent<Transform>();
		trans->SetScale(Vec3(8.0f));
		trans->SetRotation(Vec3(0));
		trans->SetPosition(Vec3(0));

		auto coll = AddComponent<TriggerSphere>();
		coll->SetDrawActive(false);//debug
		coll->SetAfterCollision(AfterCollision::None);
		coll->AddExcludeCollisionTag(L"Player");
		coll->AddExcludeCollisionTag(L"Attack");

		//描画コンポーネントの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetBlendState(BlendState::Additive);
		ptrDraw->SetDrawActive(false);//debug

		Mat4x4 meshMat;
		meshMat.affineTransformation(
			Vec3(2.0f / trans->GetScale().x, 2.0f / trans->GetScale().y, 2.0f / trans->GetScale().z), //(.1f, .1f, .1f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);
		ptrDraw->SetMeshToTransformMatrix(meshMat);

		ptrDraw->SetTextureResource(L"FIRE");

		AddTag(L"Attack");

	}

	void FireProjectile::OnUpdate() {
		//ポーズ解除後に一瞬動くのを防止するため
		if (!m_used) {
			GetComponent<TriggerSphere>()->SetUpdateActive(false);
			SetUpdateActive(false);
			return;
		}

		auto trans = GetComponent<Transform>();

		auto delta = App::GetApp()->GetElapsedTime();
		m_playTime += delta;

		if (m_EfkProj) {
			m_Effect->SetLocation(m_EfkProj, trans->GetPosition());
		}

		if (!m_stopped) {
			trans->SetPosition(trans->GetPosition() + (m_angle * m_speed * delta));
		}

		m_range -= delta;

		if (m_range <= 0 && !m_lifeEnded) {
			m_lifeEnded = true;
			m_playTime = 0;
			auto fwd = -1 * trans->GetForward();
			auto face = atan2f(fwd.z, fwd.x);

			if (m_EfkProj) {
				m_Effect->StopEffect(m_EfkProj);
			}
			m_Effect->PlayEffect(m_EfkProjEnd, L"playerprojend", GetComponent<Transform>()->GetPosition(), 0.0f);

			m_Effect->SetRotation(m_EfkProjEnd, Vec3(0, 1, 0), -face);
			m_Effect->SetScale(m_EfkProjEnd, Vec3(.8f));
		}

		if (m_lifeEnded) {
			m_Effect->SetLocation(m_EfkProjEnd, trans->GetPosition());
			if (m_range <= -.3f) {
				m_Effect->StopEffect(m_EfkProjEnd);
				m_used = false;
				SetUpdateActive(false);
			}
		}
	}

	//壁に当たったら止まる
	void FireProjectile::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FixedBox")) {
			m_stopped = true;
		}
	}

	//====================================================================
	// class SpritePlayerUI
	// プレイヤーのゲージ類
	//====================================================================

	void SpritePlayerUI::OnCreate() {
		Col4 color(1, 1, 1, 1);

		m_Vertices = {
			{Vec3(0, 0, 0.0f), color, Vec2(0, 0)},
			{Vec3(m_width, 0, 0.0f), color, Vec2(1, 0)},
			{Vec3(0, -m_height, 0.0f), color, Vec2(0, 1)},
			{Vec3(m_width, -m_height, 0.0f), color, Vec2(1, 1)},
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3,
		};
		m_DrawComp = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
		m_DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
		m_DrawComp->SetTextureResource(m_resKey);
		m_DrawComp->SetDrawActive(true);
		SetDrawLayer(m_layer);
		SetAlphaActive(true);

		GetComponent<Transform>()->SetPosition(windowWidth * -.6, windowHeight * .525, 0);

		if (m_resKey == L"PLAYERUI") {
			m_health = GetStage()->AddGameObject<SpriteHealth>(m_player.lock(), GetThis<SpritePlayerUI>());
			m_charge = GetStage()->AddGameObject<SpriteCharge>(m_player.lock(), GetThis<SpritePlayerUI>());
			m_frame = GetStage()->AddGameObject<SpritePlayerUI>(m_player.lock(), L"PLAYERUI2", 3);
		}
	}

	void SpritePlayerUI::OnUpdate() {

	}

	//====================================================================
	// class PlayerMeterBase
	// プレイヤーのゲージ類の親
	//====================================================================

	void PlayerMeterBase::Init(wstring ResKey) {
		Col4 color(1, 1, 1, 1);

		m_Vertices = {
			{Vec3(0 + m_bottomSlip, 0, 0.0f), color, Vec2(0, 0)},
			{Vec3(m_width + m_bottomSlip, 0, 0.0f), color, Vec2(1, 0)},
			{Vec3(0, -m_height, 0.0f), color, Vec2(0, 1)},
			{Vec3(m_width, -m_height, 0.0f), color, Vec2(1, 1)},
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3,
		};

		m_DrawComp = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
		m_DrawComp->SetDiffuse(color);
		m_DrawComp->SetTextureResource(ResKey);
		m_DrawComp->SetDrawActive(true);
		SetDrawLayer(2);
		SetAlphaActive(true);

		Vec3 pos = m_meter->GetComponent<Transform>()->GetPosition();
		GetComponent<Transform>()->SetPosition(pos + addPos);
	}

	//====================================================================
	// class SpriteHealth
	// プレイヤーのライフ
	//====================================================================

	void SpriteHealth::OnCreate() {
		Col4 color(1, 1, 1, 1);

		m_width = 240.0f;
		m_height = 15.0f;
		m_bottomSlip = -15.0f;

		addPos = Vec3(240.0f, -60.0f, 0.0f);

		Init(L"HEALTH");
	}

	void SpriteHealth::OnUpdate() {
		auto draw = GetComponent<PCTSpriteDraw>();

		auto player = m_player.lock();
		float perc = player->GetHPRate();
		m_Vertices[1].position.x = m_width * perc + m_bottomSlip;
		m_Vertices[3].position.x = m_width * perc;
		m_Vertices[1].textureCoordinate.x = perc;
		m_Vertices[3].textureCoordinate.x = perc;

		draw->UpdateVertices(m_Vertices);
	}
	//====================================================================
	// class SpriteCharge
	// プレイヤーの長押しゲージ
	//====================================================================

	void SpriteCharge::OnCreate() {

		m_width = 240.0f;
		m_height = 13.5f;
		m_bottomSlip = 13.5f;

		addPos = Vec3(287.0f, -82.0f, 0.0f);

		Init(L"CHARGE");
	}

	void SpriteCharge::OnUpdate() {
		auto draw = GetComponent<PCTSpriteDraw>();

		auto player = m_player.lock();
		if (player->IsCharging()) {
			draw->SetDrawActive(true);
			float perc = player->GetChargePerc();
			m_Vertices[1].position.x = (m_width * perc) + m_bottomSlip;
			m_Vertices[3].position.x = (m_width * perc);
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