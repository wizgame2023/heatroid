/*!
@file Enemy.cpp
@brief 敵など実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//初期化
	Enemy::Enemy(const shared_ptr<Stage>& stage,const shared_ptr<Player>& player) :
		GameObject(stage),
		m_player(player),
		m_pos(Vec3(-10.0f, -3.0, 0.0f)),
		m_rot(Vec3(0.0f, 0.0f, 0.0f)),
		m_scal(Vec3(1.0f, 1.0f, 1.0f)),
		m_hp(100),
		m_maxHp(100),
		m_speed(2.0f),
		m_jumpPower(5.0f),
		m_dic(-1),
		m_time(3.0f)
	{}

	Enemy::Enemy(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotatoin,
		const Vec3& scale
	) :
		GameObject(stage),
		m_pos(position),
		m_rot(rotatoin),
		m_scal(scale),
		m_hp(100),
		m_maxHp(100),
		m_speed(0.5f),
		m_jumpPower(5.0f),
		m_dic(-1),
		m_time(3.0f)
	{}

	void Enemy::OnCreate() {
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scal);

		//描画
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//衝突判定
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(false);
		ptrColl->SetDrawActive(true);
		//影
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		//重力
		auto ptrGra = AddComponent<Gravity>();

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
/*------------------テスト用-------------------------*/

		//敵のいる方向に進む
		if (m_pos.x < playerPos.x - 1.05f) {
			m_dic = -1;
		}
		else if(m_pos.x > playerPos.x + 1.05f){
			m_dic = 1;
		}
		else {
			m_dic = 0.0f;
		}

		if (m_pos.x > playerPos.x - 3.0f) {
			
		}


		m_pos.x += -m_dic * m_speed * elapsed;
		m_trans->SetPosition(m_pos);

		//敵の倒された時
		if (m_hp <= 0.0f) {
			ThisDestroy();
		}

		//デバック用
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss(L"");
		wss << L"damage : " 
			<<m_hp
			<<L"\ndic : "
			<<m_dic
			<< endl;
		scene->SetDebugString(wss.str());
	}

	//衝突判定
	void Enemy::OnCollisionEnter(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			ReceiveDamage(0.0f);
		}
	}
	void Enemy::ReceiveDamage(float damage) {
		m_hp -= damage;
	}

	//自分の削除
	void Enemy::ThisDestroy() {
		GetStage()->RemoveGameObject<Enemy>(GetThis<Enemy>());
	}

	void Enemy::EnemyJump() {
		auto grav = GetComponent<Gravity>();
		auto& keyState = App::GetApp()->GetInputDevice().GetKeyState();
		grav->StartJump(Vec3(0, m_jumpPower, 0));
	}
	int Enemy::GetDic() {
		return m_dic;
	}
	Vec3 Enemy::GetPos() {
		return m_pos;
	}

	EnemyBullet::EnemyBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy):
		GameObject(stage),
		m_enemy(enemy),
		m_pos(Vec3(0, 0, 0)),
		m_rot(Vec3(0,0,0)),
		m_scal(Vec3(0.2f,0.2f,0.2f)),
		m_speed(10.0f),
		m_dic(-1)
	{}
	void EnemyBullet::OnCreate() {
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_enemy->GetPos());
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scal);
		m_dic = m_enemy->GetDic();
		
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
	}

	void EnemyBullet::OnCollisionEnter(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			//ThisDestroy();
		}
	}
	void EnemyBullet::ThisDestroy() {
		GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
	}
}
