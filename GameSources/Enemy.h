/*!
@file Enemy.h
@brief �G�Ȃ�
�S���F�팩
*/

#pragma once
#include "stdafx.h"
#include "FixedBox.h"
#include "Gimmick.h"

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

		
	protected:
		float m_heat;          //�I�[�o�[�q�[�g�l
		float m_maxHeat;       //�ő�I�[�o�[�q�[�g
		float m_speed;       //�ړ����x
		float m_maxSpeed;
		float m_upSpeed;     //�㉺�̑��x
		float m_upHeight;    //�㉺�̍���
		float m_jumpPower;   //�W�����v��
		float m_jumpTime;    //�W�����v���x
		float m_jumpHight;   //�W�����v���鍂��
		float m_time;
		float m_bulletTime;  //�e�̔��ˊԊu
		float m_trackingRange; //�e�𔭎˂��鋗��
		float m_dropTime;
		float m_maxDropTime;
		float m_hitDropTime;
		float m_maxHitDropTime;
		float m_plungeTime;
		int m_plungeColFlag;
		Vec3 m_moveRot;

		Vec3 m_direc;
		int m_dicUp;
		Vec3 m_firstDirec;

		float m_test;
		//�d��
		float m_gravity;
		Vec3 m_grav;
		Vec3 m_gravVel;

		bool m_bulletFlag;   
		bool m_jumpFlag;
		bool m_flyFlag;
		bool m_floorFlag;
		bool m_hitDropFlag;
		bool m_plungeFlag;

		State m_stateType;
		State m_beforState;
		State m_overHeatState;

		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
		Vec3 m_beforePos;
		Vec3 m_deathPos;
		Vec3 m_playerPos;
		Vec3 m_playerScale; //�v���C���[�̃T�C�Y

		Vec3 m_floorPos;
		Vec3 m_jumpPos;

		shared_ptr<Transform> m_trans;
		weak_ptr<Player> m_player;
		weak_ptr<Transform> m_playerTrans;
		weak_ptr<FixedBox> m_box;
		shared_ptr<CollisionObb> m_collision;
		weak_ptr<TilingFixedBox> m_fixedBox;
		

	public:
		// �\�z�Ɣj��
		Enemy(const shared_ptr<Stage>& stage, 
			const Vec3& position, 
			const Vec3& rotatoin, 
			const Vec3& scale,
			const State& state,
			const State& overHeatState,
			const shared_ptr<Player>& player);
		virtual ~Enemy(){}
		virtual void OnCreate() override; // ������
		virtual void OnUpdate() override; // �X�V

		void OnCollisionEnter(shared_ptr<GameObject>& other);
		void OnCollisionExit(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		void EnemyJump();
		void HipDropJump();
		void ThisDestroy();
		void ReceiveDamage(float damage);
		void PlayerDic();
		void OneJump(float jumpHight);
		void HitDrop();
		void FindFixed();
		void Bullet();
		void EnemyAnime(wstring anime);
		void OverHeat();
		void Debug();
		//get,set
		void SetEnemyFlayFlag(bool flag);
		void SetSpeed(float speed);
		void SetUpMove(float speed,float height);
		void SetFlyPower(float power);
		void SetState(State state);
		Vec3 GetDirec();
		Vec3 GetPos();
		Vec3 GetChangePos();
		bool GetFloorFlag();
		void EnemyAngle();
		float GetHeatRatio();
		bool GetOverHeat();
		void SetPlungeFlag(bool flag);

	protected:
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
		Vec3 m_direc;
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
		Vec3 m_enemyPos;
		Vec3 m_fixedPos;
		bool m_floorCheck;
		bool m_beforFlag;
		float m_test;

		shared_ptr<Transform> m_trans;
		weak_ptr<Enemy> m_enemy;
		

	public :
		EnemyBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy,
			const Vec3& fixedPos = Vec3(0.0f));
		virtual ~EnemyBullet(){}
		virtual void OnCreate();
		virtual void OnUpdate();
		void OnCollisionEnter(shared_ptr<GameObject>& other);

		void ThisDestroy();
		void Debug();

	};



}