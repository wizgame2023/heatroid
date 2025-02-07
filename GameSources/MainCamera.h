/*!
* ���c��P
@file MainCamera.h
@brief �J�����Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {



	class CameraCollision : public GameObject
	{
		Vec3 GetPos;                            // �J�����̌��݈ʒu��ێ�����x�N�g��
		Vec3 TargetPos;                         // �J�����̖ڕW�ʒu��ێ�����x�N�g��
		bool m_Hit;                             // �q�b�g����t���O
		float m_ArmLen;                         // �r�̒�����ێ�����ϐ�
		float m_ToTargetLerp;                   // �ڕW��ǂ�������ۂ̕�Ԓl
		bsm::Vec3 m_TargetToAt;                 // �ڕW���王�_�𒲐�����ʒu�x�N�g��
		float m_RotSpeed;						// ��]�X�s�[�h
		bool m_LRBaseMode;						// ���E�X�e�B�b�N�ύX�̃��[�h
		float m_RadY;                           // Y���̉�]�p�x
		float m_RadXZ;                          // XZ���ʂ̉�]�p�x

	public:
		// �R���X�g���N�^
		CameraCollision(const shared_ptr<Stage>& StagePtr);
		virtual ~CameraCollision() {}

		// ������
		virtual void OnCreate();

		// �X�V����
		virtual void OnUpdate();

		// �Փˏ����̎��s
		virtual void OnCollisionExcute(const CollisionPair& Pair) override;

		// �ՓˊJ�n���̏���
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

		// �ՓˏI�����̏���
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;

		// �r�̒������X�V����
		void UpdateArmLengh();

		// ���E�X�e�B�b�N�ύX�̃��[�h���擾����
		bool GetLRBaseMode() const;

		// ���E�X�e�B�b�N�ύX�̃��[�h���ǂ����𔻒肷��
		bool IsLRBaseMode() const;
	};

	class MainCamera : public Camera {
	public:
		weak_ptr<GameObject> m_TargetObject;    // �ڕW�ƂȂ�I�u�W�F�N�g
		float m_ToTargetLerp;                   // �ڕW��ǂ�������ۂ̕�Ԓl
		bsm::Vec3 m_TargetToAt;                 // �ڕW���王�_�𒲐�����ʒu�x�N�g��
		float m_RadY;                           // Y���̉�]�p�x
		float m_RadXZ;                          // XZ���ʂ̉�]�p�x
		float m_CameraUnderRot;					// �J�����������鉺���p�x
		float m_ArmLen;							// �r�̒����̐ݒ�
		float m_MaxArm;                         // �r�̍ő咷��
		float m_MinArm;                         // �r�̍ŏ�����
		float m_RotSpeed;						// ��]�X�s�[�h
		float m_ZoomSpeed;						// �Y�[���X�s�[�h
		bool m_LRBaseMode;						// ���E�X�e�B�b�N�ύX�̃��[�h

		bool m_Hit;                             // �q�b�g����
		bool m_Pushbool = false;                // �{�^�������̏��
		POINT m_beforeCursorPos{ 0, 0 };        // �J�[�\���̑O��ʒu

		// �R���X�g���N�^
		MainCamera();
		MainCamera(float ArmLen);
		virtual ~MainCamera();

		// �J�����̎��_��ݒ肷��i�x�N�g���j
		virtual void SetEye(const bsm::Vec3& Eye) override;
		// �J�����̎��_��ݒ肷��i���W�j
		virtual void SetEye(float x, float y, float z) override;

		// �ڕW�I�u�W�F�N�g���擾����
		shared_ptr<GameObject> GetTargetObject() const;

		// �ڕW�I�u�W�F�N�g��ݒ肷��
		void SetTargetObject(const shared_ptr<GameObject>& Obj);

		// ��Ԓl���擾����
		float GetToTargetLerp() const;

		// ��Ԓl��ݒ肷��
		void SetToTargetLerp(float f);

		// �r�̒������擾����
		float GetArmLengh() const;

		// �r�̒������X�V����
		void UpdateArmLengh();

		// �r�̍ő咷�����擾����
		float GetMaxArm() const;

		// �r�̍ő咷����ݒ肷��
		void SetMaxArm(float f);

		// �r�̍ŏ��������擾����
		float GetMinArm() const;

		// �r�̍ŏ�������ݒ肷��
		void SetMinArm(float f);

		// ��]�X�s�[�h���擾����
		float GetRotSpeed() const;

		// ��]�X�s�[�h��ݒ肷��
		void SetRotSpeed(float f);

		// �ڕW���王�_�𒲐�����ʒu�x�N�g�����擾����
		bsm::Vec3 GetTargetToAt() const;

		// �ڕW���王�_�𒲐�����ʒu�x�N�g����ݒ肷��
		void SetTargetToAt(const bsm::Vec3& v);

		// ���E�X�e�B�b�N�ύX�̃��[�h���擾����
		bool GetLRBaseMode() const;

		// ���E�X�e�B�b�N�ύX�̃��[�h���ǂ����𔻒肷��
		bool IsLRBaseMode() const;

		// �����_��ݒ肷��i�x�N�g���j
		virtual void SetAt(const bsm::Vec3& At) override;

		// �����_��ݒ肷��i���W�j
		virtual void SetAt(float x, float y, float z) override;

		// �X�V����
		virtual void OnUpdate() override;
	};

//--------------------------------------------------------------------------------------
//	class RayMark : public GameObject; //�I�u�W�F�N�g�ɐG��Ă��邩�ǂ���
//--------------------------------------------------------------------------------------
	class RayCameraMark : public GameObject {
	private:
		bool m_hitEnemyFlag;
		bool m_hitCameraFlag;
		bool m_activeFlag;
		int m_wallCnt;
		int m_doorCnt;
		Vec3 m_pos;

		vector<bool> m_hitWallFlag;
		vector<bool> m_hitDoorFlag;
		shared_ptr<Transform> m_trans;
		shared_ptr<PNTStaticDraw> m_draw;
		weak_ptr<Player> m_player;
		weak_ptr<Camera> m_camera;


	public:
		RayCameraMark(const shared_ptr<Stage>& stage);
		RayCameraMark(const shared_ptr<Stage>& stage, const shared_ptr <Player>& player, const shared_ptr<Camera>& enemy);
		virtual ~RayCameraMark() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void Debug();
		Vec3 enemyPos(Vec3 pos);
		bool GetActiveFlag();
		Vec3 GetActivePos();

	};


	class OpeningCameraman : public GameObject {
		Vec3 m_startPos;                          // �J�����̊J�n�ʒu��ێ�����x�N�g��
		Vec3 m_endPos;                            // �J�����̏I���ʒu��ێ�����x�N�g��
		Vec3 m_atStartPos;                        // �����_�̊J�n�ʒu��ێ�����x�N�g��
		Vec3 m_atEndPos;                          // �����_�̏I���ʒu��ێ�����x�N�g��
		Vec3 m_atPos;                             // ���݂̒����_�ʒu��ێ�����x�N�g��
		Vec3 m_eyePos;                            // ���݂̃J�����ʒu��ێ�����x�N�g��
		float m_totalTime;                        // ���v���Ԃ�ێ�����ϐ�
		Vec3 m_secondEndPos;                      // �񎟏I���ʒu��ێ�����x�N�g��
		Vec3 m_secondAtEndPos;                    // �񎟒����_�I���ʒu��ێ�����x�N�g��

		Vec3 m_tempStartPos;                      // �ꎞ�I�ȊJ�n�ʒu��ێ�����x�N�g��
		Vec3 m_tempEndPos;                        // �ꎞ�I�ȏI���ʒu��ێ�����x�N�g��
		Vec3 m_tempAtStartPos;                    // �ꎞ�I�Ȓ����_�J�n�ʒu��ێ�����x�N�g��
		Vec3 m_tempAtEndPos;                      // �ꎞ�I�Ȓ����_�I���ʒu��ێ�����x�N�g��
		Vec3 m_tempAtPos;                         // �ꎞ�I�Ȓ����_�ʒu��ێ�����x�N�g��
		float m_tempTotalTime;                    // �ꎞ�I�ȍ��v���Ԃ�ێ�����ϐ�

		// �X�e�[�g�}�V��
		unique_ptr< StateMachine<OpeningCameraman> >  m_StateMachine;
	public:
		// �\�z�Ɣj��
		OpeningCameraman(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& EndPos,
			const Vec3& AtStartPos, const Vec3& AtEndPos, const Vec3& AtPos, float& TotalTime,
			const Vec3& secondEndPos, const Vec3& secondAtEndPos);
		virtual ~OpeningCameraman();

		// ������
		virtual void OnCreate() override;

		// ����
		virtual void OnUpdate() override;

		// �A�N�Z�T
		const unique_ptr<StateMachine<OpeningCameraman>>& GetStateMachine() {
			return m_StateMachine;
		}

		Vec3 GetAtPos() const {
			return m_atPos;
		}

		Vec3 GetEyePos() const {
			return m_eyePos;
		}

		// �S�[���G���^�[�r�w�C�r�A
		void ToGoalEnterBehavior();

		// �X�^�[�g�G���^�[�r�w�C�r�A
		void ToStartEnterBehavior();

		// �s�������s����
		bool ExcuteBehavior(float totaltime);

		// �I����ԃG���^�[�r�w�C�r�A
		void EndStateEnterBehavior();
	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToGoalState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class OpeningCameramanToFirstState : public ObjState<OpeningCameraman>
	{
		OpeningCameramanToFirstState() {}
	public:
		static shared_ptr<OpeningCameramanToFirstState> Instance();
		virtual void Enter(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Execute(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Exit(const shared_ptr<OpeningCameraman>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToStartState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class OpeningCameramanToSecondState : public ObjState<OpeningCameraman>
	{
		OpeningCameramanToSecondState() {}
	public:
		static shared_ptr<OpeningCameramanToSecondState> Instance();
		virtual void Enter(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Execute(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Exit(const shared_ptr<OpeningCameraman>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanEndState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class OpeningCameramanEndState : public ObjState<OpeningCameraman>
	{
		OpeningCameramanEndState() {}
	public:
		static shared_ptr<OpeningCameramanEndState> Instance();
		virtual void Enter(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Execute(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Exit(const shared_ptr<OpeningCameraman>& Obj)override;
	};

	class OpeningCamera : public Camera {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		OpeningCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~OpeningCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};

	//�X�e�[�W�N���A���̃J����
	class EndingCameraman : public GameObject {
		Vec3 m_startPos;
		Vec3 m_endPos;
		Vec3 m_atStartPos;
		Vec3 m_atEndPos;
		Vec3 m_eyePos;
		Vec3 m_atPos;
		float m_totalTime;

		//�X�e�[�g�}�V�[��
		unique_ptr< StateMachine<EndingCameraman> >  m_StateMachine;
	public:
		//�\�z�Ɣj��
		EndingCameraman(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& EndPos,
			const Vec3& AtStartPos, const Vec3& AtEndPos, const float& TotalTime);
		virtual ~EndingCameraman();
		//������
		virtual void OnCreate() override;
		//����
		virtual void OnUpdate() override;

		//�A�N�Z�T
		const unique_ptr<StateMachine<EndingCameraman>>& GetStateMachine() {
			return m_StateMachine;
		}

		Vec3 GetAtPos() const {
			return m_atPos;
		}

		Vec3 GetEyePos() const {
			return m_eyePos;
		}

		void ExcuteBehavior(float totaltime);
		void BasicStateEnterBehavior();
	};

	//--------------------------------------------------------------------------------------
	//	class EndingCameramanBasicState : public ObjState<EndingCameraman>;
	//--------------------------------------------------------------------------------------
	class EndingCameramanBasicState : public ObjState<EndingCameraman>
	{
		EndingCameramanBasicState() {}
	public:
		static shared_ptr<EndingCameramanBasicState> Instance() {
			static shared_ptr<EndingCameramanBasicState> instance(new EndingCameramanBasicState);
			return instance;
		};
		virtual void Enter(const shared_ptr<EndingCameraman>& Obj)override;
		virtual void Execute(const shared_ptr<EndingCameraman>& Obj)override;
		virtual void Exit(const shared_ptr<EndingCameraman>& Obj)override;
	};

	class EndingCamera : public Camera {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		EndingCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~EndingCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};

}

