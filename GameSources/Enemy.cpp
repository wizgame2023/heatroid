/*!
@file Enemy.cpp
@brief 敵など実体
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;  //敵
	//--------------------------------------------------------------------------------------

	Enemy::Enemy(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotatoin,
		const Vec3& scale,
		const State& state,
		const State& overHeatState,
		const shared_ptr<Player>& player
	) :
		GameObject(stage),
		m_pos(position),
		m_rot(rotatoin),
		m_scal(scale),
		m_stateType(state),
		m_overHeatState(overHeatState),
		m_player(player),
		m_heat(0),
		m_maxHeat(100),
		m_speed(5.0f),
		m_maxSpeed(5.0f),
		m_upSpeed(3.0f),
		m_upHeight(10.0f),
		m_jumpPower(5.0f),
		m_jumpTime(1.0f),
		m_dicUp(0),
		m_dropTime(4.0f),
		m_maxDropTime(m_dropTime),
		m_hitDropTime(2.0f),
		m_maxHitDropTime(m_hitDropTime),
		m_bulletTime(0.0f),
		m_trackingRange(30.0f),
		m_plungeColFlag(0),
		m_plungeTime(3.0f),
		m_firstDirec(Vec3(0.0f)),
		m_gravity(-9.8f),
		m_grav(Vec3(0.0f, m_gravity, 0.0f)),
		m_gravVel(Vec3(0.0f)),
		m_moveRot(Vec3(0.0f)),
		m_bulletFlag(false),
		m_jumpFlag(false),
		m_flyFlag(false),
		m_floorFlag(false),
		m_hitDropFlag(false),
		m_plungeFlag(false)
	{}

	void Enemy::OnCreate() {
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scal);
		m_beforState = m_stateType;
		m_beforePos = m_pos;

		auto player = m_player.lock();
		if (!player) return;
		m_playerScale = m_player.lock()->GetScale();
		//描画
		//auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		//ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//ptrDraw->SetTextureResource(L"White");
		//ptrDraw->SetDiffuse(Col4(0.0f,0.0f,1.0f,1.0f));
		
		auto ptrDraw = AddComponent<PNTBoneModelDraw>();
		Mat4x4 meshMat;
		meshMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		ptrDraw->SetMeshResource(L"ENEMYARUKU");
		ptrDraw->SetMeshToTransformMatrix(meshMat);
		ptrDraw->SetOwnShadowActive(true);

		ptrDraw->AddAnimation(L"walk", 10, 30, true, 30);
		ptrDraw->AddAnimation(L"attack", 50, 40, false, 30);
		ptrDraw->AddAnimation(L"wait", 90, 25, false, 30);
		ptrDraw->ChangeCurrentAnimation(L"walk");

		//衝突判定
		m_collision = AddComponent<CollisionObb>();
		m_collision->SetFixed(false);
		m_collision->SetDrawActive(true);
		//影
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		AddTag(L"Enemy");
	}

	void Enemy::OnUpdate() {
		auto& keyState = App::GetApp()->GetInputDevice().GetKeyState();
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto stage = GetStage();
		m_pos = m_trans->GetPosition();
		//プレイヤー情報を取得
		auto player = m_player.lock();
		if (!player) return;
		m_playerTrans = player->GetComponent<Transform>();
		m_playerPos = m_playerTrans.lock()->GetPosition();

		/*------------------テスト用-------------------------*/
		//キー操作で移動
		//if (keyState.m_bLastKeyTbl[VK_RIGHT]) {
		//	m_pos.x += m_dic * m_speed * elapsed;
		//}
		//if (keyState.m_bLastKeyTbl[VK_LEFT]) {
		//	m_pos.x += -m_dic * m_speed * elapsed;
		//}
		//スペースでジャンプ（テスト用）
		//if (keyState.m_bPressedKeyTbl[VK_DOWN]) {
		//	//EnemyJump();
		//	m_stateType = plunge;
		//}
		if (keyState.m_bPushKeyTbl[VK_DOWN]) {
			m_stateType = plunge;

		}
		if (keyState.m_bPressedKeyTbl[VK_UP]) {
			HipDropJump();
			//stage->AddGameObject<EnemyBullet>(GetThis<Enemy>());
		}
		//if (keyState.m_bPushKeyTbl['W']) {
		//	//m_stateType = stay;
		//}
		/*------------------テスト用-------------------------*/


		FindFixed();


		//行動パターン
		switch (m_stateType)
		{
		//移動なし
		case stay:
			EnemyAnime(L"wait");
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			break;
		//追従の左右移動
		case rightMove:
			EnemyAnime(L"walk");
			m_speed = m_maxSpeed;
			PlayerDic();
			EnemyAngle();
			if (m_direc.length() <= m_trackingRange) {
				Bullet();
			}
			break;
		//上下に移動
		case upMove:
			GravZero();
			Grav();
			if (m_pos.y >= m_upHeight) {
				m_dicUp = -1;
			}
			else if (m_pos.y <= m_beforePos.y) {
				m_dicUp = 1;
			}
			m_pos.y += m_dicUp * m_upSpeed * elapsed;
			break;
		//追従浮遊
		case flyMove:
			OneJump(0.1f);
			PlayerDic();
			EnemyAngle();
			Bullet();
			break;
		//浮遊
		case fly:
			OneJump(0.1f);
			EnemyAngle();
			break;
		//プレイヤーに触れた位置で固定
		case fixedStay:
			GravZero();
			Grav();
			m_pos = m_deathPos;
			m_collision->SetFixed(true);
			AddTag(L"FixedBox");
			break;
			//プレイヤーと逆方向に移動
		case runaway:

			PlayerDic();
			break;
			//ヒップドロップ
		case hitDrop:
			m_dropTime -= elapsed;
			if (m_dropTime <= 0.0f) {
				HipDropJump();
				m_dropTime = m_maxDropTime;
			}
			HitDrop();
			break;
			//突っ込み
		case plunge:
			EnemyAnime(L"attack");
			//PlayerDic();
			if (m_floorFlag) {
				if (!m_plungeFlag) {
					m_firstDirec = m_playerPos - GetChangePos();
					m_plungeFlag = true;
				}
				Vec3 dic = Vec3(m_firstDirec.x, 0.0f, m_firstDirec.z);
				m_pos += dic * m_speed * 0.2f * elapsed;

			}
			break;
		default:
			break;
		}
		//重力の処理
		if (!m_floorFlag) {
			Grav();
		}
		if (m_floorFlag) {
			GravZero();
			Grav();
		}
		m_trans->SetPosition(m_pos);


		auto draw = GetComponent<PNTBoneModelDraw>();
		draw->UpdateAnimation(elapsed);
		//Debug();
	}

	//ジャンプ
	void Enemy::EnemyJump() {
		m_floorFlag = false;
		AddGrav(Vec3(0.0f, m_jumpPower*3, 0.0f));
		SetGrav(Vec3(0.0f, -0.98f, 0.0f));
		Grav();
	}
	void Enemy::HipDropJump() {
		m_hitDropFlag = true;
		GravZero();
		EnemyJump();
	}
	//削除
	void Enemy::ThisDestroy() {
		GetStage()->RemoveGameObject<Enemy>(GetThis<Enemy>());
	}
	//ダメージを受ける
	void Enemy::ReceiveDamage(float damage) {
		m_heat -= damage;
	}
	//プレイヤーの向いている方向に進む
	void Enemy::PlayerDic() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto player = m_player.lock();
		if (player) {
			auto playerTrans = player->GetComponent<Transform>();
			auto playerPos = playerTrans->GetPosition();
			m_direc = playerPos - GetChangePos();
			Vec3 dic = Vec3(m_direc.x, 0.0f, m_direc.z);
			auto direc = dic.normalize();
			if (m_direc.length() <= m_trackingRange && m_stateType != plunge) {
				m_pos += m_speed * direc * elapsed;

			}
		}
	}
	//プレイヤーの方向を向かせる
	void Enemy::EnemyAngle()
	{
		auto front = GetDirec();
		auto elapsed = App::GetApp()->GetElapsedTime();
		front.y = 0;
		front.normalize();
		float frontAngle = atan2(front.z, front.x);
		float rad = XMConvertToRadians(90.0f);
		m_trans->SetRotation(Vec3(0.0f, -frontAngle - rad, 0.0f));
	}
	//オーバーヒート
	void Enemy::OverHeat() {
		float elapsed = App::GetApp()->GetElapsedTime();
		if (m_heat >= m_maxHeat) {
			m_stateType = m_overHeatState;
		}
		if (m_heat > 0.0f) {
			m_heat -= elapsed * 10;
		}
		else if (GetOverHeat()&&m_heat <= 0.0f) {
			m_heat = 0.0f;
			if (m_stateType != m_beforState) {
				m_stateType = m_beforState;
			}
		}
	}

	//ジャンプ
	void Enemy::OneJump(float jumpHight) {
		if (!m_jumpFlag) {
			EnemyJump();
			m_jumpFlag = true;
		}
		if (m_pos.y <= jumpHight) {
			m_jumpFlag = false;
		}

	}
	//ヒップドロップ
	void Enemy::HitDrop() {
		auto fixed = m_fixedBox.lock();
		auto elapsed = App::GetApp()->GetElapsedTime();
		//ヒップドロップ中
		if (m_hitDropFlag) {
			if (fixed) {
				Vec3 fixPos = m_fixedBox.lock()->GetComponent<Transform>()->GetPosition();
				Vec3 fixScal = m_fixedBox.lock()->GetComponent<Transform>()->GetScale();
				//床に衝突した時
				m_hitDropTime -= elapsed;
				if (m_floorFlag) {
					m_hitDropTime = m_maxHitDropTime;
					m_hitDropFlag = false;
					AddGrav(Vec3(0.0f, -m_jumpPower, 0.0f));
				}
				if (m_hitDropTime <= 0) {
					if (m_pos.y - m_scal.y / 2 + fixPos.y >= fixPos.y + fixScal.y / 2 + 0.01f) {
						m_pos.y += -m_speed * 3 * elapsed;
					}
				}
			}
		}

	}
	//床に触れたら取得
	void Enemy::FindFixed() {
		auto fixed = m_fixedBox.lock();
		if (fixed) {
			Vec3 fixPos = fixed->GetComponent<Transform>()->GetPosition();
			Vec3 fixScal = fixed->GetComponent<Transform>()->GetScale();
			//if (m_pos.y - m_scal.y / 2 + fixPos.y <= fixPos.y + fixScal.y / 2 + 0.001f) {
			//	m_floorFlag = true;
			//}
			Vec3 worldPos = GetChangePos();
			if (worldPos.y - m_scal.y / 2.0f <= fixPos.y + fixScal.y / 2.0f+0.01f) {
				m_floorFlag = true;
				//m_pos.y = fixPos.y - fixScal.y / 2 - m_scal.y / 2;
			}
			//if (m_trans->GetPosition().y >= fixScal.y / 2.0f) {
			//	m_floorFlag = true;
			//}
			if (m_pos.x >= fixScal.x / 2 ||
				m_pos.x <= -fixScal.x / 2) {
				m_floorFlag = false;
			}
			if (m_pos.z >=fixScal.z / 2 ||
				m_pos.z <=-fixScal.z / 2) {
				m_floorFlag = false;
			}

			//床から落ちないようにする
			//if (m_pos.x > fixPos.x + fixScal.x / 2 - m_scal.x / 2 - 0.2f) {
			//	m_speed = 0.0f;
			//	if (m_pos.x > m_playerPos.x - fixPos.x) {
			//		m_speed = 0.3f;
			//	}
			//}
			//else if (m_pos.x < fixPos.x - fixScal.x / 2 + m_scal.x / 2 + 0.2f) {
			//	m_speed = 0;
			//	if (m_pos.x < m_playerPos.x - fixPos.x) {
			//		m_speed = 0.3f;
			//	}
			//}
			//else {
			//	m_speed = 0.3f;
			//}
		}
		else {
			m_floorFlag = false;
		}

	}
	//弾の発射
	void Enemy::Bullet() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto stage = GetStage();
		auto fixed = m_fixedBox.lock();
		m_bulletTime += elapsed;
		if (!m_bulletFlag) {
			if (fixed) {
				auto fixedPos = fixed->GetComponent<Transform>()->GetPosition();
				stage->AddGameObject<EnemyBullet>(GetThis<Enemy>(), fixedPos);
			}
			else {
				stage->AddGameObject<EnemyBullet>(GetThis<Enemy>());
			}
			m_bulletFlag = true;
			m_bulletTime = 0.0f;
		}
		if (m_bulletTime >= 2.0f) {
			m_bulletFlag = false;
		}

	}

	//衝突判定
	void Enemy::OnCollisionEnter(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			m_deathPos = m_pos;

			ReceiveDamage(50.0f);
		}
		if (other->FindTag(L"Wall")) {
			auto breakWall = dynamic_pointer_cast<BreakWall>(other);
		}
		if (other->FindTag(L"FixedBox")) {
			m_fixedBox = dynamic_pointer_cast<TilingFixedBox>(other);
			m_trans->SetParent(m_fixedBox.lock());
			m_floorPos = m_pos;
		}
		if ((other->FindTag(L"GimmickButton")))
		{
			auto group = GetStage()->GetSharedObjectGroup(L"Switch");
			auto& vec = group->GetGroupVector();
			for (auto& v : vec) {
				auto shObj = v.lock();
				if (other == shObj) {
					auto Switchs = dynamic_pointer_cast<GimmickButton>(shObj);
					Switchs->SetButton(true);
				}
			}
		}
		if (other->FindTag(L"Attack")) {
			m_deathPos = m_pos;
			m_heat = m_maxHeat;
		}
	}
	void Enemy::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
		if ((Other->FindTag(L"GimmickButton")))
		{
			auto group = GetStage()->GetSharedObjectGroup(L"Switch");
			auto& vec = group->GetGroupVector();
			for (auto& v : vec) {
				auto shObj = v.lock();
				if (Other == shObj) {
					auto Switchs = dynamic_pointer_cast<GimmickButton>(shObj);
					Switchs->SetButton(false);
				}
			}
		}
	}

	void Enemy::OnCollisionExcute(shared_ptr<GameObject>& Other)
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

	void Enemy::EnemyAnime(wstring anime) {
		auto draw = GetComponent<PNTBoneModelDraw>()->GetCurrentAnimation();
		if ( draw!= anime) {
			GetComponent<PNTBoneModelDraw>()->ChangeCurrentAnimation(anime);
		}

	}
	//デバック
	void Enemy::Debug() {
		//デバック用
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss(L"");
		wss << L"damage : "
			<< m_heat
			<< L"\nfps : "
			<< fps
			<< L"\nstate : "
			<< m_stateType
			<< L"\npos : "
			<< L"x "
			<< m_pos.x
			<< L"\n y "
			<<m_pos.y
			<<L"\n z "
			<<m_pos.z
			<< L"\nfloor : "
			<< m_floorFlag
			<< L"\nhipDrop : "
			<< m_hitDropFlag
			<< L"\ntest : "
			<< m_test
			<<L"\n changePos : "
			<<GetChangePos().y
			<<L"\ngrav : "
			<<m_grav.y
			<<L"\nOverHeat : "
			<< GetOverHeat()
			<<L"\n"
			<< m_plungeTime
			<< endl;
		scene->SetDebugString(wss.str());

	}
	//重力
	void Enemy::Grav() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		m_gravVel += m_grav * elapsed;
		m_pos += m_gravVel * elapsed;
		if (m_gravVel.y < m_grav.y) {
			m_gravVel.y = m_grav.y;
		}
		m_trans->SetPosition(m_pos);
	}
	void Enemy::GravZero() {
		m_grav = Vec3(0.0f);
	}
	void Enemy::GravVelZero() {
		m_gravVel = Vec3(0.0f);
	}
	void Enemy::SetGrav(Vec3 grav) {
		m_grav = grav;
	}
	void Enemy::AddGrav(Vec3 grav) {
		m_gravVel = grav;
	}
	void Enemy::SetGravVel(Vec3 grav) {
		auto elapsed = App::GetApp()->GetElapsedTime();
		m_gravVel = grav;
	}

	//ゲッターセッター
	Vec3 Enemy::GetDirec() {
		return m_direc;
	}
	Vec3 Enemy::GetPos() {
		return m_pos;
	}
	void Enemy::SetEnemyFlayFlag(bool flag) {
		m_flyFlag = flag;
	}
	void Enemy::SetSpeed(float speed) {
		m_speed = speed;
	}
	void Enemy::SetState(State state) {
		m_stateType = state;
	}
	void Enemy::SetUpMove(float speed, float height) {
		m_upSpeed = speed;
		m_upHeight = height;
	}
	void Enemy::SetFlyPower(float power) {
		m_jumpPower = power;
	}
	bool Enemy::GetFloorFlag() {
		return m_floorFlag;
	}
	float Enemy::GetHeatRatio() {
		float ratio = m_heat / m_maxHeat;
		if (ratio <= 0.0f) {
			return 0.0f;
		}
		else {
			return ratio;
		}
	}
	Vec3 Enemy::GetChangePos() {
		auto fixed = m_fixedBox.lock();
		if (fixed) {
			//Vec3 pos = m_pos + fixed->GetComponent<Transform>()->GetPosition();
			Vec3 pos = m_trans->GetWorldPosition();
			return pos;
		}
		else {
			return m_pos;
		}
	}
	bool Enemy::GetOverHeat() {
		if (m_stateType == m_overHeatState) {
			return true;
		}
		else {
			return false;
		}
	}

	//--------------------------------------------------------------------------------------
	//	class EnemyBullet : public GameObject;  
	//--------------------------------------------------------------------------------------
	EnemyBullet::EnemyBullet(const shared_ptr<Stage>& stage,
		const shared_ptr<Enemy>& enemy,
		const Vec3& fixedPos
	) :
		GameObject(stage),
		m_enemy(enemy),
		m_fixedPos(fixedPos),
		m_pos(Vec3(0, 0, 0)),
		m_rot(Vec3(0, 0, 0)),
		m_scal(Vec3(0.5f, 0.5f, 0.5f)),
		m_speed(20.0f),
		m_Range(1.0f),
		m_floorCheck(false)
	{}
	void EnemyBullet::OnCreate() {
		m_trans = GetComponent<Transform>();
		m_enemyPos = m_enemy->GetChangePos();
		m_trans->SetPosition(m_enemyPos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scal);
		auto direc = m_enemy->GetDirec();
		m_direc = direc.normalize();
		m_beforFlag = m_enemy->GetFloorFlag();
		m_enemyPos = m_enemy->GetChangePos();

		//描画
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetTextureResource(L"White");
		ptrDraw->SetDiffuse(Col4(0.0f, 0.0f, 1.0f, 1.0f));

		//衝突判定
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetFixed(false); //空間に固定するか
		ptrColl->SetDrawActive(false);
		//影
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

	}
	void EnemyBullet::OnUpdate() {
		auto elapsed = App::GetApp()->GetElapsedTime();

		m_trans = GetComponent<Transform>();
		m_pos = m_trans->GetPosition();
		m_pos.y = m_enemyPos.y;
		m_pos += m_direc * m_speed * elapsed;
		m_trans->SetPosition(m_pos);
	}

	void EnemyBullet::OnCollisionEnter(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			ThisDestroy();
		}
	}
	void EnemyBullet::ThisDestroy() {
		GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
	}

	void EnemyBullet::Debug() {
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss(L"");
		wss << L"pos : ( "
			<< m_pos.x
			<< L" , "
			<< m_pos.y
			<< L" , "
			<< m_pos.z
			<< L" ) "
			<<L"\nbefor : "
			<< m_beforFlag
			<<L"\nfloor : "
			<< m_enemy->GetFloorFlag()
			<<L"\ndic : "
			<<m_beforFlag
			<<"\n"
			<<m_test
			<< endl;
		scene->SetDebugString(wss.str());


	}
}
