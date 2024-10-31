/*!
@file Enemy.h
@brief �G�Ȃ�
�S���F�팩
*/

#pragma once
#include "stdafx.h"
#include "FixedBox.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject
	{
	public:
		enum State {
			stay,      //�ړ��Ȃ�
			rightMove, //�Ǐ]���E�ړ�
			upMove,    //�㉺�ړ�
			flyMove,   //�Ǐ]���V�ړ�
			fly,       //���V
			fixedStay, //�v���C���[�ɐG�ꂽ�ʒu�ŌŒ�
			runaway,   //�v���C���[�Ƌt�����Ɉړ�
			hitDrop,   //�q�b�v�h���b�v
			plunge,    //�˂�����
			attack
		};

	private:
		float m_hp;          //�̗�
		float m_maxHp;       //�ő�̗�
		float m_speed;       //�ړ����x
		float m_upSpeed;     //�㉺�̑��x
		float m_upHeight;    //�㉺�̍���
		float m_jumpPower;   //�W�����v��
		float m_jumpTime;    //�W�����v���x
		float m_time;
		float m_bulletTime;  //�e�̔��ˊԊu
		float m_bulletRange; //�e�𔭎˂��鋗��
		float m_gravity;
		Vec3 m_grav;
		Vec3 m_gravVel;
		float m_test;
		int m_dic;           //�����Ă������ ��-1,�E1
		int m_dic2;

		bool m_bulletFlag;   
		bool m_jumpFlag;
		bool m_flyFlag;
		bool m_floorFlag;
		bool m_hitDropFlag;
		bool m_plungeFlag;
		bool m_plungeColFlag;

		State m_stateType;
		State m_beforState;
		State m_deathState;

		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
		Vec3 m_deathPos;
		Vec3 m_playerScale; //�v���C���[�̃T�C�Y
		Vec3 m_floorPos;
		Vec3 m_jumpPos;

		shared_ptr<Transform> m_trans;
		weak_ptr<Player> m_player;
		weak_ptr<Transform> m_playerTrans;
		weak_ptr<FixedBox> m_box;
		shared_ptr<CollisionObb> m_collision;
		weak_ptr<FixedBox> m_fixedBox;

	public:
		// �\�z�Ɣj��
		Enemy(const shared_ptr<Stage>& stage, 
			const Vec3& position, 
			const Vec3& rotatoin, 
			const Vec3& scale,
			const State& state,
			const State& deathState,
			const shared_ptr<Player>& player,
			const shared_ptr<FixedBox>& box = nullptr);
		virtual ~Enemy(){}
		virtual void OnCreate() override; // ������
		virtual void OnUpdate() override; // �X�V

		void OnCollisionEnter(shared_ptr<GameObject>& other);
		void OnCollisionExcute(shared_ptr<GameObject>& other);
		void OnCollisionExit(shared_ptr<GameObject>& other);
		void EnemyJump();
		void HipDrop();
		void ThisDestroy();
		void ReceiveDamage(float damage);
		void SetEnemyFlayFlag(bool flag);
		void SetSpeed(float speed);
		void SetUpMove(float speed,float height);
		void SetFlyPower(float power);
		void SetState(State state);
		int GetDic();
		Vec3 GetPos();

		//�d�͂Ɋւ���֐�
		void Grav();
		void SetGrav(Vec3 grav);
		void AddGrav(Vec3 grav);
		void GravZero();
		void GravVelZero();
		void SetGravVel(Vec3 grav);

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
		Vec3 m_fixedPos;

		shared_ptr<Transform> m_trans;
		shared_ptr<Enemy> m_enemy;
		

	public :
		EnemyBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy,const Vec3& fixedPos = Vec3(0.0f));
		virtual ~EnemyBullet(){}
		virtual void OnCreate();
		virtual void OnUpdate();
		void OnCollisionEnter(shared_ptr<GameObject>& other);

		void ThisDestroy();

	};


	class MyGravity : public Component {
	public:
		explicit MyGravity(const shared_ptr<GameObject>& GameObjectPtr,
			const bsm::Vec3& gravity = bsm::Vec3(0, -9.8f, 0));
		MyGravity::~MyGravity() {}

	private:
		Vec3 m_gravity;
		//struct Impl;
		//unique_ptr<Impl> pImpl;
	public:
		Vec3 GetGravity() const;
		void SetGravity(const bsm::Vec3& gravity);
		void SetGravityZero();
		void OnUpdate();
	};

}