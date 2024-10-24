/*!
@file Enemy.h
@brief �G�Ȃ�
�S���F�팩
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject
	{
		enum State {
			stay,
			rightMove,
			upMove,
			fly,
		};

		float m_hp;          //�̗�
		float m_maxHp;       //�ő�̗�
		float m_speed;       //�ړ����x
		float m_upSpeed;     //�㉺�̑��x
		float m_jumpPower;   //�W�����v��
		float m_jumpTime;   //�W�����v���x
		float m_time;
		float m_bulletTime;  //�e�̔��ˊԊu
		float m_bulletRange; //�e�𔭎˂��鋗��
		float m_playerScale;
		int m_dic;           //�����Ă������ ��-1,�E1
		bool m_bulletFlag;
		bool m_jumpFlag;
		bool m_flyFlag;

		State m_stateType;

		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;

		shared_ptr<Transform> m_trans;
		shared_ptr<Player> m_player;
		shared_ptr<Transform> m_playerTrans;
		weak_ptr<Gravity> m_gravity;

	public:
		// �\�z�Ɣj��
		Enemy(const shared_ptr<Stage>& stage,const shared_ptr<Player>& player);
		Enemy(const shared_ptr<Stage>& stage, 
			const Vec3& position, 
			const Vec3& rotatoin, 
			const Vec3& scale);
		virtual ~Enemy(){}
		virtual void OnCreate() override; // ������
		virtual void OnUpdate() override; // �X�V

		void OnCollisionEnter(shared_ptr<GameObject>& other);
		void ReceiveDamage(float damage);
		void ThisDestroy();
		void EnemyJump();
		void SetEnemyFlayFlag(bool flag);
		void SetSpeed(float speed);
		void SetUpSpeed(float speed);
		void SetflyPower(float power);
		void SetState(State state);

		int GetDic();
		Vec3 GetPos();
	};


	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;
	//--------------------------------------------------------------------------------------
	class EnemyBullet : public GameObject {
	private:
		
		float m_speed;   //�e�̑���
		float m_Range;   //�˒�
		int m_dic;       //�e�̌��� ��-1,�E1
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
		Vec3 m_enemyPos;

		shared_ptr<Transform> m_trans;
		shared_ptr<Enemy> m_enemy;

	public :
		EnemyBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy);
		virtual ~EnemyBullet(){}
		virtual void OnCreate();
		virtual void OnUpdate();
		void OnCollisionEnter(shared_ptr<GameObject>& other);

		void ThisDestroy();

	};

	class MyGravity : public Component {
		explicit MyGravity(const shared_ptr<GameObject>& GameObjectPtr,
			const bsm::Vec3& gravity = bsm::Vec3(0, -9.8f, 0));

	};

}