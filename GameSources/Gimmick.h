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
		Vec3 m_position;
		Vec3 m_rotation;
		Vec3 m_scale;
		bool m_open;
		float m_UPic;
		float m_VPic;
		shared_ptr<GimmickButton> GDoor;
	public:
		//�\�z�Ɣj��
		GimmickDoor(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scales,
			float UPic,
			float VPic
		);
		virtual ~GimmickDoor();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	};

	class GimmickButton : public GameObject
	{
		Vec3 m_position;
		Vec3 m_rotation;
		Vec3 m_scale;
		static bool m_open;
		float m_UPic;
		float m_VPic;
		shared_ptr<GameObject> GDoor;
	public:
		GimmickButton(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			float UPic,
			float VPic
		);
		virtual ~GimmickButton();
		//������
		virtual void OnCreate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;

		static bool GetButton() {
			return m_open;
		}
		static void SetButton(const bool& open)
		{
			m_open = open;
		}
	};

}