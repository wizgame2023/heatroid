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
		m_meshName(L"ENEMYARUKU"),
		m_heat(0),
		m_maxHeat(100),
		m_angle(0.0f),
		m_speed(5.0f),
		m_maxSpeed(5.0f),
		m_upSpeed(3.0f),
		m_upHeight(10.0f),
		m_jumpPower(5.0f),
		m_jumpTime(1.0f),
		m_dicUp(0),
		m_direcNorm(Vec3(0.0f)),
		m_dropTime(4.0f),
		m_maxDropTime(m_dropTime),
		m_hitDropTime(1.0f),
		m_maxHitDropTime(m_hitDropTime),
		m_spareTime(0.75f),
		m_maxSpareTime(m_spareTime),
		m_bulletTime(5.0f),
		m_maxBulletTime(m_bulletTime),
		m_trackingRange(20.0f),
		m_firstDirec(Vec3(0.0f)),
		m_gravity(-9.8f),
		m_grav(Vec3(0.0f, m_gravity, 0.0f)),
		m_gravVel(Vec3(0.0f)),
		m_moveRot(Vec3(0.0f)),
		m_bulletFlag(true),
		m_jumpFlag(false),
		m_jumpMoveFlag(false),
		m_flyFlag(false),
		m_floorFlag(false),
		m_hitDropFlag(false),
		m_plungeFlag(false),
		m_pGrabFlag(false)
	{}
	Enemy::Enemy(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotatoin,
		const Vec3& scale,
		const State& state,
		const State& overHeatState,
		const wstring& meshName,
		const shared_ptr<Player>& player
	) :
		GameObject(stage),
		m_pos(position),
		m_rot(rotatoin),
		m_scal(scale),
		m_stateType(state),
		m_overHeatState(overHeatState),
		m_meshName(meshName),
		m_player(player),
		m_heat(0),
		m_maxHeat(100),
		m_angle(0.0f),
		m_speed(5.0f),
		m_maxSpeed(5.0f),
		m_upSpeed(3.0f),
		m_upHeight(10.0f),
		m_jumpPower(5.0f),
		m_jumpTime(1.0f),
		m_dicUp(0),
		m_direcNorm(Vec3(0.0f)),
		m_dropTime(4.0f),
		m_maxDropTime(m_dropTime),
		m_hitDropTime(1.0f),
		m_maxHitDropTime(m_hitDropTime),
		m_spareTime(0.75f),
		m_maxSpareTime(m_spareTime),
		m_bulletTime(5.0f),
		m_maxBulletTime(m_bulletTime),
		m_trackingRange(20.0f),
		m_firstDirec(Vec3(0.0f)),
		m_gravity(-9.8f),
		m_grav(Vec3(0.0f, m_gravity, 0.0f)),
		m_gravVel(Vec3(0.0f)),
		m_moveRot(Vec3(0.0f)),
		m_bulletFlag(true),
		m_jumpFlag(false),
		m_jumpMoveFlag(false),
		m_flyFlag(false),
		m_floorFlag(false),
		m_hitDropFlag(false),
		m_plungeFlag(false),
		m_pGrabFlag(false)
	{}
	void Enemy::OnCreate() {
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scal);
		m_fastState = m_stateType;
		m_beforePos = m_pos;
		auto player = m_player.lock();
		if (!player) return;
		m_playerScale = m_player.lock()->GetScale();
		//描画
		
		auto ptrDraw = AddComponent<PNTBoneModelDraw>();
		float rad = XMConvertToRadians(-90.0f);
		Mat4x4 meshMat;
		meshMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, rad, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		ptrDraw->SetMeshResource(m_meshName);
		ptrDraw->SetMeshToTransformMatrix(meshMat);
		ptrDraw->SetOwnShadowActive(true);

		ptrDraw->AddAnimation(L"walk", 10, 30, true, 30);    //歩き
		ptrDraw->AddAnimation(L"jump", 10, 30, false, 15);
		ptrDraw->AddAnimation(L"attack", 50, 40, false, 30); //攻撃
		ptrDraw->AddAnimation(L"spare", 50, 10, false, 15);  //突っ込み前の予備動作
		ptrDraw->AddAnimation(L"wait", 90, 25, false, 30);   //オーバーヒート状態
		ptrDraw->ChangeCurrentAnimation(L"walk");
		//衝突判定
		m_collision = AddComponent<CollisionObb>();
		m_collision->SetAfterCollision(AfterCollision::Auto);
		m_collision->SetFixed(false);
		m_collision->SetDrawActive(false);
		//敵の別コリジョンとの判定をなくす
		m_collision->AddExcludeCollisionTag(L"EnemyFloor");
		//影
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		//足場コリジョンの追加
		auto floorCol = GetStage()->AddGameObject<EnemyFloorCol>(GetThis<Enemy>());
		floorCol->SetDrawActive(false);
		//オーバーヒートゲージの追加
		GetStage()->AddGameObject<GaugeSquare>(4.0f, 2.0f, L"OverHeatGauge",
			Col4(1.0f, 0.0f, 0.0f, 1.0f), GetThis<Enemy>());
		GetStage()->AddGameObject<Square>(4.0f, 2.0f, L"OverHeatFram",
			Col4(1.0f, 1.0f, 1.0f, 1.0f), GetThis<Enemy>());
		AddTag(L"Enemy");
	}

	void Enemy::OnUpdate() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto stage = GetStage();
		m_pos = m_trans->GetPosition();
		//プレイヤー情報を取得
		auto player = m_player.lock();
		if (!player) return;
		m_playerTrans = player->GetComponent<Transform>();
		m_playerPos = m_playerTrans.lock()->GetPosition();

		Debug();
		FindFixed();
		if (m_stateType != m_beforeState) {
			m_plungeFlag = false;
			m_jumpMoveFlag = false;
			m_spareTime = m_maxSpareTime;
		}
		//行動パターン
		switch (m_stateType)
		{
		//移動なし
		case stay:
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			EnemyAnime(L"wait");
			break;
		//追従の左右移動
		case rightMove:
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			EnemyAnime(L"walk");
			m_speed = m_maxSpeed;

			PlayerDic();
			EnemyAngle();
			if (m_direc.length() <= m_trackingRange * 2) {
				m_pos += m_speed * m_direcNorm * elapsed;
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
			OneJump(0.5f);
			PlayerDic();
			EnemyAngle();
			Bullet();
			break;
		//浮遊
		case fly:
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			OneJump(5.0f);
			EnemyAngle();
			break;
		//プレイヤーに触れた位置で固定
		case fixedStay:
			GravZero();
			Grav();
			m_pos = m_deathPos;
			m_collision->SetFixed(true);
			break;
		//ジャンプ
		case jump:
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			JumpMove();
			break;
		//突っ込み
		case plunge:
			//EnemyAnime(L"spare");
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			Plunge();
			break;
		//弾を撃ってくる
		case bullet:
			EnemyAnime(L"walk");
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			Bullet();
			EnemyAngle();
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
		m_beforeState = m_stateType;
		if (GetOverHeat()) {
			Grab();
		}
		OverHeat();
		auto draw = GetComponent<PNTBoneModelDraw>();
		draw->UpdateAnimation(elapsed);
		Debug();
	}

	//ジャンプ
	void Enemy::EnemyJump() {
		m_floorFlag = false;
		AddGrav(Vec3(0.0f, m_jumpPower*2, 0.0f));
		SetGrav(Vec3(0.0f, m_gravity, 0.0f));
		Grav();
	}
	void Enemy::HipDropJump() {
		m_hitDropFlag = true;
		GravZero();
		EnemyJump();
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
			m_direcNorm = dic.normalize();

		}
	}
	//プレイヤーの方向を向かせる
	void Enemy::EnemyAngle()
	{
		PlayerDic();
		auto front = GetDirec();
		auto elapsed = App::GetApp()->GetElapsedTime();
		front.y = 0;
		front.normalize();
		m_angle = atan2(front.z, front.x);
		float rad = XMConvertToRadians(90.0f);
		m_trans->SetRotation(Vec3(0.0f, -m_angle, 0.0f));
	}
	//オーバーヒート
	void Enemy::OverHeat() {
		float elapsed = App::GetApp()->GetElapsedTime();
		if (m_heat >= m_maxHeat) {
			m_stateType = m_overHeatState;
		}
		if (m_heat > 0.0f) {
			m_heat -= elapsed * 5;
		}
		else if (GetOverHeat()&&m_heat <= 0.0f) {
			m_heat = 0.0f;
			if (m_stateType != m_fastState) {
				m_stateType = m_fastState;
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
	void Enemy::Plunge() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		m_spareTime -= elapsed;
		if (m_spareTime <= 0.0f) {
			if (!m_plungeFlag) {
				m_firstDirec = m_playerPos - GetChangePos();
				m_plungeFlag = true;
			}
			if (m_direc.length() <= m_trackingRange) {
				m_pos += Vec3(m_firstDirec.x, 0.0f, m_firstDirec.z) * m_speed * 0.2f * elapsed;
			}
			m_spareTime = 0.0f;
		}
	}
	void Enemy::JumpMove() {
		float elapsed = App::GetApp()->GetElapsedTime();
		EnemyAnime(L"spare");
		m_spareTime -= elapsed;
		if (m_spareTime <= 0.0f) {
			if (!m_jumpMoveFlag) {
				PlayerDic();
				EnemyAngle();
				EnemyJump();
				m_jumpMoveFlag = true;
				m_speed = m_maxSpeed * 6;
			}
			if (m_direc.length() <= m_trackingRange * 2) {
				m_pos += m_speed * m_direcNorm * elapsed;
			}
			if (m_floorFlag) {
				m_speed = 0.0f;
			}
			else {
				EnemyAnime(L"jump");
			}

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
					m_speed = m_maxSpeed;

				}
				if (m_hitDropTime <= 0) {
					if (m_pos.y - m_scal.y / 2 + fixPos.y >= fixPos.y + fixScal.y / 2 + 0.01f) {
						m_pos.y += -m_maxSpeed * 5 * elapsed;
						m_speed = m_maxSpeed * 5;
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
		m_bulletTime += elapsed;
		if (!m_bulletFlag) {
			stage->AddGameObject<EnemyBullet>(GetThis<Enemy>());
			m_bulletFlag = true;
			m_bulletTime = 0.0f;
		}
		if (m_bulletTime >= m_maxBulletTime) {
			m_bulletFlag = false;
		}

	}
	//削除
	void Enemy::ThisDestroy() {
		GetStage()->RemoveGameObject<Enemy>(GetThis<Enemy>());
	}
	//SEの再生
	void Enemy::PlayerSE(wstring path, float volume, float loopcnt) {
		auto playerSE = App::GetApp()->GetXAudio2Manager();
		playerSE->Start(path, loopcnt, volume);
	}

	//衝突判定
	void Enemy::OnCollisionEnter(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			m_deathPos = m_pos;

		}
		if (other->FindTag(L"Wall")) {
			auto breakWall = dynamic_pointer_cast<BreakWall>(other);
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
			PlayerSE(L"OverHeatSE");
		}
		if (other->FindTag(L"PlayerGrab")) {
			m_playerGrab = dynamic_pointer_cast<PlayerGrab>(other);
			m_pGrabFlag = true;
		}

	}
	void Enemy::Grab() {
		auto pad = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (pad[0].wReleasedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ||
			keyState.m_bUpKeyTbl['Q'] == true) {
			m_pGrabFlag = false;
		}
		if (m_pGrabFlag) {
			m_floorFlag = false;
			auto player = m_player.lock();
			if (!player) return;
			auto pGrab = m_playerGrab.lock();
			if(!pGrab) return;
			auto playerPos = player->GetComponent<Transform>()->GetPosition();
			auto playerFor = player->GetComponent<Transform>()->GetForward();
			auto grabScal = m_playerGrab.lock()->GetComponent<Transform>()->GetScale();
			Vec3 scal = Vec3(grabScal.x / 2.5, 0.0f, grabScal.z / 2.5);
			auto grabPos = pGrab->GetComponent<Transform>();
			float rad = XMConvertToRadians(-45);
			if (pGrab) {
				m_trans->SetParent(pGrab);
				m_pos = Vec3(0.0f, 0.0f, 0.0f);
				m_trans->SetPosition(m_pos);

			}
			else {
				return;
			}
		}
		else {
			m_trans->ClearParent();
		}
	}
	void Enemy::OnCollisionExit(shared_ptr<GameObject>& other)
	{
		if ((other->FindTag(L"GimmickButton")))
		{
			auto group = GetStage()->GetSharedObjectGroup(L"Switch");
			auto& vec = group->GetGroupVector();
			for (auto& v : vec) {
				auto shObj = v.lock();
				if (other == shObj) {
					auto Switchs = dynamic_pointer_cast<GimmickButton>(shObj);
					Switchs->SetButton(false);
				}
			}
		}
		if (other->FindTag(L"PlayerGrab")) {
			//m_trans->ClearParent();
			//m_pGrabFlag = false;
			//m_playerGrabFlag = true;

		}
	}

	void Enemy::OnCollisionExcute(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Floor")) {
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
	}

	void Enemy::EnemyAnime(wstring anime) {
		auto draw = GetComponent<PNTBoneModelDraw>()->GetCurrentAnimation();
		if ( draw!= anime) {
			GetComponent<PNTBoneModelDraw>()->ChangeCurrentAnimation(anime);
		}

	}
	//デバック
	void Enemy::Debug() {
		auto& keyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (keyState.m_bPushKeyTbl[VK_DOWN]) {
			//m_stateType = plunge;

		}
		if (keyState.m_bPressedKeyTbl[VK_UP]) {
			//HipDropJump();
			//stage->AddGameObject<EnemyBullet>(GetThis<Enemy>());
		}
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
			<<L"\nheat"
			<< m_heat
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
	float Enemy::GetAngle() {
		return m_angle;
	}
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
		Vec3 pos = m_trans->GetWorldPosition();
		return pos;
		//auto fixed = m_fixedBox.lock();
		//if (fixed) {
		//	//Vec3 pos = m_pos + fixed->GetComponent<Transform>()->GetPosition();
		//	return pos;
		//}
		//else {
		//	return m_pos;
		//}
	}
	bool Enemy::GetOverHeat() {
		if (m_stateType == m_overHeatState) {
			return true;
		}
		else {
			return false;
		}
	}
	void Enemy::SetPlungeFlag(bool flag) {
		m_plungeFlag = flag;
	}

	//--------------------------------------------------------------------------------------
	//	class EnemyBullet : public GameObject;  
	//--------------------------------------------------------------------------------------
	EnemyBullet::EnemyBullet(const shared_ptr<Stage>& stage,
		const shared_ptr<Enemy>& enemy
	) :
		GameObject(stage),
		m_enemy(enemy),
		m_pos(Vec3(0, 0, 0)),
		m_rot(Vec3(0, 0, 0)),
		m_scal(Vec3(1.5f, 1.5f, 1.5f)),
		m_speed(20.0f),
		m_Range(1.0f),
		m_power(15.0f),
		m_maxPower(m_power),
		m_grav(Vec3(0.0f,-9.8f,0.0f)),
		m_floorCheck(false)
	{}
	void EnemyBullet::OnCreate() {
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

		StartVel();
	}
	void EnemyBullet::StartVel() {
		m_trans = GetComponent<Transform>();
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scal);
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		m_enemyPos = enemy->GetChangePos();
		m_beforFlag = enemy->GetFloorFlag();

		m_pos = m_enemyPos;
		m_pos.y += 5.0f;
		float t;           //仮の時間
		float Vy = 10.0f;  //飛ばず高さ
		t = Vy / (-m_grav.y / 2);
		float direcVelo = enemy->GetDirec().length() / t;
		float angle = enemy->GetAngle();
		//プレイヤーの方向に飛ばすベクトル
		Vec3 velo = Vec3(direcVelo * cos(angle), Vy, direcVelo * sin(angle));
		m_velocity = velo;
		m_trans->SetPosition(m_pos);

	}
	void EnemyBullet::Grav() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		m_gravVel += m_grav * elapsed;
		m_pos += m_gravVel * elapsed;
		m_trans->SetPosition(m_pos);
	}
	void EnemyBullet::OnUpdate() {
		Grav();
		auto elapsed = App::GetApp()->GetElapsedTime();

		m_pos = m_trans->GetPosition();
		m_pos += m_velocity * elapsed;
		m_trans->SetPosition(m_pos);

		if (m_pos.y <= -10) {
			ThisDestroy();
		}
		Debug();
	}

	void EnemyBullet::OnCollisionEnter(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			ThisDestroy();
		}
		if (other->FindTag(L"Wall")) {
			ThisDestroy();
		}
		if (other->FindTag(L"Floor")) {
			ThisDestroy();
		}
	}
	void EnemyBullet::ThisDestroy() {
		GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
	}

	void EnemyBullet::Debug() {
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		auto scene = App::GetApp()->GetScene<Scene>();
		auto enemy = m_enemy.lock();
		wstringstream wss(L"");
		wss << L"pos : ( "
			<< m_pos.x
			<< L" , "
			<< m_pos.y
			<< L" , "
			<< m_pos.z
			<< L" ) "
			<< L"\nDirec : "
			<< m_enemy.lock()->GetDirec().length()
			<< endl;
		scene->SetDebugString(wss.str());
	}

	//敵の上に付ける足場コリジョン
	EnemyFloorCol::EnemyFloorCol(const shared_ptr<Stage>& stage,
		const shared_ptr<Enemy>& enemy
	):
		GameObject(stage),
		m_enemy(enemy)
	{}
	void EnemyFloorCol::OnCreate() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		auto enemyTrans = enemy->GetComponent<Transform>();
		m_enemyPos = enemyTrans->GetPosition();
		m_enemyScal = enemyTrans->GetScale();
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(Vec3(m_enemyPos.x, m_enemyPos.y + m_enemyScal.y / 2, m_enemyPos.z));
		m_trans->SetScale(Vec3(m_enemyScal.x,m_enemyScal.y/5,m_enemyScal.z));
		m_trans->SetParent(enemy);

		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetFixed(false); //空間に固定するか
		ptrColl->SetDrawActive(true);

		AddTag(L"Floor");
		AddTag(L"EnemyFloor");

	}
	void EnemyFloorCol::OnUpdate() {
		auto enemy = m_enemy.lock();
		if (enemy) {
			auto enemyTrans = enemy->GetComponent<Transform>();
			m_enemyPos = enemyTrans->GetPosition();
			m_trans->SetPosition(Vec3(0.0f, m_enemyScal.y / 2.0f, 0.0f));
		}
		else {
			ThisDestroy();
		}

	}
	void EnemyFloorCol::ThisDestroy() {
		GetStage()->RemoveGameObject<EnemyFloorCol>(GetThis<EnemyFloorCol>());
	}
}
