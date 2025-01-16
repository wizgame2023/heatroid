/*!
@file Gimmick.h
@brief Gimmick�Ȃ�
���c
*/

#pragma once
#include "stdafx.h"

namespace basecross {
//--------------------------------------------------------------------------------------
//�@�^�C�����O����Œ�̃{�b�N�X
//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;         // �{�b�N�X�̃X�P�[���i�T�C�Y�j��ێ�����x�N�g��
		Vec3 m_Rotation;      // �{�b�N�X�̉�]����ێ�����x�N�g��
		Vec3 m_Position;      // �{�b�N�X�̈ʒu��ێ�����x�N�g��
		float m_UPic;         // �e�N�X�`����U�������̃s�N�`���T�C�Y
		float m_VPic;         // �e�N�X�`����V�������̃s�N�`���T�C�Y
		wstring m_Texname;    // �e�N�X�`���̖��O
	public:
		// �\�z�Ɣj��
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const float& UPic,
			const float& VPic,
			const wstring& m_Texname
		);
		virtual ~TilingFixedBox();

		// ������
		virtual void OnCreate() override;
	};

	class GimmickButton;
	class GimmickDoor : public GameObject {
		Vec3 m_Position;                       // �h�A�̈ʒu��ێ�����x�N�g��
		Vec3 m_Rotation;                      // �h�A�̉�]����ێ�����x�N�g��
		Vec3 m_Scale;                         // �h�A�̃X�P�[���i�T�C�Y�j��ێ�����x�N�g��
		bool m_open;                          // �h�A���J���Ă��邩�̃t���O
		bool m_open2;                         // ������̃h�A�J�t���O
		float m_UPic;                         // �e�N�X�`����U�������̃s�N�`���T�C�Y
		float m_VPic;                         // �e�N�X�`����V�������̃s�N�`���T�C�Y
		shared_ptr<GimmickButton> GDoor;      // �֘A����M�~�b�N�{�^��
		int kazu = 1;                         // ���ʂ�\���ϐ�
		int m_number;                         // �h�A�̎��ʔԍ�
		bool m_Flag;                          // �t���O
	public:
		float m_OpenSwitch;                   // �h�A�̊J�X�C�b�`
		wstring m_Texname;                    // �e�N�X�`���̖��O
		// �\�z�Ɣj��
		GimmickDoor(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scales,
			float UPic,
			float VPic,
			float m_OpenSwitch,
			int number,
			const wstring& m_Texname
		);
		virtual ~GimmickDoor();

		// ������
		virtual void OnCreate() override;

		// �X�V����
		virtual void OnUpdate() override;

		// �h�A���J��
		void OpenDoor();

		// �T�E���h�G�t�F�N�g���Đ�����
		void PlaySE(wstring path, float loopcnt, float volume);
	};

	class GimmickUp : public GameObject {
		Vec3 m_Position;                       // �M�~�b�N�̈ʒu��ێ�����x�N�g��
		Vec3 m_Rotation;                      // �M�~�b�N�̉�]����ێ�����x�N�g��
		Vec3 m_Scale;                         // �M�~�b�N�̃X�P�[���i�T�C�Y�j��ێ�����x�N�g��
		bool m_open;                          // �M�~�b�N���J���Ă��邩�̃t���O
		bool m_open2;                         // ������̃M�~�b�N�J�t���O
		float m_UPic;                         // �e�N�X�`����U�������̃s�N�`���T�C�Y
		float m_VPic;                         // �e�N�X�`����V�������̃s�N�`���T�C�Y
		shared_ptr<GimmickButton> GDoor;      // �֘A����M�~�b�N�{�^��
		int kazu = 1;                         // ���ʂ�\���ϐ�
		int ido = 0;                          // �ړ��ʂ�\���ϐ�
		int m_number;                         // �M�~�b�N�̎��ʔԍ�
		bool m_Flag;                          // �t���O
	public:
		float m_Max;                          // �M�~�b�N�̍ő�l
		float m_OpenSwitch;                   // �M�~�b�N�̊J�X�C�b�`
		wstring m_Texname;                    // �e�N�X�`���̖��O

		// �\�z�Ɣj��
		GimmickUp(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scales,
			float UPic,
			float VPic,
			float m_OpenSwitch,
			int number,
			const wstring& m_Texname,
			float m_Max
		);
		virtual ~GimmickUp();

		// ������
		virtual void OnCreate() override;

		// �X�V����
		virtual void OnUpdate() override;

		// �h�A���J��
		void OpenDoor();

		// �T�E���h�G�t�F�N�g���Đ�����
		void PlaySE(wstring path, float loopcnt, float volume);
	};

	class GimmickButton : public GameObject
	{
	public:
		// �G�t�F�N�g
		shared_ptr<EfkEffect> m_EfkEffect;           // �G�t�F�N�g�I�u�W�F�N�g
		shared_ptr<EfkEffect> m_EfkEffectLoop;       // ���[�v�G�t�F�N�g�I�u�W�F�N�g
		// �G�t�F�N�g���s�I�u�W�F�N�g
		shared_ptr<EfkPlay> m_EfkPlay;               // �G�t�F�N�g���s�I�u�W�F�N�g

		Vec3 m_Position;                             // �M�~�b�N�{�^���̈ʒu��ێ�����x�N�g��
		Vec3 m_Rotation;                            // �M�~�b�N�{�^���̉�]����ێ�����x�N�g��
		Vec3 m_Scale;                               // �M�~�b�N�{�^���̃X�P�[���i�T�C�Y�j��ێ�����x�N�g��
		int m_switch;                               // �X�C�b�`���ʔԍ�
		int m_number;                               // �{�^���̎��ʔԍ�
		wstring m_Texname;                          // �e�N�X�`���̖��O

		bool m_open;                                // �{�^����������Ă��邩�̃t���O
		bool m_flag = false;                        // �t���O
		float time = 0;                             // ���Ԃ��v������ϐ�

		// �\�z�Ɣj��
		GimmickButton(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			int m_switch,
			int number,
			const wstring& m_Texname
		);
		virtual ~GimmickButton();

		// ������
		virtual void OnCreate() override;

		// �X�V����
		virtual void OnUpdate() override;

		// �T�E���h�G�t�F�N�g���Đ�����
		void PlaySE(wstring path, float volume, float loopcnt);

		// �G�t�F�N�g���Đ�����
		void EfectPlay();

		// ���[�v�G�t�F�N�g���Đ�����
		void EfectLoopPlay();

		// ������Ă���̂��̔���
		bool GetButton()
		{
			return m_open;
		}

		// �{�^���̏�Ԃ�ݒ肷��
		void SetButton(const bool& open)
		{
			m_open = open;
		}

		// �X�C�b�`�𔻕ʂ���
		int GetSwitch()
		{
			return m_switch;
		}
	};

	class Door : public GameObject
	{
		Vec3 m_Position;                       // �h�A�̈ʒu��ێ�����x�N�g��
		Vec3 m_Rotation;                      // �h�A�̉�]����ێ�����x�N�g��
		Vec3 m_Scale;                         // �h�A�̃X�P�[���i�T�C�Y�j��ێ�����x�N�g��
		bool m_open;                          // �h�A���J���Ă��邩�̃t���O
		bool m_Goaltrue;                      // �S�[���ɒB�������̃t���O
		wstring m_Texname;                    // �e�N�X�`���̖��O
		float _delta = App::GetApp()->GetElapsedTime(); // �o�ߎ��Ԃ�ێ�����ϐ�
	public:
		// �\�z�Ɣj��
		Door(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const wstring& m_Texname
		);
		virtual ~Door();

		// ������
		virtual void OnCreate() override;

		// �X�V����
		virtual void OnUpdate() override;

		// �A�j���[�V������ǉ�����
		void AddAnim();

		// �A�j���[�V������ύX����(���ɂ��̃A�j�����Đ����Ȃ牽�����Ȃ�)
		const void SetAnim(wstring animname, float time = 0.0f) {
			auto draw = GetComponent<PNTBoneModelDraw>();
			if (draw->GetCurrentAnimation() != animname)
				draw->ChangeCurrentAnimation(animname, time);
		}

		// �{�^���̏�Ԃ�ݒ肷��
		void SetButton(const bool& open)
		{
			m_open = open;
		}
	};

}