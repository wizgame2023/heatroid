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
	public:
		//�\�z�Ɣj��
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const float& UPic,
			const float& VPic
		);
		virtual ~TilingFixedBox();
		//������
		virtual void OnCreate() override;
		static vector<weak_ptr<Transform>> m_moveObject;

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
	public:
		float m_OpenSwitch;
		//�\�z�Ɣj��
		GimmickDoor(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scales,
			float UPic,
			float VPic,
			float m_OpenSwitch,
			int number
		);
		virtual ~GimmickDoor();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void OpenDoor();
	};

	class GimmickButton : public GameObject
	{
		Vec3 m_Position;
		Vec3 m_Rotation;
		Vec3 m_Scale;
		bool m_open;
		float m_UPic;
		float m_VPic;
		shared_ptr<GameObject> GDoor;
		int m_number;
	public:
		int m_switch;
		GimmickButton(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			float UPic,
			float VPic,
			int m_switch,
			int number
		);
		virtual ~GimmickButton();
		//������
		virtual void OnCreate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;

		bool GetButton() {
			return m_open;
		}
		void SetButton(const bool& open)
		{
			m_open = open;
		}
		
		int GetSwitch()
		{
			return m_switch;
		}
	};

}