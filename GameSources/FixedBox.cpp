/*!
@file FixedBox.cpp
@brief 足場ブロックなど
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale
	) :
		GameObject(StagePtr),
		m_position(position),
		m_rotation(rotation),
		m_scale(scale)
	{
	}
	FixedBox::~FixedBox() {}

	//初期化
	void FixedBox::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_position);
		ptrTransform->SetRotation(m_rotation);
		ptrTransform->SetScale(m_scale);
		//OBB衝突j判定を付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//タグをつける
		AddTag(L"FixedBox");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");

	}

	Vec3 FixedBox::GetPositoin() {
		return m_position;
	}
	Vec3 FixedBox::GetScale() {
		return m_scale;
	}

	//--------------------------------------------------------------------------------------
	//	class BreakWall : public GameObject;
	//--------------------------------------------------------------------------------------

}