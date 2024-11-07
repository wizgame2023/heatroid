/*!
@file Gimmick.h
@brief Gimmickなど
鎌田
*/

#pragma once
#include "stdafx.h"

namespace basecross {
//--------------------------------------------------------------------------------------
//　タイリングする固定のボックス
//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const float& UPic,
			const float& VPic
		);
		virtual ~TilingFixedBox();
		//初期化
		virtual void OnCreate() override;
		static vector<weak_ptr<Transform>> m_moveObject;

		//操作
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
		//構築と破棄
		GimmickDoor(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scales,
			float UPic,
			float VPic
		);
		virtual ~GimmickDoor();
		//初期化
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
		//初期化
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