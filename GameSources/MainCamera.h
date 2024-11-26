/*!
* ���c��P
@file MainCamera.h
@brief �J�����Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class CameraCollision :public GameObject
	{
		Vec3 GetPos;
		Vec3 TargetPos;
		bool m_Hit;
		float m_ArmLen;
		float m_ToTargetLerp;	//�ڕW��ǂ�������ۂ̕�Ԓl
		bsm::Vec3 m_TargetToAt;	//�ڕW���王�_�𒲐�����ʒu�x�N�g��
		//��]�X�s�[�h
		float m_RotSpeed;
		//���E�X�e�B�b�N�ύX�̃��[�h
		bool m_LRBaseMode;
		float m_RadY;
		float m_RadXZ;


	public:
		CameraCollision(const shared_ptr<Stage>& StagePtr);
		virtual ~CameraCollision() {}
		//������
		virtual void OnCreate();
		//�X�V
		virtual void OnUpdate();
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
		void UpdateArmLengh();
		bool GetLRBaseMode() const;
		bool IsLRBaseMode() const;

	};

	class MainCamera : public Camera {
	public:
		weak_ptr<GameObject> m_TargetObject;	//�ڕW�ƂȂ�I�u�W�F�N�g
		float m_ToTargetLerp;	//�ڕW��ǂ�������ۂ̕�Ԓl
		bsm::Vec3 m_TargetToAt;	//�ڕW���王�_�𒲐�����ʒu�x�N�g��
		float m_RadY;
		float m_RadXZ;
		//�J�����������鉺���p�x
		float m_CameraUnderRot;
		//�r�̒����̐ݒ�
		float m_ArmLen;
		float m_MaxArm;
		float m_MinArm;
		//��]�X�s�[�h
		float m_RotSpeed;
		//�Y�[���X�s�[�h
		float m_ZoomSpeed;
		//���E�X�e�B�b�N�ύX�̃��[�h
		bool m_LRBaseMode;

		bool m_Hit;
		bool m_Pushbool = false;
		POINT m_beforeCursorPos{ 0, 0 };
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		MainCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	ArmLen	�ŏ���Arm�̒���
		*/
		//--------------------------------------------------------------------------------------
		MainCamera(float ArmLen);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MainCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �J�����̈ʒu��ݒ肷��
		@param[in]	Eye	�J�����ʒu
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetEye(const bsm::Vec3& Eye)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �J�����̈ʒu��ݒ肷��
		@param[in]	x	x�ʒu
		@param[in]	y	y�ʒu
		@param[in]	z	z�ʒu
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetEye(float x, float y, float z)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�����̖ڕW�I�u�W�F�N�g�𓾂�
		@return	�J�����̖ڕW
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetTargetObject() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�����̖ڕW�I�u�W�F�N�g��ݒ肷��
		@param[in]	Obj	�J�����̖ڕW�I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetObject(const shared_ptr<GameObject>& Obj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I�u�W�F�N�g��ǂ�������ꍇ�̕�Ԓl�𓾂�
		@return	�I�u�W�F�N�g��ǂ�������ꍇ�̕�Ԓl
		*/
		//-------------------------------------------------------------------------------------
		float GetToTargetLerp() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I�u�W�F�N�g��ǂ�������ꍇ�̕�Ԓl��ݒ肷��
		@param[in]	f	�I�u�W�F�N�g��ǂ�������ꍇ�̕�Ԓl
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetToTargetLerp(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Eye��At�̋����𓾂�
		@return	Eye��At�̋���
		*/
		//--------------------------------------------------------------------------------------
		float GetArmLengh() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Eye��At�̋������X�V����i���݂�Eye��At���狁�߂�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateArmLengh();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Eye��At�̋����̍ő�l�𓾂�
		@return	Eye��At�̋����̍ő�l
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxArm() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Eye��At�̋����̍ő�l��ݒ肷��
		@param[in]	f	Eye��At�̋����̍ő�l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMaxArm(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Eye��At�̋����̍ŏ��l�𓾂�
		@return	Eye��At�̋����̍ŏ��l
		*/
		//--------------------------------------------------------------------------------------
		float GetMinArm() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Eye��At�̋����̍ŏ��l�ݒ肷��
		@param[in]	f	Eye��At�̋����̍ŏ��l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMinArm(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��]�X�s�[�h�𓾂�
		@return	��]�X�s�[�h�i0.0f�ȏ�j
		*/
		//--------------------------------------------------------------------------------------
		float GetRotSpeed() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��]�X�s�[�h��ݒ肷��
		@param[in]	f	��]�X�s�[�h�i�}�C�i�X����͂��Ă��v���X�ɂȂ�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetRotSpeed(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�^�[�Q�b�g����At�ւ̒����x�N�g���𓾂�
		@return	�^�[�Q�b�g����At�ւ̒����x�N�g��
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3 GetTargetToAt() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�^�[�Q�b�g����At�ւ̒����x�N�g����ݒ肷��
		@param[in]	v	�^�[�Q�b�g����At�ւ̒����x�N�g����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetToAt(const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	R�X�e�B�b�N�̍��E�ύX��Base���[�h�ɂ��邩�ǂ����𓾂�
		@return	Base���[�h�Ȃ�true�i�f�t�H���g�j
		*/
		//--------------------------------------------------------------------------------------
		bool GetLRBaseMode() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	R�X�e�B�b�N�̍��E�ύX��Base���[�h�ɂ��邩�ǂ����𓾂�
		@return	Base���[�h�Ȃ�true�i�f�t�H���g�j
		*/
		//--------------------------------------------------------------------------------------
		bool IsLRBaseMode() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	R�X�e�B�b�N�̍��E�ύX��Base���[�h�ɂ��邩�ǂ�����ݒ肷��
		@param[in]	b	Base���[�h�Ȃ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetAt(const bsm::Vec3& At)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �J�����̎��_��ݒ肷��
		@param[in]	x	x�ʒu
		@param[in]	y	y�ʒu
		@param[in]	z	z�ʒu
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetAt(float x, float y, float z)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};

	class OpeningCameraman : public GameObject {
		Vec3 m_startPos;
		Vec3 m_endPos;
		Vec3 m_atStartPos;
		Vec3 m_atEndPos;
		Vec3 m_atPos;
		Vec3 m_eyePos;
		float m_totalTime;
		Vec3 m_secondEndPos;
		Vec3 m_secondAtEndPos;

		Vec3 m_tempStartPos;
		Vec3 m_tempEndPos;
		Vec3 m_tempAtStartPos;
		Vec3 m_tempAtEndPos;
		Vec3 m_tempAtPos;
		float m_tempTotalTime;

		//�X�e�[�g�}�V�[��
		unique_ptr< StateMachine<OpeningCameraman> >  m_StateMachine;
	public:
		//�\�z�Ɣj��
		OpeningCameraman(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& EndPos,
			const Vec3& AtStartPos, const Vec3& AtEndPos, const Vec3& AtPos, float& TotalTime,
			const Vec3& secondEndPos, const Vec3& secondAtEndPos);
		virtual ~OpeningCameraman();
		//������
		virtual void OnCreate() override;
		//����
		virtual void OnUpdate() override;

		//�A�N�Z�T
		const unique_ptr<StateMachine<OpeningCameraman>>& GetStateMachine() {
			return m_StateMachine;
		}

		Vec3 GetAtPos() const {
			return m_atPos;
		}

		Vec3 GetEyePos() const {
			return m_eyePos;
		}
		void ToGoalEnterBehavior();
		void ToStartEnterBehavior();
		bool ExcuteBehavior(float totaltime);
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
}