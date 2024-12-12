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
		m_rad(0.0f),
		m_dicUp(0),
		m_direcNorm(Vec3(0.0f)),
		m_dropTime(4.0f),
		m_maxDropTime(m_dropTime),
		m_hitDropTime(1.0f),
		m_maxHitDropTime(m_hitDropTime),
		m_spareTime(0.75f),
		m_maxSpareTime(m_spareTime),
		m_bulletTime(0.1f),
		m_maxBulletTime(m_bulletTime),
		m_pBulletTime(3.0f),
		m_maxPbulletTime(m_pBulletTime),
		m_bulletCnt(0),
		m_bulletRangeTime(5.0f),
		m_maxBulletRangeTime(m_bulletRangeTime),
		m_trackingRange(30.0f),
		m_efcTime(0.0f),
		m_firstDirec(Vec3(0.0f)),
		m_bulletDic(Vec2(0.0f, 1.0f)),
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
		m_pGrabFlag(false),
		m_playerFlag(false),
		m_overHeatSE(false),
		m_plungeSE(false)
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
		m_rad(0.0f),
		m_dicUp(0),
		m_direcNorm(Vec3(0.0f)),
		m_dropTime(4.0f),
		m_maxDropTime(m_dropTime),
		m_hitDropTime(1.0f),
		m_maxHitDropTime(m_hitDropTime),
		m_spareTime(0.75f),
		m_maxSpareTime(m_spareTime),
		m_bulletTime(0.1f),
		m_maxBulletTime(m_bulletTime),
		m_pBulletTime(3.0f),
		m_maxPbulletTime(m_pBulletTime),
		m_bulletCnt(0),
		m_bulletRangeTime(5.0f),
		m_maxBulletRangeTime(m_bulletRangeTime),
		m_trackingRange(30.0f),
		m_efcTime(2.0f),
		m_firstDirec(Vec3(0.0f)),
		m_bulletDic(Vec2(0.0f, 1.0f)),
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
		m_pGrabFlag(false),
		m_playerFlag(false),
		m_overHeatSE(false),
		m_plungeSE(false)
	{}

	void Enemy::OnCreate() {
		m_trans = GetComponent<Transform>();
		m_trans->SetRotation(Vec3(m_rot.x,m_rot.y+XM_PIDIV2,m_rot.z));
		m_fastState = m_stateType;
		m_beforePos = m_pos;
		
		auto player = m_player.lock();
		if (!player) return;
		m_playerScale = m_player.lock()->GetScale();
		float rad = XMConvertToRadians(180.0f);
		m_draw = AddComponent<PNTBoneModelDraw>();

		if (m_stateType == slide) {
			m_meshName = L"ENEMYYOKO";
			m_scal.y = m_scal.y * 2;
			m_pos.y = m_pos.y + m_scal.y / 2;

			Mat4x4 meshMat;
			meshMat.affineTransformation(
				Vec3(1.0f, 1.0f*0.5, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, rad, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
			m_draw->SetMeshToTransformMatrix(meshMat);
		}
		else {
			Mat4x4 meshMat;
			meshMat.affineTransformation(
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, rad, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
			m_draw->SetMeshToTransformMatrix(meshMat);
		}
		m_trans->SetScale(m_scal);
		m_trans->SetPosition(m_pos);
		//描画

		m_draw->SetMeshResource(m_meshName);
		m_draw->SetOwnShadowActive(true);

		m_draw->AddAnimation(L"walk", 10, 30, true, 30);    //歩き
		m_draw->AddAnimation(L"speedWalk", 10, 30, true, 60);    //歩き

		m_draw->AddAnimation(L"jump", 10, 30, false, 15);
		m_draw->AddAnimation(L"attack", 50, 40, false, 30); //攻撃
		m_draw->AddAnimation(L"spare", 50, 10, false, 15);  //突っ込み前の予備動作
		m_draw->AddAnimation(L"wait", 90, 25, false, 30);   //オーバーヒート状態
		m_draw->AddAnimation(L"stand", 120, 30, false, 30);

		m_draw->AddAnimation(L"walks", 10, 59, true, 30);    //歩き
		m_draw->AddAnimation(L"jumps", 10, 30, false, 30);
		m_draw->AddAnimation(L"waits", 210, 30, false, 30);  //突っ込み前の予備動作

		m_draw->AddAnimation(L"kaihi", 40, 60, true, 30);
		m_draw->AddAnimation(L"hassya", 100, 10, false, 30);
		m_draw->AddAnimation(L"overheat", 120, 30, false, 30);
		m_draw->AddAnimation(L"stand2", 160, 20, false, 30);

		m_draw->ChangeCurrentAnimation(L"walk");
		//衝突判定
		m_collision = AddComponent<CollisionObb>();
		m_collision->SetAfterCollision(AfterCollision::Auto);
		m_collision->SetFixed(false);
		m_collision->SetDrawActive(false);
		//敵の別コリジョンとの判定をなくす
		m_collision->AddExcludeCollisionTag(L"EnemyFloor");
		//影
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(m_meshName);

		//足場コリジョンの追加
		m_floorCol = GetStage()->AddGameObject<EnemyFloorCol>(GetThis<Enemy>());
		m_floorCol->SetDrawActive(false);
		//オーバーヒートゲージの追加
		GetStage()->AddGameObject<GaugeSquare>(4.0f, 2.0f, L"OverHeatGauge",
			Col4(1.0f, 0.0f, 0.0f, 1.0f), GetThis<Enemy>());
		GetStage()->AddGameObject<Square>(4.0f, 2.0f, L"OverHeatFram",
			Col4(1.0f, 1.0f, 1.0f, 1.0f), GetThis<Enemy>());
		AddTag(L"Enemy");

		//エフェクトの設定
		wstring DataDir;
		App::GetApp()->GetAssetsDirectory(DataDir);
		wstring effectSmoke = DataDir + L"Effects\\smoke.efk";
		wstring effectEye = DataDir + L"Effects\\EnemyEye.efk";
		auto stageManager = GetStage()->GetSharedGameObject<StageManager>(L"StageManager");
		auto efkInterface = stageManager->GetEfkInterface();
		m_heatEffect = ObjectFactory::Create<EfkEffect>(efkInterface, effectSmoke);
		m_eyeEffect = ObjectFactory::Create<EfkEffect>(efkInterface, effectEye);
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
		auto shaft = GetDirec().cross(Vec3(0.0f, 1.0f, 0.0f));

		FindFixed();
		if (m_stateType != m_beforeState) {
			m_plungeFlag = false;
			m_jumpMoveFlag = false;
			m_spareTime = m_maxSpareTime;
			m_plungeSE = false;
			m_overHeatSE = false;
		}
		//行動パターン
		switch (m_stateType)
		{
		//移動なし
		case stay:
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			if (m_fastState == slide) {
				EnemyAnime(L"overheat");
			}
			else {
				EnemyAnime(L"wait");
			}
			break;
		//追従の左右移動
		case rightMove:
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			EnemyAnime(L"walk");
			m_speed = m_maxSpeed;
			PlayerDic();
			if (!m_floorCol->GetPlayerFlag()) {
				EnemyAngle();

			}
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
			FallBullet();
			break;
		//浮遊
		case fly:
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			OneJump(3.0f);
			//EnemyAngle();
			//RemoveTag(L"Enemy");
			break;
		//プレイヤーに触れた位置で固定
		case fixedStay:
			GravZero();
			Grav();
			m_pos = m_deathPos;
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
			FallBullet();
			EnemyAngle();
			break;
		case bulletMove:
			PlayerDic();
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			RapidFireBullet(3);
			break;
		case wait:
			if (m_fastState == slide) {
				EnemyAnime(L"stand2");
			}
			else {
				EnemyAnime(L"stand");
			}
			if (!m_overHeatSE) {
				EffectPlay(m_eyeEffect, GetEyePos(Vec3(2.0f, 0.5f, 0.5f)),1,Vec3(0.5f));
				EffectPlay(m_eyeEffect, GetEyePos(Vec3(2.0f, 0.5f, -0.5f)),2, Vec3(0.5f));
				PlaySE(L"EnemyRevival",2.0f);
				m_overHeatSE = true;
			}
			m_spareTime -= elapsed;
			if (m_spareTime <= 0.0f) {
				
				m_stateType = m_fastState;
			}
			break;
		case slide:
			EnemyAnime(L"kaihi");
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			PlayerDic();
			EnemyAngle();
			m_maxBulletTime = 3.0f;
			StraightXBullet();
			m_rad += 0.05f;
			if (XMConvertToRadians(m_rad)>=360) {
				m_rad = 0;
			}

			m_pos += shaft.normalize() * sinf(m_rad) * 10.0f * elapsed;
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
		m_draw->UpdateAnimation(elapsed);
		//Debug();
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
		if (!m_playerFlag) {
			PlayerDic();
			auto front = GetDirec();
			auto elapsed = App::GetApp()->GetElapsedTime();
			front.y = 0;
			front.normalize();
			m_angle = atan2(front.z, front.x);
			float rad = XMConvertToRadians(90.0f);
			m_trans->SetRotation(Vec3(0.0f, -m_angle+rad, 0.0f));
		}
	}
	Vec3 Enemy::GetEyePos(const Vec3& eye) {
		Vec3 pos = m_pos;
		Vec3 forward = m_trans->GetForward();
		float face = atan2f(forward.z, forward.x);
		Vec3 eyePos;
		eyePos.x = (cosf(face) * eye.x) - (sinf(face) * eye.z);
		eyePos.y = eye.y;
		eyePos.z = (cosf(face) * eye.z) + (sinf(face) * eye.x);
		eyePos = eyePos * m_scal / 2;
		pos += eyePos;
		return pos;
	}
	//オーバーヒート
	void Enemy::OverHeat() {
		float elapsed = App::GetApp()->GetElapsedTime();
		if (m_heat >= m_maxHeat) {
			m_stateType = m_overHeatState;
			EffectPlay(m_heatEffect,m_pos,3);
		}
		if (m_heat > 0.0f) {
			m_heat -= elapsed * 5;
			m_efcTime -= elapsed;
			//EffectPlay(m_heatEffect, m_pos, 3);
			//if (m_efcTime <= 0.0f && m_heat >= 20.0f) {
			//	EffectPlay(m_heatEffect, m_pos, 3);
			//	m_efcTime = 2.0f;
			//}
			
		}
		else if (GetOverHeat()&&m_heat <= 0.0f) {
			m_heat = 0.0f;
			m_stateType = wait;
		}
		if (m_EfkPlayer[2]) {
			if (!m_pGrabFlag) {
				m_EfkPlayer[2]->SetLocation(Vec3(m_pos));
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
		if (!m_plungeSE) {
			PlaySE(L"EnemyAttack");
			m_plungeSE = true;
		}
		if (m_spareTime <= 0.0f) {
			if (!m_plungeFlag) {
				m_firstDirec = m_playerPos - GetChangePos();
				m_plungeFlag = true;
				PlaySE(L"EnemyDash");
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
		if (!m_plungeSE) {
			PlaySE(L"EnemyAttack");
			m_plungeSE = true;
		}
		m_spareTime -= elapsed;
		if (m_spareTime <= 0.0f) {
			if (!m_jumpMoveFlag) {
				PlayerDic();
				EnemyAngle();
				EnemyJump();
				PlaySE(L"EnemyDash");
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
	//弾の発射
	//放物線に発射する弾
	void Enemy::FallBullet() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto stage = GetStage();
		m_pBulletTime += elapsed;
		if (!m_bulletFlag) {
			stage->AddGameObject<ParabolaBullet>(GetThis<Enemy>());
			m_bulletFlag = true;
			m_pBulletTime = 0.0f;
		}
		if (m_pBulletTime >= m_maxPbulletTime) {
			m_bulletFlag = false;
		}

	}
	//真っ直ぐ発射する弾
	void Enemy::StraightXBullet() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto stage = GetStage();
		m_bulletTime -= elapsed;
		if (m_bulletTime <= 0.0f) {
			m_bulletFlag = false;
			m_bulletTime = m_maxBulletTime;
		}
		if (!m_bulletFlag) {
			m_bulletCnt++;
			stage->AddGameObject<StraightBullet>(GetThis<Enemy>());
			m_bulletFlag = true;
		}
	}
	void Enemy::RapidFireBullet(int bulletNum) {
		auto elapsed = App::GetApp()->GetElapsedTime();
		m_pos += m_speed * 0.2f * Vec3(m_direc.x * m_bulletDic.x, 0.0f,
			m_direc.z * m_bulletDic.y) * elapsed;
		if (m_bulletRangeTime <= 0.0f) {
			m_bulletRangeTime = m_maxBulletRangeTime;
			m_bulletCnt = 0;
		}
		if (m_bulletCnt < bulletNum) {
			StraightXBullet();
			m_speed = 0.0f;
		}
		else if (m_bulletCnt >= bulletNum) {
			m_bulletRangeTime -= elapsed;
			m_speed = m_maxSpeed;
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
	//削除
	void Enemy::ThisDestroy() {
		GetStage()->RemoveGameObject<Enemy>(GetThis<Enemy>());
		m_floorCol->ThisDestroy();
	}

	//衝突判定
	void Enemy::OnCollisionEnter(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			m_deathPos = m_pos;
			m_playerFlag = true;
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
			if (!m_overHeatSE) {
				PlaySE(L"OverHeatSE",5.0f);
				m_overHeatSE = true;
			}
		}
		if (other->FindTag(L"PlayerGrab")) {
			m_playerGrab = dynamic_pointer_cast<PlayerGrab>(other);
			if (GetOverHeat()) {
				m_pGrabFlag = true;
			}
			else {
				m_pGrabFlag = false;
			}
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
			auto pGrab = m_playerGrab.lock();
			if(!pGrab) return;
			auto grabTrans = pGrab->GetComponent<Transform>();
			if (pGrab) {
				m_trans->SetParent(pGrab);
				m_pos = Vec3(0.0f, 0.0f, 0.0f);
				m_trans->SetPosition(m_pos);
				if (m_EfkPlayer[2]) {
					m_EfkPlayer[2]->SetLocation(grabTrans->GetPosition());
				}

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
		if (other->FindTag(L"Player")) {
			m_playerFlag = false;
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

		if (other->FindTag(L"Player")) {
			m_playerFlag = false;
		}

	}
	//SEの再生
	void Enemy::PlaySE(wstring path, float volume, float loopcnt) {
		auto playerSE = App::GetApp()->GetXAudio2Manager();
		playerSE->Start(path, loopcnt, volume);
	}
	//アニメーションの再生
	void Enemy::EnemyAnime(wstring anime) {
		auto draw = GetComponent<PNTBoneModelDraw>()->GetCurrentAnimation();
		if ( draw!= anime) {
			GetComponent<PNTBoneModelDraw>()->ChangeCurrentAnimation(anime);
		}
	}

	//エフェクトの再生
	void Enemy::EffectPlay(const shared_ptr<EfkEffect>& efk,const Vec3& pos, 
		const int num, const Vec3& scale) {
		m_EfkPlayer[num - 1] = ObjectFactory::Create<EfkPlay>(efk, pos, 0.0f);
		
		m_EfkPlayer[num - 1]->SetScale(scale);
		m_EfkPlayer[num - 1]->SetAllColor(Col4(1.0f));
	}
	//デバック
	void Enemy::Debug() {
		auto& keyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (keyState.m_bPushKeyTbl[VK_DOWN]) {
			//m_stateType = plunge;

		}
		if (keyState.m_bPressedKeyTbl[VK_UP]) {
			//HipDropJump();
			//stage->AddGameObject<ParabolaBullet>(GetThis<Enemy>());
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
			<<L"\n"
			<< m_beforePos.length()
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
	void Enemy::SetBulletDirec(Vec2 direc) {
		m_bulletDic = direc;
	}

	//--------------------------------------------------------------------------------------
	//	class EnemyBullet : public GameObject;  
	//--------------------------------------------------------------------------------------
	EnemyBullet::EnemyBullet(const shared_ptr<Stage>& stage
	):
		GameObject(stage),
		m_colTime(0.0f),
		m_playerColFlag(false)
	{}
	void EnemyBullet::OnCreate() {
		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"DEFAULT_SPHERE");
		m_draw->SetTextureResource(L"AreaDoorBLUCK");
		m_draw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));

		//衝突判定
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetDrawActive(false);
		//影
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		AddTag(L"EnemyBullet");
	}
	void EnemyBullet::SetColor(Col4 color) {
		m_draw->SetDiffuse(color);
	}
	void EnemyBullet::ThisDestroy() {
		GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
	}
	void EnemyBullet::OnCollisionEnter(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			m_playerColFlag = true;
			m_draw->SetDrawActive(false);
		}
		if (other->FindTag(L"Wall")) {
			ThisDestroy();
		}
		if (other->FindTag(L"Floor")) {
			ThisDestroy();
		}
		if (other->FindTag(L"GimmickDoor")) {
			ThisDestroy();
		}
	}
	void EnemyBullet::OnUpdate() {
		float elapsed = App::GetApp()->GetElapsedTime();
		if (m_playerColFlag) {
			m_colTime += elapsed;
		}
		if (m_colTime >= 0.1f) {
			ThisDestroy();
			m_playerColFlag = false;
			m_colTime = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------
	//	class StraightXBullet : public EnemyBullet;  
	//--------------------------------------------------------------------------------------

	StraightBullet::StraightBullet(const shared_ptr<Stage>& stage,
		const shared_ptr<Enemy>& enemy
	):
		EnemyBullet(stage),
		m_enemy(enemy),
		m_speed(30.0f),
		m_Range(70.0f),
		m_pos(Vec3(0.0f)),
		m_scal(Vec3(1.0f))

	{}
	void StraightBullet::OnCreate() {
		EnemyBullet::OnCreate();
		m_trans = GetComponent<Transform>();
		m_trans->SetRotation(Vec3(0.0f,0.0f, 0.0f));
		m_trans->SetScale(m_scal);

		auto enemy = m_enemy.lock();
		if (!enemy) return;
		m_enemyPos = enemy->GetChangePos();
		m_trans->SetPosition(m_enemyPos);

	}
	void StraightBullet::OnUpdate() {
		EnemyBullet::OnUpdate();
		float elapsed = App::GetApp()->GetElapsedTime();
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		auto enemyFor= enemy->GetComponent<Transform>()->GetForward();
		m_pos = m_trans->GetPosition();
		m_pos.y = m_enemyPos.y;
		m_pos += enemyFor *m_speed * elapsed;
		Vec3 pos = m_enemyPos - m_pos;
		if (pos.length() >= m_Range) {
			ThisDestroy();
		}
		m_trans->SetPosition(m_pos);

		//Debug();
	}
	void StraightBullet::Debug() {
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

	//--------------------------------------------------------------------------------------
	//	class ParabolaBullet : public EnemyBullet;  
	//--------------------------------------------------------------------------------------
	ParabolaBullet::ParabolaBullet(const shared_ptr<Stage>& stage,
		const shared_ptr<Enemy>& enemy
	) :
		EnemyBullet(stage),
		m_enemy(enemy),
		m_pos(Vec3(0, 0, 0)),
		m_rot(Vec3(0, 0, 0)),
		m_scal(Vec3(1.5f, 1.5f, 1.5f)),
		m_speed(20.0f),
		m_grav(Vec3(0.0f,-9.8f,0.0f))
	{}
	void ParabolaBullet::OnCreate() {
		EnemyBullet::OnCreate();
		StartVel();
	}

	void ParabolaBullet::StartVel() {
		m_trans = GetComponent<Transform>();
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scal);
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		m_enemyPos = enemy->GetChangePos();

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
	void ParabolaBullet::Grav() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		m_gravVel += m_grav * elapsed;
		m_pos += m_gravVel * elapsed;
		m_trans->SetPosition(m_pos);
	}
	void ParabolaBullet::OnUpdate() {
		EnemyBullet::OnUpdate();
		Grav();
		auto elapsed = App::GetApp()->GetElapsedTime();

		m_pos = m_trans->GetPosition();
		m_pos += m_velocity * elapsed;
		m_trans->SetPosition(m_pos);

		if (m_pos.y <= -10) {
			ThisDestroy();
		}
		//Debug();
	}
	void ParabolaBullet::Debug() {
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
		m_enemy(enemy),
		m_playerFlag(false)
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
	void EnemyFloorCol::OnCollisionEnter(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			m_playerFlag = true;
		}
	}
	void EnemyFloorCol::OnCollisionExit(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			m_playerFlag = false;
		}
	}
	bool EnemyFloorCol::GetPlayerFlag() {
		return m_playerFlag;
	}
}
