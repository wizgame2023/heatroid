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
		//構築と破棄
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
		//初期化
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
		//初期化
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