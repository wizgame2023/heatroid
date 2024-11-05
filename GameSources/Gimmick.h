/*!
@file Gimmick.h
@brief GimmickÇ»Ç«
*/

#pragma once
#include "stdafx.h"

namespace basecross {
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
		//ç\ízÇ∆îjä¸
		GimmickDoor(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scales,
			float UPic,
			float VPic
		);
		virtual ~GimmickDoor();
		//èâä˙âª
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
		//èâä˙âª
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