/*!
@file Enemy.cpp
@brief �G�Ȃǎ���
�S���F�팩
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;  //�G
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
		m_stateType(flyMove),
		m_deathState(stay),
		m_player(player),
		m_box(box),
		m_hp(100),
		m_maxHp(100),
		m_speed(0.3f),
		m_upSpeed(0.3f),
		m_upHeight(0.1f),
		m_jumpPower(0.5f),
		m_jumpTime(1.0f),
		m_dic(-1),
		m_dicUp(0),
		m_firstDic(0),
		m_time(2.0f),
		m_bulletTime(0.0f),
		m_bulletRange(1.0f),
		m_gravity(-0.98f),
		m_grav(Vec3(0.0f, m_gravity, 0.0f)),
		m_gravVel(Vec3(0.0f)),
		m_angle(Vec3(0.0f)),
		m_angleSpeed(0.3f),
		m_bulletFlag(false),
		m_jumpFlag(false),
		m_flyFlag(false),
		m_floorFlag(false),
		m_hitDropFlag(false),
		m_plungeFlag(false),
		m_plungeColFlag(false)
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
		//�`��
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//�Փ˔���
		m_collision = AddComponent<CollisionObb>();
		m_collision->SetFixed(false);
		m_collision->SetDrawActive(true);
		//�e
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");

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
		//�v���C���[�����擾
		auto player = m_player.lock();
		if (!player) return;
		m_playerTrans = m_player.lock()->GetComponent<Transform>();
		m_playerPos = m_playerTrans.lock()->GetPosition();

		/*------------------�e�X�g�p-------------------------*/
				//�L�[����ňړ�
		if (keyState.m_bLastKeyTbl[VK_RIGHT]) {
			m_pos.x += m_dic * m_speed * elapsed;
		}
		if (keyState.m_bLastKeyTbl[VK_LEFT]) {
			m_pos.x += -m_dic * m_speed * elapsed;
		}
		//�X�y�[�X�ŃW�����v�i�e�X�g�p�j
		if (keyState.m_bPressedKeyTbl[VK_DOWN]) {
			//EnemyJump();
			HipDropJump();
		}
		if (keyState.m_bPressedKeyTbl[VK_UP]) {
			//EnemyJump();
			stage->AddGameObject<EnemyBullet>(GetThis<Enemy>());
		}
		if (keyState.m_bPushKeyTbl['W']) {
			//m_stateType = stay;
		}
		/*------------------�e�X�g�p-------------------------*/

		FindFixed();

		//�s���p�^�[��
		switch (m_stateType)
		{
			//�ړ��Ȃ�
		case stay:
			if (!m_hitDropFlag) {
				SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			}
			m_dic = 0;
			break;
			//�Ǐ]�̍��E�ړ�
		case rightMove:
			PlayerDic();
			break;
			//�㉺�Ɉړ�
		case upMove:
			GravZero();
			Grav();
			if (m_pos.y >= m_beforePos.y) {
				m_dicUp = -1;
			}
			else if (m_pos.y <= m_upHeight) {
				m_dicUp = 1;
			}
			m_dic *= -1;
			m_pos.y += m_dicUp * m_upSpeed * elapsed;
			break;
			//�Ǐ]���V
		case flyMove:
			OneJump(0.1f);
			PlayerDic(false);
			break;
			//���V
		case fly:
			OneJump(0.2f);
			break;
			//�v���C���[�ɐG�ꂽ�ʒu�ŌŒ�
		case fixedStay:
			GravZero();
			Grav();
			m_pos = m_deathPos;
			m_collision->SetFixed(true);
			AddTag(L"FixedBox");
			break;
			//�v���C���[�Ƌt�����Ɉړ�
		case runaway:
			PlayerDic(false, -5.0f);
			break;
			//�q�b�v�h���b�v
		case hitDrop:
			break;
			//�˂�����
		case plunge:
			//PlayerDic(true,3.0f);
			if (m_pos.x < m_playerPos.x - (m_scal.x / 2 + m_playerScale.x / 2)) {
				m_dic = -1;
			}
			else if (m_pos.x > m_playerPos.x + (m_scal.x / 2 + m_playerScale.x / 2)) {
				m_dic = 1;
			}
			else {
				m_dic = 0;
			}
			if (!m_plungeFlag && !m_plungeColFlag) {
				m_firstDic = m_dic;
				m_plungeFlag = true;
			}

			//if (m_dic == 0) {
			//	m_plungeFlag = false;
			//}
			if (m_plungeColFlag) {
				m_firstDic *= -1;
				m_plungeColFlag = false;
			}
			m_pos.x += -m_firstDic * m_speed * 3.0f * elapsed;
			break;
		default:
			break;
		}
		//�d�͂̏���
		if (!m_floorFlag) {
			Grav();
		}
		if (m_floorFlag) {
			GravZero();
			Grav();
		}
		HitDrop();
		Bullet();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_angle);



		//�G�̓|���ꂽ��
		if (m_hp <= 0.0f) {
			m_stateType = m_deathState;
			//ThisDestroy();
		}

		//Debug();
	}

	//�Փ˔���
	//�W�����v
	void Enemy::EnemyJump() {
		m_floorFlag = false;
		AddGrav(Vec3(0.0f, m_jumpPower, 0.0f));
		SetGrav(Vec3(0.0f, -0.98f, 0.0f));
		Grav();
	}
	void Enemy::HipDropJump() {
		m_hitDropFlag = true;
		EnemyJump();
		GravZero();
	}
	//�����̍폜
	void Enemy::ThisDestroy() {
		GetStage()->RemoveGameObject<Enemy>(GetThis<Enemy>());
	}
	//�_���[�W���󂯂�
	void Enemy::ReceiveDamage(float damage) {
		m_hp -= damage;
	}
	//�v���C���[�̕����擾�A���̕����ɉ�]
	void Enemy::PlayerDic(bool zero, float addSpeed) {
		auto elapsed = App::GetApp()->GetElapsedTime();
		if (m_pos.x < m_playerPos.x - (m_scal.x / 2 + m_playerScale.x / 2)) {
			if (m_angleSpeed <= XMConvertToRadians(45)) {
				m_angleSpeed += XMConvertToRadians(1) * 360.0f * elapsed;
			}
			m_angle = Vec3(0.0f, m_angleSpeed, 0.0f);
			m_dic = -1;
		}
		else if (m_pos.x > m_playerPos.x + (m_scal.x / 2 + m_playerScale.x / 2)) {
			if (m_angleSpeed >= XMConvertToRadians(-45)) {
				m_angleSpeed += -XMConvertToRadians(1) * 360.0f * elapsed;
			}
			m_angle = Vec3(0.0f, m_angleSpeed, 0.0f);

			m_dic = 1;
		}
		else {
			if (zero) {
				m_dic = 0;
			}
		}
		m_pos.x += -m_dic * m_speed * addSpeed * elapsed;

	}
	//�W�����v
	void Enemy::OneJump(float jumpHight) {
		if (!m_jumpFlag) {
			EnemyJump();
			m_jumpFlag = true;
		}
		if (m_pos.y <= jumpHight) {
			m_jumpFlag = false;
		}

	}
	//�q�b�v�h���b�v
	void Enemy::HitDrop() {
		auto fixed = m_fixedBox.lock();
		auto elapsed = App::GetApp()->GetElapsedTime();
		//�q�b�v�h���b�v��
		if (m_hitDropFlag) {
			if (fixed) {
				Vec3 fixPos = m_fixedBox.lock()->GetComponent<Transform>()->GetPosition();
				Vec3 fixScal = m_fixedBox.lock()->GetComponent<Transform>()->GetScale();
				//���ɏՓ˂�����
				if (m_floorFlag) {
					m_time = 2.0f;
					m_hitDropFlag = false;
					AddGrav(Vec3(0.0f, -m_jumpPower, 0.0f));
				}
				m_time -= elapsed;
				if (m_time <= 0) {
					if (m_pos.y - m_scal.y / 2 + fixPos.y >= fixPos.y + fixScal.y / 2 + 0.01f) {
						m_pos.y += -m_speed * 15 * elapsed;
					}
				}
			}
		}

	}
	//���ɐG�ꂽ��擾
	void Enemy::FindFixed() {
		auto fixed = m_fixedBox.lock();
		if (fixed) {
			Vec3 fixPos = m_fixedBox.lock()->GetComponent<Transform>()->GetPosition();
			Vec3 fixScal = m_fixedBox.lock()->GetComponent<Transform>()->GetScale();
			if (m_pos.y - m_scal.y / 2 + fixPos.y <= fixPos.y + fixScal.y / 2 + 0.001f) {
				m_floorFlag = true;
			}
			if (m_pos.x >= fixPos.x + fixScal.x / 2 + m_scal.x / 2 ||
				m_pos.x <= fixPos.x - fixScal.x / 2 - m_scal.x / 2) {
				m_floorFlag = false;
			}

			//�����痎���Ȃ��悤�ɂ���
			if (m_pos.x > fixPos.x + fixScal.x / 2 - m_scal.x / 2 - 0.2f) {
				m_speed = 0.0f;
				if (m_pos.x > m_playerPos.x - fixPos.x) {
					m_speed = 0.3f;
				}
			}
			else if (m_pos.x < fixPos.x - fixScal.x / 2 + m_scal.x / 2 + 0.2f) {
				m_speed = 0;
				if (m_pos.x < m_playerPos.x - fixPos.x) {
					m_speed = 0.3f;
				}
			}
			else {
				m_speed = 0.3f;
			}
		}
		else {
			m_floorFlag = false;
		}

	}
	//�e�̔���
	void Enemy::Bullet() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto stage = GetStage();
		auto fixed = m_fixedBox.lock();

		//�e�̐���
		if (m_pos.x > m_playerPos.x - m_bulletRange) {
			m_bulletTime += elapsed;
			if (!m_bulletFlag && m_dic != 0) {
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
		}
		if (m_bulletTime >= 2.0f) {
			m_bulletFlag = false;
		}

	}
	//�Փ˔���
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
			m_plungeColFlag = true;
		}
		if (other->FindTag(L"FixedBox")) {
			m_fixedBox = dynamic_pointer_cast<FixedBox>(other);
			m_trans->SetParent(m_fixedBox.lock());
			m_pos = m_pos - m_fixedBox.lock()->GetComponent<Transform>()->GetPosition();
		}

	}
	//�f�o�b�N
	void Enemy::Debug() {
		//�f�o�b�N�p
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss(L"");
		wss << L"damage : "
			<< m_hp
			<< L"\ndic : "
			<< m_dic
			<< L"\nfps : "
			<< fps
			<< L"\nstate : "
			<< m_stateType
			<< L"\npos : "
			<< m_pos.y
			<< L"\nfloor : "
			<< m_floorFlag
			<< L"\nhipDrop : "
			<< m_hitDropFlag
			<< L"\ntest : "
			<< m_test
			<< endl;
		scene->SetDebugString(wss.str());

	}
	//�d��
	void Enemy::Grav() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		m_gravVel += m_grav * elapsed;
		m_pos += m_gravVel * elapsed;
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

	//�Q�b�^�[�Z�b�^�[
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
	bool Enemy::GetFloorFlag() {
		return m_floorFlag;
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
		m_scal(Vec3(0.02f, 0.02f, 0.02f)),
		m_speed(1.0f),
		m_Range(1.0f),
		m_floorCheck(false),
		m_dic(1)
	{}
	void EnemyBullet::OnCreate() {
		m_trans = GetComponent<Transform>();
		m_enemyPos = m_enemy->GetComponent<Transform>()->GetPosition();

		if (m_fixedPos!=Vec3(0.0f)) {
			Vec3 pos = m_enemy->GetPos() + m_fixedPos;
			Vec3 enemyScale = m_enemy->GetComponent<Transform>()->GetScale();
			m_trans->SetPosition(Vec3(pos.x, pos.y + enemyScale.y / 2, m_pos.z));
		}
		m_trans->SetPosition(m_enemyPos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scal);
		m_dic = m_enemy->GetDic();
		m_beforFlag = m_enemy->GetFloorFlag();

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
		m_enemyPos = m_enemy->GetComponent<Transform>()->GetPosition();

		if (m_beforFlag != m_enemy->GetFloorFlag()) {
			m_floorCheck = false;
		}
		if (!m_enemy->GetFloorFlag()) {
			if (!m_floorCheck) {
				Vec3 pos = m_enemy->GetPos() + m_fixedPos;
				m_trans->SetPosition(pos);
				m_beforFlag = m_enemy->GetFloorFlag();
				m_floorCheck = true;
			}
		}
		if (m_enemy->GetFloorFlag()) {
			if (!m_floorCheck) {
				Vec3 pos = m_enemy->GetPos() + m_fixedPos;
				Vec3 enemyScale = m_enemy->GetComponent<Transform>()->GetScale();
				m_trans->SetPosition(Vec3(pos.x, pos.y + enemyScale.y / 2, m_pos.z));
				m_beforFlag = m_enemy->GetFloorFlag();
				m_floorCheck = true;
			}
		}

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

		Debug();
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
