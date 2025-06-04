/*!
@file Enemy.h
@brief �G�Ȃ�
�S���F�팩
*/

#pragma once
#include "stdafx.h"
#include "FixedBox.h"
#include "Gimmick.h"
#include "EnemyState.h"
#include "EnemyImage.h"


namespace basecross {
	class EnemyFloorCol;
	class EnemyBullet;
	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;
	//--------------------------------------------------------------------------------------
	static const int MAX_EFFECT_NUM = 4; //�G�t�F�N�g�̐�
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
			slide,     //���E�Ɉړ����Ȃ���e������
			throwAway, //��������
		};

	public:
		float m_heat;          //�I�[�o�[�q�[�g�l
		float m_maxHeat;       //�ő�I�[�o�[�q�[�g
		float m_angle;          //�v���C���[�Ɍ����p�x
		float m_speed;         //�ړ����x
		float m_maxSpeed;      //���̍ő�l
		float m_upSpeed;       //�㉺�̑��x
		float m_bulletTime;    //�e�̔��ˊԊu
		float m_maxBulletTime; //���̍ő�l
		float m_parabolaBulletTime;    //�������ɔ�΂��e�̊Ԋu
		float m_maxParabolabulletTime; //���̍ő�l
		int m_bulletCnt;        //�A���ŏo���e�̐�
		float m_throwLength;    //�����钷�����`���[�W�ʂɂ���ĕς���ϐ�
		float m_bulletRangeTime;//�e�̔��˃N�[���^�C��
		float m_maxBulletRangeTime;//���̍ő�l
		float m_trackingRange;  //�e�𔭎˂��鋗��
		float m_spareTime;      //�˂����݂̗\������
		float m_maxSpareTime;   //���̍ő�l
		float m_rad;            //�p�x�̌v�Z
		float m_throwTime;      //������鎞��
		Vec3 m_playerDirec;     //�v���C���[�ւ̃x�N�g��
		Vec3 m_playerDirecNorm; //�v���C���[�ւ̃x�N�g���̐��K��
		Vec3 m_moveRot;
		Vec2 m_bulletDic;
		//�e�X�g�p
		bool m_bulletFlag;      //�e�𔭎˂���t���O
		bool m_objFlag;         //�I�u�W�F�N�g�ւ̏Փ˃t���O
		bool m_pGrabFlag;       //������Ă�����
		bool m_playerFlag;      //�v���C���[�ւ̏Փ˃t���O
		bool m_throwFlag;       //���������
		bool m_overHeatFlag;    //�I�[�o�[�q�[�g���
		bool m_activeFlag;      //�A�N�e�B�u���

		wstring m_meshName;     //���b�V���̃p�X

		//�d��
		float m_gravity;        //�d�͂̒l
		Vec3 m_grav;            //�d�͂̑傫��
		Vec3 m_gravVel;         //�d�̓x�N�g��

		Vec3 m_firstRot;        //�ŏ��̉�]
		Vec3 m_firstScal;       //�ŏ��̑傫��
		Vec3 m_beforePos;       //�ŏ��̍��W
		Vec3 m_playerPos;       //�v���C���[�̍��W
		Vec3 m_playerScale;     //�v���C���[�̃T�C�Y

		shared_ptr<Transform> m_trans;
		shared_ptr<PNTBoneModelDraw> m_draw;
		shared_ptr<CollisionCapsule> m_collision;
		shared_ptr<EnemyFloorCol> m_floorCol;
		//�v���C���[�̎擾
		weak_ptr<Player> m_player;
		weak_ptr<Transform> m_playerTrans;
		weak_ptr<PlayerGrab> m_playerGrab;
		//�I�[�o�[�q�[�g��UI
		weak_ptr<GaugeSquare> m_gauge;
		weak_ptr<Square> m_gaugeFram;
		//�G�t�F�N�g
		shared_ptr<EfkEffect> m_heatEffect;
		shared_ptr<EfkEffect> m_eyeEffect;
		shared_ptr<EfkEffect> m_burstEffect;
		shared_ptr<EfkPlay> m_EfkPlayer[MAX_EFFECT_NUM];
		//�X�e�[�g
		unique_ptr<EnemyState> m_currentState;  //���݂̃X�e�[�g
		unique_ptr<EnemyState> m_previousState; //��O�̃X�e�[�g

	public:
		// �\�z�Ɣj��
		Enemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const shared_ptr<Player>& player);


		virtual ~Enemy() {}
		virtual void OnCreate() override; // ������
		virtual void OnUpdate() override; // �X�V

		//�Փ˔���
		virtual void OnCollisionEnter(shared_ptr<GameObject>& other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& other) override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& other) override;
	public:
		//�������g�̍폜
		void ThisDestroy();
		//�v���C���[�̕������v�Z
		void PlayerDic();
		//�v���C���[�̕�������������
		void EnemyAngle();
		//�������ɔ��˂���e
		void FallBullet();
		//�^���������˂���e
		void StraightXBullet();
		//�A�˂���e
		void RapidFireBullet(int bulletNum);
		//������I�[�o�[�q�[�g����
		void AroundOverHeat();
		//�A�j���[�V�����̍Đ�
		void EnemyAnime(wstring anime);
		//SE�̍Đ�
		void PlaySE(wstring path, float volume = 1.0f, float loopcnt = 0);
		//�G�t�F�N�g�̍Đ�
		void EffectPlay(const shared_ptr<EfkEffect>& efk,
			const Vec3& pos, const int num, const Vec3& scale = Vec3(1.0f));
		//�G�t�F�N�g�̎~�߂�
		void EffectStop(int num);
		//�G�̖ڂ̏ꏊ��ݒ�
		Vec3 GetEyePos(const Vec3& eye);
		//�f�o�b�N�p
		void Debug();
	public:
		//get,set
		//�p�x�̎擾
		float GetAngle();
		//�X�s�[�h�̐ݒ�
		void SetSpeed(float speed);
		Vec3 GetDirec();
		//���W�̎擾
		Vec3 GetPos();
		//���[���h���W�̎擾
		Vec3 GetWorldPos();
		//���ɐG��Ă邩�̏��
		bool GetFloorFlag();
		//�I�[�o�[�q�[�g���̎擾
		float GetHeatRatio();
		//�I�[�o�[�q�[�g���
		bool GetOverHeat();
		//�e���΂��p�x�̐ݒ�
		void SetBulletDirec(Vec2 direc);
		bool GetActiveFlag();
		void SetActiveFlag(bool flag);
		//�I�[�o�[�q�[�g�ɂ���
		void PlayOverHeat();
		//�����Ă�����
		void SetThrowFlag(bool flag);
		//������Ƃ��Ƃ��̋���
		void SetThorwLenght(float length);

		//�X�e�[�^�X�̐؂�ւ�
		template <class NextState>
		void ChangeState() {
			m_currentState->Exit();
			m_previousState = move(m_currentState);
			m_currentState.reset();
			m_currentState = make_unique<NextState>(GetThis<Enemy>());
			m_currentState->Enter();
		}
		//�X�e�[�^�X�̐؂�ւ��@�i�O�̃X�e�[�^�X�̋L���Ȃ��j
		template <class NextState>
		void OnlyChangeState() {
			m_currentState->Exit();
			m_currentState.reset();
			m_currentState = make_unique<NextState>(GetThis<Enemy>());
			m_currentState->Enter();
		}
		//�O�̃X�e�[�^�X�Ɛ؂�ւ�
		void RetrunState() {
			m_currentState->Exit();
			m_currentState.reset();
			m_currentState = move(m_previousState);
			if (m_currentState) {
				m_currentState->Enter();
			}
		}

	public:
		//�d�͂Ɋւ���֐�
		Vec3 Grav();
		//�d�͂̑傫����ݒ�
		void SetGrav(Vec3 grav);
		//�d�͂��Ȃ���
		void GravZero();
		
	};

	//--------------------------------------------------------------------------------------
	//	class EnemyBullet : public GameObject; //�e
	//--------------------------------------------------------------------------------------
	class EnemyBullet : public GameObject {
	protected:
		float m_colTime;      //�덷�̎���
		bool m_playerColFlag; //�v���C���[�ւ̏Փ˃t���O
		shared_ptr<PNTStaticDraw> m_draw;

	public:
		EnemyBullet(const shared_ptr<Stage>& stage);
		virtual ~EnemyBullet() {};
		virtual void OnCreate();
		virtual void OnUpdate();
		virtual void OnCollisionEnter(shared_ptr<GameObject>& other);
		void SetColor(Col4 color);
		void ThisDestroy();

	};
	//--------------------------------------------------------------------------------------
	//	class StraightBullet : public EnemyBullet; //�����ɒ��Ԓe
	//--------------------------------------------------------------------------------------
	class StraightBullet : public EnemyBullet {
	private:
		float m_speed;   //�e�̑���
		float m_range;   //�˒�
		float m_height;  //�e�𔭎ˎ��̍���
		Vec3 m_pos;
		Vec3 m_scal;
		Vec3 m_enemyPos;

		shared_ptr<Transform> m_trans;
		weak_ptr<Enemy> m_enemy;
	public:
		StraightBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy);
		StraightBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy, const float height);
		virtual ~StraightBullet() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void Debug();
	};

	//--------------------------------------------------------------------------------------
	//	class ParabolaBullet : public EnemyBullet; //��������ɒ��Ԓe
	//--------------------------------------------------------------------------------------
	class ParabolaBullet : public EnemyBullet {
	private:

		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
		Vec3 m_enemyPos;
		float m_speed;   //�e�̑���
		Vec3 m_velocity; //��΂��x�N�g��

		shared_ptr<Transform> m_trans;
		weak_ptr<Enemy> m_enemy;

		Vec3 m_grav;
		Vec3 m_gravVel;


	public:
		ParabolaBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy);
		virtual ~ParabolaBullet() {}
		virtual void OnCreate();
		virtual void OnUpdate();

		void StartVel();
		void Debug();
		void Grav();

	};
	//--------------------------------------------------------------------------------------
	//	class TrackingBullet : public EnemyBullet; //�v���C���[���߂����Ē��Ԓe
	//--------------------------------------------------------------------------------------
	class TrackingBullet : public EnemyBullet {
	private:
		float m_speed;
		float m_range;
		Vec3 m_pos;
		Vec3 m_dic;
		Vec3 m_enemyPos;
		shared_ptr<Transform> m_trans;
		weak_ptr<Enemy> m_enemy;
		weak_ptr<Player> m_player;
	public:
		TrackingBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy, const shared_ptr<Player>& player);
		virtual ~TrackingBullet() {};
		virtual void OnCreate();
		virtual void OnUpdate();
	};

	//--------------------------------------------------------------------------------------
	//	class EnemyFloorCol : public GameObject; //�G�̓���̃R���W����
	//--------------------------------------------------------------------------------------
	class EnemyFloorCol : public GameObject {
	private:
		float m_posHight;
		float m_plusScale;
		Vec3 m_pos;
		Vec3 m_enemyPos;
		Vec3 m_enemyScal;

		bool m_playerFlag;
		shared_ptr<Transform> m_trans;
		weak_ptr<Enemy> m_enemy;



	public:
		EnemyFloorCol(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy);
		virtual ~EnemyFloorCol() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& other);
		virtual void OnCollisionExit(shared_ptr<GameObject>& other);
		void ThisDestroy();
		bool GetPlayerFlag();
		void SetPosHight(float hight);
	};

}