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
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
		wstring m_Texname;
	public:
		//�\�z�Ɣj��
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const float& UPic,
			const float& VPic,
			const wstring& m_Texname
		);
		virtual ~TilingFixedBox();
		//������
		virtual void OnCreate() override;

		//����
	};

	class GimmickButton;
	class GimmickDoor : public GameObject {
		Vec3 m_Position;
		Vec3 m_Rotation;
		Vec3 m_Scale;
		bool m_open;
		bool m_open2;
		float m_UPic;
		float m_VPic;
		shared_ptr<GimmickButton> GDoor;
		int kazu = 1;
		int m_number;
		bool m_Flag;
	public:
		float m_OpenSwitch;
		wstring m_Texname;
		//�\�z�Ɣj��
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
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void OpenDoor();
		void PlaySE(wstring path, float loopcnt, float volume);
	};

	class GimmickUp : public GameObject {
		Vec3 m_Position;
		Vec3 m_Rotation;
		Vec3 m_Scale;
		bool m_open;
		bool m_open2;
		float m_UPic;
		float m_VPic;
		shared_ptr<GimmickButton> GDoor;
		int kazu = 1;
		int m_number;
		bool m_Flag;
	public:
		float m_Max;
		float m_OpenSwitch;
		wstring m_Texname;
		//�\�z�Ɣj��
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
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void OpenDoor();
		void PlaySE(wstring path, float loopcnt, float volume);
	};

	class GimmickButton : public GameObject
	{
		public:
			//�G�t�F�N�g
			shared_ptr<EfkEffect> m_EfkEffect;
			//�G�t�F�N�g���s�I�u�W�F�N�g
			shared_ptr<EfkPlay> m_EfkPlay;


			Vec3 m_Position;
			Vec3 m_Rotation;
			Vec3 m_Scale;
			int m_switch;
			int m_number;
			wstring m_Texname;

			bool m_open;
			bool m_flag = false;

			GimmickButton(const shared_ptr<Stage>& stage,
				const Vec3& position,
				const Vec3& rotation,
				const Vec3& scale,
				int m_switch,
				int number,
				const wstring& m_Texname
			);
			virtual ~GimmickButton();
			//������
			virtual void OnCreate() override;
			virtual void OnUpdate() override;
			void PlaySE(wstring path, float volume, float loopcnt);
			void EfectPlay();
			//������Ă���̂��̔���
			bool GetButton() 
			{
				return m_open;
			}
			void SetButton(const bool& open)
			{
				m_open = open;
			}
			//swich����
			int GetSwitch()
			{
				return m_switch;
			}
	};

	class Door : public GameObject
	{
		Vec3 m_Position;
		Vec3 m_Rotation;
		Vec3 m_Scale;
		bool m_open;
		bool m_Goaltrue;
		wstring m_Texname;
		float _delta = App::GetApp()->GetElapsedTime();
	public:
		Door(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const wstring& m_Texname
		);
		virtual ~Door();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void AddAnim();

		//�A�j���[�V������ύX����(���ɂ��̃A�j�����Đ����Ȃ牽�����Ȃ�)
		const void SetAnim(wstring animname, float time = 0.0f) {
			auto draw = GetComponent<PNTBoneModelDraw>();
			if (draw->GetCurrentAnimation() != animname)
				draw->ChangeCurrentAnimation(animname, time);
		}

		void SetButton(const bool& open)
		{
			m_open = open;
		}

	};

}