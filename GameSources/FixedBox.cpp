/*!
@file FixedBox.cpp
@brief 足場ブロックなど
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	FixedBox::FixedBox(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale
	) :
		GameObject(stage),
		m_pos(position),
		m_rot(rotation),
		m_scal(scale)
	{}

	//初期化
	void FixedBox::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetRotation(m_rot);
		ptrTransform->SetScale(m_scal);
		//描画
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//衝突判定
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//タグをつける
		AddTag(L"FixedBox");

	}

	Vec3 FixedBox::GetPositoin() {
		return m_pos;
	}
	Vec3 FixedBox::GetScale() {
		return m_scal;
	}

	//--------------------------------------------------------------------------------------
	//	class BreakWall : public GameObject;
	//--------------------------------------------------------------------------------------
	BreakWall::BreakWall(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale
		):
		GameObject(stage),
		m_pos(position),
		m_rot(rotation),
		m_scal(scale)

	{}
	void BreakWall::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetRotation(m_rot);
		ptrTransform->SetScale(m_scal);
		//描画
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//衝突判定
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//タグをつける
		AddTag(L"FixedBox");
		AddTag(L"BreakWall");


	}
	void BreakWall::OnUpdate() {

	}
	void BreakWall::ThisDestory() {
		GetStage()->RemoveGameObject<BreakWall>(GetThis<BreakWall>());
	}
}