/*!
@file Enemy.cpp
@brief �G�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//-----     �G     -----
	Enemy::Enemy(const shared_ptr<Stage>& stage, const shared_ptr<Player>& player) :
		GameObject(stage),
		m_player(player),
		m_stateType(rightMove),
		m_pos(Vec3(-1.0f, 0.4f, 0.0f)),
		m_rot(Vec3(0.0f, 0.0f, 0.0f)),
		m_scal(Vec3(0.1f, 0.1f, 0.1f)),
		m_hp(100),
		m_maxHp(100),
		m_speed(0.3f),
		m_upSpeed(0.3f),
		m_jumpPower(1.0f),
		m_jumpTime(1.0f),
		m_dic(-1),
		m_time(3.0f),
		m_bulletTime(0.0f),
		m_bulletRange(1.0f),
		m_bulletFlag(false),
		m_jumpFlag(false),
		m_flyFlag(true)
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
		m_speed(0.0f),
		m_upSpeed(0.3f),
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

		m_playerScale = m_player->GetCollisionScale();
		//�`��
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//�Փ˔���
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(false);
		ptrColl->SetDrawActive(true);
		//�e
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		//�d��
		if (m_stateType == stay || m_stateType == rightMove) {
			m_gravity.lock() = AddComponent<Gravity>();
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
/*------------------�e�X�g�p-------------------------*/
		//�L�[����ňړ�
		if (keyState.m_bLastKeyTbl[VK_RIGHT]) {
			m_pos.x += m_dic * m_speed * elapsed;
		}
		if (keyState.m_bLastKeyTbl[VK_LEFT]) {
			m_pos.x += -m_dic * m_speed * elapsed;
		}
		//�X�y�[�X�ŃW�����v�i�e�X�g�p�j
		if (keyState.m_bLastKeyTbl[VK_DOWN]) {
			EnemyJump();

		}
		if (keyState.m_bPressedKeyTbl[VK_UP]) {
			stage->AddGameObject<EnemyBullet>(GetThis<Enemy>());
		}
/*------------------�e�X�g�p-------------------------*/

		//�v���C���[�̂�������ɍ��E�ړ�
		if (m_stateType == rightMove) {
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
		}

		//�㉺�Ɉړ�
		if (m_stateType == upMove) {
			if (m_pos.y > 0.5f) {
				m_dic = -1;
			}
			else if(m_pos.y < -0.25f) {
				m_dic = 1;
			}
			m_pos.y += m_dic * m_upSpeed * elapsed;
		}

		//���V
		if (m_stateType == fly) {
			if (!m_jumpFlag) {
				EnemyJump();
				m_jumpFlag = true;
			}
			if (m_pos.y <= 0.4f) {
				m_jumpFlag = false;
			}
		}


		//�e�̐���
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

		//���V
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


		m_trans->SetPosition(m_pos);

		//�G�̓|���ꂽ��
		if (m_hp <= 0.0f) {
			ThisDestroy();
		}

		//�f�o�b�N�p
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss(L"");
		wss << L"damage : " 
			<<m_hp
			<<L"\ndic : "
			<<m_dic
			<<L"\nfps : "
			<<fps
			<< endl;
		scene->SetDebugString(wss.str());
	}

	//�Փ˔���
	void Enemy::OnCollisionEnter(shared_ptr<GameObject>& other) {
		if (other->FindTag(L"Player")) {
			ReceiveDamage(0.0f);
		}
	}
	//�_���[�W���󂯂�
	void Enemy::ReceiveDamage(float damage) {
		m_hp -= damage;
	}

	//�����̍폜
	void Enemy::ThisDestroy() {
		GetStage()->RemoveGameObject<Enemy>(GetThis<Enemy>());
	}

	void Enemy::EnemyJump() {
		m_gravity = AddComponent<Gravity>();
		m_gravity.lock()->SetGravity(Vec3(0.0f, -9.8f/5, 0.0f));
		auto& keyState = App::GetApp()->GetInputDevice().GetKeyState();
		m_gravity.lock()->StartJump(Vec3(0, m_jumpPower, 0));
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

	//-----     �G�̒e     -----
	EnemyBullet::EnemyBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy):
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
		
		//�`��
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");

		//�Փ˔���
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetFixed(false); //��ԂɌŒ肷�邩
		ptrColl->SetDrawActive(true);
		//�e
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

}
