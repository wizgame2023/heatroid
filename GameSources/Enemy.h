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
	class EnemyFloorCol;
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
			jump,      //�W�����v
			plunge,    //�˂�����
			bullet,    //�e�������Ă���
			bulletMove,//���̓����������Ă��Ēe������
			wait,      //�����オ��
			slide
		};

		
	protected:
		float m_heat;          //�I�[�o�[�q�[�g�l
		float m_maxHeat;       //�ő�I�[�o�[�q�[�g
		float m_angle;          //�v���C���[�Ɍ����p�x
		float m_speed;         //�ړ����x
		float m_maxSpeed;      //���̍ő�l
		float m_upSpeed;       //�㉺�̑��x
		float m_upHeight;      //�㉺�̍���
		float m_jumpPower;     //�W�����v��
		float m_jumpTime;      //�W�����v���x
		float m_jumpHight;     //�W�����v���鍂��
		float m_bulletTime;    //�e�̔��ˊԊu
		float m_maxBulletTime; //���̍ő�l
		int m_bulletCnt;
		float m_bulletRangeTime;//�e�̔��˃N�[���^�C��
		float m_maxBulletRangeTime;//���̍ő�l
		float m_trackingRange; //�e�𔭎˂��鋗��
		float m_dropTime;      //�q�b�g�h���b�v�܂ł̎���
		float m_maxDropTime;   //���̍ő�l
		float m_hitDropTime;   //�q�b�g�h���b�v���̕����Ă鎞��
		float m_maxHitDropTime;//���̍ő�l
		float m_spareTime;    //�˂����݂̗\������
		float m_maxSpareTime; //���̍ő�l
		int m_dicUp;
		Vec3 m_direc;
		Vec3 m_direcNorm;
		Vec3 m_firstDirec;
		Vec3 m_moveRot;
		Vec2 m_bulletDic;
		//�e�X�g�p
		float m_test;

		bool m_bulletFlag;   
		bool m_jumpFlag;
		bool m_jumpMoveFlag;
		bool m_flyFlag;
		bool m_floorFlag;
		bool m_hitDropFlag;
		bool m_plungeFlag;
		bool m_pGrabFlag;
		bool m_playerFlag;

		wstring m_meshName;

		//�d��
		float m_gravity;
		Vec3 m_grav;
		Vec3 m_gravVel;

		State m_stateType;
		State m_fastState;
		State m_beforeState;
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
		shared_ptr<PNTBoneModelDraw> m_draw;
		shared_ptr<EnemyFloorCol> m_floorCol;
		weak_ptr<Player> m_player;
		weak_ptr<Transform> m_playerTrans;
		weak_ptr<FixedBox> m_box;
		shared_ptr<CollisionObb> m_collision;
		weak_ptr<TilingFixedBox> m_fixedBox;
		weak_ptr<PlayerGrab> m_playerGrab;
		

	public:
		// �\�z�Ɣj��
		Enemy(const shared_ptr<Stage>& stage, 
			const Vec3& position, 
			const Vec3& rotatoin, 
			const Vec3& scale,
			const State& state,
			const State& overHeatState,
			const shared_ptr<Player>& player);
		Enemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const State& state,
			const State& overHeatState,
			const wstring& meshName,
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
		void PlayerDic();
		void OneJump(float jumpHight);
		void HitDrop();
		void Plunge();
		void JumpMove();
		void FindFixed();
		void EnemyAngle();
		void Grab();
		void FallBullet();
		void StraightXBullet();
		void RapidFireBullet(int bulletNum);
		void EnemyAnime(wstring anime);
		void OverHeat();
		void PlayerSE(wstring path, float volume = 1.0f, float loopcnt = 0);
		void Debug();
		//get,set
		float GetAngle();
		void SetEnemyFlayFlag(bool flag);
		void SetSpeed(float speed);
		void SetUpMove(float speed,float height);
		void SetFlyPower(float power);
		void SetState(State state);
		Vec3 GetDirec();
		Vec3 GetPos();
		Vec3 GetChangePos();
		bool GetFloorFlag();
		float GetHeatRatio();
		bool GetOverHeat();
		void SetPlungeFlag(bool flag);
		void SetBulletDirec(Vec2 direc);

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
	//	class StraightXBullet : public GameObject; //�����ɒ��Ԓe
	//--------------------------------------------------------------------------------------
	class EnemyBullet : public GameObject {
	protected:
		shared_ptr<PNTStaticDraw> m_draw;

	public:
		EnemyBullet(const shared_ptr<Stage>& stage);
		virtual ~EnemyBullet() {};
		virtual void OnCreate();
		virtual void OnCollisionEnter(shared_ptr<GameObject>& other);
		void SetColor(Col4 color);
		void ThisDestroy();

	};
	//--------------------------------------------------------------------------------------
	//	class StraightXBullet : public GameObject; //�����ɒ��Ԓe
	//--------------------------------------------------------------------------------------
	class StraightBullet : public EnemyBullet {
	private:
		float m_speed;   //�e�̑���
		float m_Range;   //�˒�
		Vec3 m_pos;
		Vec3 m_scal;
		Vec3 m_enemyPos;

		shared_ptr<Transform> m_trans;
		weak_ptr<Enemy> m_enemy;
	public:
		StraightBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy);
		virtual ~StraightBullet() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void Debug();
	};

	//--------------------------------------------------------------------------------------
	//	class ParabolaBullet : public GameObject; //��������ɒ��Ԓe
	//--------------------------------------------------------------------------------------
	class ParabolaBullet : public EnemyBullet {
	private:
		
		float m_speed;   //�e�̑���
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
		Vec3 m_enemyPos;
		Vec3 m_velocity;
		float m_test;

		shared_ptr<Transform> m_trans;
		weak_ptr<Enemy> m_enemy;

		Vec3 m_grav;
		Vec3 m_gravVel;
		

	public :
		ParabolaBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy);
		virtual ~ParabolaBullet(){}
		virtual void OnCreate();
		virtual void OnUpdate();

		void StartVel();
		void Debug();
		void Grav();

	};

	//--------------------------------------------------------------------------------------
	//	class EnemyFloorCol : public GameObject;
	//--------------------------------------------------------------------------------------
	class EnemyFloorCol : public GameObject {
	private:
		Vec3 m_pos;
		Vec3 m_enemyPos;
		Vec3 m_enemyScal;

		bool m_playerFlag;
		shared_ptr<Transform> m_trans;
		weak_ptr<Enemy> m_enemy;



	public:
		EnemyFloorCol(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy);
		virtual ~EnemyFloorCol(){}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& other);
		virtual void OnCollisionExit(shared_ptr<GameObject>& other);
		void ThisDestroy();
		bool GetPlayerFlag();
	};

}