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
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
	public:
		//構築と破棄
		FixedBox(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale
		);

		virtual ~FixedBox(){}
		//������

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
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
	public:
		BreakWall(const shared_ptr<Stage>& m_stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale);
		virtual ~BreakWall(){}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//virtual void OnCollisionEnter(shared_ptr<GameObject>& other) override;
		void ThisDestory();

	};

}
//end basecross
