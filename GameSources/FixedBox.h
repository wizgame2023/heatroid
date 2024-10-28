/*!
@file FixedBox.h
@brief 足場ブロックなど
担当：逸見
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vec3 m_position;
		Vec3 m_rotation;
		Vec3 m_scale;
	public:
		//構築と破棄
		FixedBox(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale
		);
		virtual ~FixedBox();
		//初期化
		virtual void OnCreate() override;
		Vec3 GetPositoin();
		Vec3 GetScale();
	};

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
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//	class BreakWall : public GameObject;
	//--------------------------------------------------------------------------------------
	class BreakWall :public GameObject {

	public:
		BreakWall(const shared_ptr<Stage>& m_stage);
		virtual ~BreakWall();
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
//end basecross
