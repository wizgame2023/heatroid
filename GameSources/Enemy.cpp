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
		m_beforePos(position),
		m_rot(rotatoin),
		m_scal(scale),
		m_player(player),
		m_meshName(L"ENEMYARUKU"),
		m_heat(0),
		m_maxHeat(100),
		m_angle(0.0f),
		m_speed(5.0f),
		m_maxSpeed(5.0f),
		m_upSpeed(3.0f),
		m_rad(0.0f),
		m_direcNorm(Vec3(0.0f)),
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
		m_throwLength(1.0f),
		m_firstDirec(Vec3(0.0f)),
		m_bulletDic(Vec2(0.0f, 1.0f)),
		m_gravity(-9.8f),
		m_grav(Vec3(0.0f, m_gravity, 0.0f)),
		m_gravVel(Vec3(0.0f)),
		m_moveRot(Vec3(0.0f)),
		m_bulletFlag(true),
		m_floorFlag(false),
		m_plungeFlag(false),
		m_pGrabFlag(false),
		m_playerFlag(false),
		m_overHeatSE(false),
		m_plungeSE(false),
		m_activeFlag(true),
		m_throwFlag(false),
		m_overHeatFlag(false),
		m_test(1.0f),
		m_throwTime(0.5f)
	{
	}
	Enemy::Enemy(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		const State& state,
		const State& overHeatState,
		const wstring& meshName,
		const shared_ptr<Player>& player
	) :
		GameObject(stage),
		m_beforePos(position),
		m_rot(rotation),
		m_scal(scale),
		m_meshName(meshName),
		m_player(player),
		m_heat(0),
		m_maxHeat(100),
		m_angle(0.0f),
		m_speed(5.0f),
		m_maxSpeed(5.0f),
		m_upSpeed(3.0f),
		m_rad(0.0f),
		m_direcNorm(Vec3(0.0f)),
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
		m_throwLength(1.0f),
		m_firstDirec(Vec3(0.0f)),
		m_bulletDic(Vec2(0.0f, 1.0f)),
		m_gravity(-9.8f),
		m_grav(Vec3(0.0f, m_gravity, 0.0f)),
		m_gravVel(Vec3(0.0f)),
		m_moveRot(Vec3(0.0f)),
		m_bulletFlag(true),
		m_floorFlag(false),
		m_plungeFlag(false),
		m_pGrabFlag(false),
		m_playerFlag(false),
		m_overHeatSE(false),
		m_plungeSE(false),
		m_activeFlag(true),
		m_throwFlag(false),
		m_overHeatFlag(false),
		m_test(1.0f),
		m_throwTime(0.5f)
	{}
	Enemy::Enemy(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		const shared_ptr<Player>& player
	):
		GameObject(stage),
		m_beforePos(position),
		m_rot(rotation),
		m_scal(scale),
		m_meshName(L"ENEMYARUKU"),
		m_player(player),
		m_heat(0),
		m_maxHeat(100),
		m_angle(0.0f),
		m_speed(5.0f),
		m_maxSpeed(5.0f),
		m_upSpeed(3.0f),
		m_rad(0.0f),
		m_direcNorm(Vec3(0.0f)),
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
		m_throwLength(1.0f),
		m_firstDirec(Vec3(0.0f)),
		m_bulletDic(Vec2(0.0f, 1.0f)),
		m_gravity(-9.8f),
		m_grav(Vec3(0.0f, m_gravity, 0.0f)),
		m_gravVel(Vec3(0.0f)),
		m_moveRot(Vec3(0.0f)),
		m_bulletFlag(true),
		m_floorFlag(false),
		m_plungeFlag(false),
		m_pGrabFlag(false),
		m_playerFlag(false),
		m_overHeatSE(false),
		m_plungeSE(false),
		m_activeFlag(true),
		m_throwFlag(false),
		m_overHeatFlag(false),
		m_test(1.0f),
		m_throwTime(0.5f)

	{}

	void Enemy::OnCreate() {
		//ステータスの設定
		m_currentState= make_unique<ChaseState>(GetThis<Enemy>());
		m_previousState = move(m_currentState);

		m_trans = GetComponent<Transform>();
		m_trans->SetRotation(Vec3(m_rot.x,m_rot.y+XM_PIDIV2,m_rot.z));
		auto pos = m_beforePos;

		auto player = m_player.lock();
		if (!player) return;
		m_playerScale = m_player.lock()->GetScale();
		float rad = XMConvertToRadians(180.0f);
		m_draw = AddComponent<PNTBoneModelDraw>();

		//オーバーヒートゲージの追加
		m_gauge = GetStage()->AddGameObject<GaugeSquare>(4.0f, 2.0f, L"OverHeatGauge",
			Col4(1.0f, 0.0f, 0.0f, 1.0f), GetThis<Enemy>());
		m_gauge.lock()->DrawGauge(false);
		m_gaugeFram = GetStage()->AddGameObject<Square>(4.0f, 2.0f, L"OverHeatFram",
			Col4(1.0f, 1.0f, 1.0f, 1.0f), GetThis<Enemy>());
		m_gaugeFram.lock()->DrawGauge(false);
		//足場コリジョンの追加
		m_floorCol = GetStage()->AddGameObject<EnemyFloorCol>(GetThis<Enemy>());
		m_floorCol->SetDrawActive(false);

		//描画
		m_scal.y = m_scal.y * 0.5;
		pos.y = pos.y + m_scal.y * 0.5;

		Mat4x4 meshMat;
		meshMat.affineTransformation(
			Vec3(1.0f, 1.0f * 2.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, rad, 0.0f),
			Vec3(0.0f, -1.5f, 0.0f)
		);
		m_draw->SetMeshToTransformMatrix(meshMat);
		m_floorCol->SetPosHight(1.5f);


		////左右に動く敵の場合モデルの変更と位置の調整
		//if (m_fastState == slide) {
		//	m_meshName = L"ENEMYYOKO";
		//	m_scal.y = m_scal.y * 0.5f;
		//	pos.y = pos.y + m_scal.y;

		//	Mat4x4 meshMat;
		//	meshMat.affineTransformation(
		//		Vec3(1.0f, 1.0f * 2.0f, 1.0f),
		//		Vec3(0.0f, 0.0f, 0.0f),
		//		Vec3(0.0f, rad, 0.0f),
		//		Vec3(0.0f, -1.5f, 0.0f)
		//	);
		//	m_draw->SetMeshToTransformMatrix(meshMat);
		//	gauge->SetPosHight(6.0f);
		//	gaugeFram->SetPosHight(6.0f);
		//	m_floorCol->SetPosHight(3.0f);
		//}
		//else {
		//	m_scal.y = m_scal.y * 0.5;
		//	pos.y = pos.y + m_scal.y * 0.5;

		//	Mat4x4 meshMat;
		//	meshMat.affineTransformation(
		//		Vec3(1.0f, 1.0f * 2.0f, 1.0f),
		//		Vec3(0.0f, 0.0f, 0.0f),
		//		Vec3(0.0f, rad, 0.0f),
		//		Vec3(0.0f, -1.5f, 0.0f)
		//	);
		//	m_draw->SetMeshToTransformMatrix(meshMat);
		//	m_floorCol->SetPosHight(1.5f);
		//}

		m_trans->SetScale(m_scal);
		m_trans->SetPosition(pos);
		//描画
		m_draw->SetMeshResource(m_meshName);
		m_draw->SetOwnShadowActive(true);

		//アニメーションの設定
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

		m_draw->AddAnimation(L"kaihi", 40, 60, true, 35);
		m_draw->AddAnimation(L"hassya", 100, 10, false, 30);
		m_draw->AddAnimation(L"overheat", 120, 30, false, 30);
		m_draw->AddAnimation(L"stand2", 160, 20, false, 30);

		m_draw->ChangeCurrentAnimation(L"walk");

		m_draw->SetDrawActive(true);
		//衝突判定
		m_collision = AddComponent<CollisionCapsule>();
		m_collision->SetAfterCollision(AfterCollision::Auto);
		m_collision->SetFixed(false);
		m_collision->SetDrawActive(false);
		//敵の別コリジョンとの判定をなくす
		m_collision->AddExcludeCollisionTag(L"EnemyFloor");
		//m_collision->SetSleepActive(false);
		//影
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(m_meshName);

		AddTag(L"Enemy");

		//エフェクトの設定
		wstring DataDir;
		App::GetApp()->GetAssetsDirectory(DataDir);
		wstring effectSmoke = DataDir + L"Effects\\smoke.efk";
		wstring effectEye = DataDir + L"Effects\\EnemyEye.efk";
		wstring effectBurst = DataDir + L"Effects\\EnemyBurst.efk";
		auto stageManager = GetStage()->GetSharedGameObject<StageGenerator>(L"StageManager");
		auto efkInterface = stageManager->GetEfkInterface();
		m_heatEffect = ObjectFactory::Create<EfkEffect>(efkInterface, effectSmoke);
		m_eyeEffect = ObjectFactory::Create<EfkEffect>(efkInterface, effectEye);
		m_burstEffect = ObjectFactory::Create<EfkEffect>(efkInterface, effectBurst);
	}

	void Enemy::OnUpdate() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto stage = GetStage();
		auto pos = m_trans->GetPosition();
		//プレイヤー情報を取得
		auto player = m_player.lock();
		if (!player) return;
		m_playerTrans = player->GetComponent<Transform>();
		m_playerPos = m_playerTrans.lock()->GetPosition();
		auto shaft = GetDirec().cross(Vec3(0.0f, 1.0f, 0.0f));

		//FindFixed();

		pos.y = Grav().y;


		m_trans->SetPosition(pos);

		//アニメーションの実装
		m_draw->UpdateAnimation(elapsed);
		//Debug();

	}

	//プレイヤーの方向
	void Enemy::PlayerDic() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto player = m_player.lock();
		if (player) {
			auto playerTrans = player->GetComponent<Transform>();
			auto playerPos = playerTrans->GetPosition();
			m_direc = playerPos - GetPos();
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
			m_trans->SetRotation(Vec3(0.0f, -m_angle + rad, 0.0f));
		}
	}

	//プレイヤーに突っ込む
	Vec3 Enemy::Plunge() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto pos = m_trans->GetPosition();

		m_spareTime -= elapsed;
		if (!m_plungeSE) {
			PlaySE(L"EnemyAttack");
			m_plungeSE = true;
		}
		if (m_spareTime <= 0.0f) {
			if (m_direc.length() <= m_trackingRange) {
				if (!m_plungeFlag) {
					m_firstDirec = m_playerPos - pos;
					m_plungeFlag = true;
					PlaySE(L"EnemyDash");
				}
				pos += Vec3(m_firstDirec.x, 0.0f, m_firstDirec.z) * m_speed * 0.2f * elapsed;
			}
			m_spareTime = 0.0f;
		}
		return pos;
	}

	//周りもオーバーヒートする
	void Enemy::AroundOverHeat() {
		auto stage = GetStage();
		auto enemyGroup = stage->GetSharedObjectGroup(L"Enemy");
		auto& enemyVec = enemyGroup->GetGroupVector();

		for (auto v : enemyVec) {
			auto shObj = v.lock();
			auto enemyObj = dynamic_pointer_cast<Enemy>(shObj);
			auto shEnemyPos = enemyObj->GetWorldPos();
			auto dic = GetWorldPos() - shEnemyPos;
			if (dic.length() < 10 && enemyObj != GetThis<Enemy>()) {
				enemyObj->PlayOverHeat();
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
		auto player = m_player.lock();
		if (!player) return;

		m_bulletTime -= elapsed;
		if (m_bulletTime <= 0.0f) {
			m_bulletFlag = false;
			m_bulletTime = m_maxBulletTime;
		}
		if (!m_bulletFlag) {
			m_bulletCnt++;
			//一方に跳ぶ弾
			stage->AddGameObject<StraightBullet>(GetThis<Enemy>());

			m_bulletFlag = true;
		}
	}
	//連射する弾
	void Enemy::RapidFireBullet(int bulletNum) {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto pos = m_trans->GetPosition();
		pos += m_speed * 0.2f * Vec3(m_direc.x * m_bulletDic.x, 0.0f,
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

	//削除
	void Enemy::ThisDestroy() {
		GetStage()->RemoveGameObject<Enemy>(GetThis<Enemy>());
		m_floorCol->ThisDestroy();
	}

	//衝突判定
	void Enemy::OnCollisionEnter(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			m_playerFlag = true;
		}
		if (other->FindTag(L"Wall")) {
			auto breakWall = dynamic_pointer_cast<BreakWall>(other);
			m_floorFlag = true;
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
			m_activeFlag = true;
		}
		if (other->FindTag(L"Attack")) {
			PlayOverHeat();
			if (!m_overHeatSE) {
				//PlaySE(L"OverHeatSE",5.0f);
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
		if (other->FindTag(L"Floor")) {
			m_floorFlag = true;
		}

	}
	//衝突判定
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
			m_activeFlag = false;
		}
		if (other->FindTag(L"Player")) {
			m_playerFlag = false;
		}
		if (other->FindTag(L"Floor")) {
			m_floorFlag = false;
		}

	}
	void Enemy::OnCollisionExcute(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Floor")) {
			//m_fixedBox = dynamic_pointer_cast<TilingFixedBox>(other);
			//m_trans->SetParent(m_fixedBox.lock());
			//m_floorPos = m_pos;
			m_floorFlag = true;
		}
		if (other->FindTag(L"Wall")) {
			auto breakWall = dynamic_pointer_cast<BreakWall>(other);
			m_floorFlag = true;
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
			m_activeFlag = true;
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
		if (draw != anime) {
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
	//エフェクトを止める
	void Enemy::EffectStop(int num) {
		if (MAX_EFFECT_NUM <= num) return;
		if (m_EfkPlayer[num - 1]) {
			m_EfkPlayer[num - 1]->StopEffect();
		}
	}
	//敵の目の場所を設定
	Vec3 Enemy::GetEyePos(const Vec3& eye) {
		Vec3 pos = GetWorldPos();
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
		auto pos = m_trans->GetPosition();
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss(L"");
		wss << L"\nfps : "
			<< fps
			<< L"\npos : "
			<< L"x "
			<< pos.x
			<< L"\n y "
			<< pos.y
			<< L"\n z "
			<< pos.z
			<< L"\nfloor : "
			<< m_floorFlag
			<< L"\nActive"
			<< m_activeFlag
			<< endl;
		scene->SetDebugString(wss.str());

	}

	//重力
	Vec3 Enemy::Grav() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto pos = m_trans->GetPosition();
		m_gravVel += m_grav * elapsed;
		pos += m_gravVel * elapsed;
		if (m_gravVel.y < m_grav.y) {
			m_gravVel.y = m_grav.y;
		}
		return pos;
	}
	void Enemy::GravZero() {
		m_grav = Vec3(0.0f);
	}
	void Enemy::SetGrav(Vec3 grav) {
		m_grav = grav;
	}

	//ゲッターセッター
	float Enemy::GetAngle() {
		return m_angle;
	}
	Vec3 Enemy::GetDirec() {
		return m_direc;
	}
	Vec3 Enemy::GetPos() {
		return m_trans->GetPosition();

	}
	void Enemy::SetSpeed(float speed) {
		m_speed = speed;
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
	Vec3 Enemy::GetWorldPos() {
		return m_trans->GetWorldPosition();
	}
	bool Enemy::GetOverHeat() {
		return m_overHeatFlag;
	}
	void Enemy::SetPlungeFlag(bool flag) {
		m_plungeFlag = flag;
	}
	void Enemy::SetBulletDirec(Vec2 direc) {
		m_bulletDic = direc;
	}
	bool Enemy::GetActiveFlag() {
		return m_activeFlag;
	}
	void Enemy::SetActiveFlag(bool flag) {
		m_activeFlag = flag;
	}
	void Enemy::PlayOverHeat() {
		m_heat = m_maxHeat;
	}
	void Enemy::SetThrowFlag(bool flag) {
		m_throwFlag = flag;
	}
	void Enemy::SetThorwLenght(float lenght) {
		m_throwLength = lenght;
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
		m_range(70.0f),
		m_pos(Vec3(0.0f)),
		m_scal(Vec3(1.0f)),
		m_height(0.0f)

	{}
	StraightBullet::StraightBullet(const shared_ptr<Stage>& stage,
		const shared_ptr<Enemy>& enemy,
		const float height
	) :
		EnemyBullet(stage),
		m_enemy(enemy),
		m_speed(30.0f),
		m_range(70.0f),
		m_pos(Vec3(0.0f)),
		m_scal(Vec3(1.0f)),
		m_height(height)
	{}

	void StraightBullet::OnCreate() {
		EnemyBullet::OnCreate();
		m_trans = GetComponent<Transform>();
		m_trans->SetRotation(Vec3(0.0f,0.0f, 0.0f));
		m_trans->SetScale(m_scal);

		auto enemy = m_enemy.lock();
		if (!enemy) return;
		m_enemyPos = enemy->GetWorldPos();
		m_trans->SetPosition(Vec3(m_enemyPos.x, m_enemyPos.y + m_height, m_enemyPos.z));

	}
	void StraightBullet::OnUpdate() {
		EnemyBullet::OnUpdate();
		float elapsed = App::GetApp()->GetElapsedTime();
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		auto enemyFor= enemy->GetComponent<Transform>()->GetForward();
		m_pos = m_trans->GetPosition();
		//m_pos.y = m_enemyPos.y;
		m_pos += enemyFor *m_speed * elapsed;
		Vec3 pos = m_enemyPos - m_pos;
		if (pos.length() >= m_range) {
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
		m_enemyPos = enemy->GetWorldPos();

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

	//プレイヤーをめがけて跳ぶ弾
	TrackingBullet::TrackingBullet(const shared_ptr<Stage>& stage,
		const shared_ptr<Enemy>& enemy,
		const shared_ptr<Player>& player
	) :
		EnemyBullet(stage),
		m_enemy(enemy),
		m_player(player),
		m_speed(0.7f),
		m_range(50.0f),
		m_pos(Vec3(0.0f)),
		m_enemyPos(Vec3(0.0f))
	{};
	void TrackingBullet::OnCreate() {
		EnemyBullet::OnCreate();
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		m_enemyPos = enemy->GetWorldPos();
		auto player = m_player.lock();
		if (!player) return;
		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();

		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(Vec3(m_enemyPos.x, m_enemyPos.y + 5.0f, m_enemyPos.z));

		Vec3 dic = playerPos - m_enemyPos;
		m_dic = Vec3(dic.x, dic.y - 5.0f, dic.z);
	}
	void TrackingBullet::OnUpdate() {
		EnemyBullet::OnUpdate();
		float elapsed = App::GetApp()->GetElapsedTime();

		auto enemy = m_enemy.lock();
		if (!enemy) return;
		m_pos = m_trans->GetPosition();
		m_pos += m_dic * m_speed * elapsed;

		Vec3 pos = m_enemyPos - m_pos;
		m_trans->SetPosition(m_pos);

		if (pos.length() >= m_range) {
			ThisDestroy();
		}
	}

	//敵の上に付ける足場コリジョン
	EnemyFloorCol::EnemyFloorCol(const shared_ptr<Stage>& stage,
		const shared_ptr<Enemy>& enemy
	):
		GameObject(stage),
		m_enemy(enemy),
		m_posHight(0.0f),
		m_plusScale(2.5f),
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
		m_trans->SetScale(Vec3(m_enemyScal.x * m_plusScale, m_enemyScal.y / 5, m_enemyScal.z * m_plusScale));
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
			m_trans->SetPosition(Vec3(0.0f, m_enemyScal.y / 2.0f + m_posHight, 0.0f));
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
	void EnemyFloorCol::SetPosHight(float hight) {
		m_posHight = hight;
	}
}
