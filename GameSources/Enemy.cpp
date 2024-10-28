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
		const State& deathState,
		const shared_ptr<Player>& player,
		const shared_ptr<FixedBox>& box
	) :
		GameObject(stage),
		m_pos(position),
		m_rot(rotatoin),
		m_scal(scale),
		m_stateType(state),
		m_deathState(deathState),
		m_player(player),
		m_box(box),
		m_hp(100),
		m_maxHp(100),
		m_speed(0.3f),
		m_upSpeed(0.3f),
		m_upHeight(0.25f),
		m_jumpPower(1.0f),
		m_jumpTime(1.0f),
		m_dic(-1),
		m_time(3.0f),
		m_bulletTime(0.0f),
		m_bulletRange(1.0f),
		m_bulletFlag(false),
		m_jumpFlag(false),
		m_flyFlag(false)
	{}

	void Enemy::OnCreate() {
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scal);
		m_beforState = m_stateType;

		m_playerScale = m_player->GetScale();
		//描画
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//衝突判定
		m_collision = AddComponent<CollisionObb>();
		m_collision->SetFixed(false);
		m_collision->SetDrawActive(true);
		//影
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		//重力
		if (m_stateType == stay || m_stateType == rightMove) {
			m_gravity = AddComponent<Gravity>();
		}

		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

		AddTag(L"Enemy");
	}

	void Enemy::OnUpdate() {

		auto& keyState = App::GetApp()->GetInputDevice().GetKeyState();
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto stage = GetStage();
		m_pos = m_trans->GetPosition();
		m_playerTrans = m_player->GetComponent<Transform>();
		Vec3 playerPos = m_playerTrans->GetPosition();
		auto playerScale = m_playerTrans->GetScale();

		Vec3 playerDis = playerPos - m_pos;
/*------------------テスト用-------------------------*/
		//キー操作で移動
		if (keyState.m_bLastKeyTbl[VK_RIGHT]) {
			m_pos.x += m_dic * m_speed * elapsed;
		}
		if (keyState.m_bLastKeyTbl[VK_LEFT]) {
			m_pos.x += -m_dic * m_speed * elapsed;
		}
		//スペースでジャンプ（テスト用）
		if (keyState.m_bLastKeyTbl[VK_DOWN]) {
			EnemyJump();

		}
		if (keyState.m_bPressedKeyTbl[VK_UP]) {
			stage->AddGameObject<EnemyBullet>(GetThis<Enemy>());
		}
		if (keyState.m_bPushKeyTbl['W']) {
			m_stateType = stay;
		}
/*------------------テスト用-------------------------*/

		//足場から落下防止
		if (m_box != nullptr) {
			auto boxPos = m_box->GetPositoin();
			auto boxScal = m_box->GetScale();
			if (m_pos.x > boxPos.x + boxScal.x / 2 - m_scal.x / 2-0.2f) {
				m_speed = 0.0f;
				if (m_pos.x > playerPos.x) {
					m_speed = 0.3f;
				}
			}
			else if (m_pos.x < boxPos.x - boxScal.x / 2 + m_scal.x / 2+0.2f) {
				m_speed = 0;
				if (m_pos.x < playerPos.x) {
					m_speed = 0.3f;
				}
			}
			else {
				m_speed = 0.3f;
			}
		}


		//行動パターン
		switch (m_stateType)
		{
		//移動なし
		case stay:
			m_dic = 0;
			if (m_gravity) {
				m_gravity->SetGravity(Vec3(0.0f,-9.8/5,0.0f));
			}
			else {
				m_gravity = AddComponent<Gravity>();
			}

			break;

		//追従の左右移動
		case rightMove:
			if (m_pos.x < playerPos.x - (m_scal.x / 2 + m_playerScale.x / 2 + 0.01f)) {
				m_dic = -1;
			}
			else if (m_pos.x > playerPos.x + (m_scal.x / 2 + m_playerScale.x / 2 + 0.01f)) {
				m_dic = 1;
			}
			else {
				m_dic = 0;
			}
			m_pos.x += -m_dic * m_speed * elapsed;
			break;
		//上下に移動
		case upMove:
			if (m_pos.y > 0.5f) {
				m_dic = -1;
			}
			else if (m_pos.y < -0.25f) {
				m_dic = 1;
			}
			m_pos.y += m_dic * m_upSpeed * elapsed;
			break;
		//追従浮遊
		case flyMove:
			if (!m_jumpFlag) {
				EnemyJump();
				m_jumpFlag = true;
			}
			if (m_pos.y <= 0.4f) {
				m_jumpFlag = false;
			}
			if (m_pos.x < playerPos.x - (m_scal.x / 2 + 0.051)) {
				m_dic = -1;
			}
			else if (m_pos.x > playerPos.x + (m_scal.x / 2 + 0.051)) {
				m_dic = 1;
			}
			else {
				m_dic = 0;
			}
			m_pos.x += -m_dic * m_speed * elapsed;
			break;
		//浮遊
		case fly:
			if (!m_jumpFlag) {
				EnemyJump();
				m_jumpFlag = true;
			}
			if (m_pos.y <= 0.4f) {
				m_jumpFlag = false;
			}
			break;
		//プレイヤーに触れた位置で固定
		case fixedStay:
			m_pos = m_deathPos;
			m_collision->SetFixed(true);
			if (m_gravity) {
				m_gravity->SetGravityVerocityZero();
			}
			AddTag(L"FixedBox");
			break;
		//プレイヤーと逆方向に移動
		case runaway:
			if (m_pos.x < playerPos.x) {
				m_dic = -1;
			}
			else {
				m_dic = 1;
			}
			m_pos.x += m_dic * m_speed * 5.0f * elapsed;
			break;

		default:
			break;
		}
		m_trans->SetPosition(m_pos);


		//弾の生成
		if (m_pos.x > playerPos.x - m_bulletRange) {
			m_bulletTime += elapsed;
			if (!m_bulletFlag && m_dic != 0) {
				stage->AddGameObject<EnemyBullet>(GetThis<Enemy>());
				m_bulletFlag = true;
				m_bulletTime = 0.0f;
			}
		}
		if (m_bulletTime >= 2.0f) {
			m_bulletFlag = false;
		}

		//浮遊
		//if (m_flyFlag) {
		//	m_jumpTime -= elapsed;
		//	if (!m_jumpFlag) {
		//		EnemyJump();

		//		m_jumpFlag = true;
		//		m_jumpTime = 1.0f;
		//	
		//	}
		//	if (m_jumpTime <= 0.0f) {
		//		m_jumpFlag = false;
		//	}
		//}



		//敵の倒された時
		if (m_hp <= 0.0f) {
			m_stateType = m_deathState;
			//if (m_stateType == rightMove) {
			//}

			//ThisDestroy();
		}

		//デバック用
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss(L"");
		wss << L"damage : " 
			<<m_hp
			<<L"\ndic : "
			<<m_dic
			<<L"\nfps : "
			<<fps
			<<L"\nstate : "
			<<m_stateType
			<<L"\npos : "
			<<m_pos.x
			<< endl;
		scene->SetDebugString(wss.str());
	}

	//衝突判定
	void Enemy::OnCollisionEnter(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			m_deathPos = m_pos;
			ReceiveDamage(100.0f);
		}
		if (other->FindTag(L"BreakWall")) {
			auto breakWall = dynamic_pointer_cast<BreakWall>(other);
			if (m_stateType == runaway) {
				breakWall->ThisDestory();
			}
		}

	}
	//ダメージを受ける
	void Enemy::ReceiveDamage(float damage) {
		m_hp -= damage;
	}

	//自分の削除
	void Enemy::ThisDestroy() {
		GetStage()->RemoveGameObject<Enemy>(GetThis<Enemy>());
	}
	
	void Enemy::EnemyJump() {
		m_gravity = AddComponent<Gravity>();
		m_gravity->SetGravity(Vec3(0.0f, -9.8f/5, 0.0f));
		auto& keyState = App::GetApp()->GetInputDevice().GetKeyState();
		m_gravity->StartJump(Vec3(0, m_jumpPower, 0));
	}
	int Enemy::GetDic() {
		return m_dic;
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


	//--------------------------------------------------------------------------------------
	//	class EnemyBullet : public GameObject;  
	//--------------------------------------------------------------------------------------
	EnemyBullet::EnemyBullet(const shared_ptr<Stage>& stage, 
		const shared_ptr<Enemy>& enemy):
		GameObject(stage),
		m_enemy(enemy),
		m_pos(Vec3(0, 0, 0)),
		m_rot(Vec3(0,0,0)),
		m_scal(Vec3(0.02f,0.02f,0.02f)),
		m_speed(1.0f),
		m_Range(1.0f),
		m_dic(1)
	{}
	void EnemyBullet::OnCreate() {
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_enemy->GetPos());
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scal);
		m_dic = m_enemy->GetDic();
		m_enemyPos = m_enemy->GetComponent<Transform>()->GetPosition();
		
		//描画
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");

		//衝突判定
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetFixed(false); //空間に固定するか
		ptrColl->SetDrawActive(true);
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
		m_pos.x += -m_dic * m_speed * elapsed;
		m_trans->SetPosition(m_pos);

		if (m_dic == -1 && m_enemyPos.x + m_Range <= m_pos.x) {
			ThisDestroy();
		}
		if (m_dic == 1 && m_enemyPos.x - m_Range >= m_pos.x) {
			ThisDestroy();
		}
	}

	void EnemyBullet::OnCollisionEnter(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			ThisDestroy();
		}
	}
	void EnemyBullet::ThisDestroy() {
		GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
	}

	//struct MyGravity::Impl{
	//	bsm::Vec3 m_Gravity;
	//	bsm::Vec3 m_GravityVelocity;
	//	Impl():
	//		m_Gravity(0),
	//		m_GravityVelocity(0)
	//	{}
	//	~Impl() {}
	//};

	MyGravity::MyGravity(const shared_ptr<GameObject>& GameObjectPtr,
		const bsm::Vec3& gravity
	):
		Component(GameObjectPtr)
	{
		m_gravity = gravity;
	}

	Vec3 MyGravity::GetGravity() const{
		return m_gravity;
	}
	void MyGravity::SetGravity(const bsm::Vec3& gravity) {
		m_gravity = gravity;
	}
	void MyGravity::SetGravityZero() {
		m_gravity = Vec3(0.0f);
	}
	void MyGravity::OnUpdate() {
		auto ptrCollision = GetGameObject()->GetComponent<Collision>(false);
		if (ptrCollision && ptrCollision->IsSleep()) {
			return;
		}

		auto transform = GetGameObject()->GetComponent<Transform>();
		auto pos = transform->GetPosition();
		float elapsed = App::GetApp()->GetElapsedTime();
		m_gravity += m_gravity * elapsed;
		pos += m_gravity * elapsed;
		transform->SetPosition(pos);

	}
}
