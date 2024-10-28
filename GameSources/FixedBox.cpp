/*!
@file FixedBox.cpp
@brief ����u���b�N�Ȃ�
�S���F�팩
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

	//������
	void FixedBox::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetRotation(m_rot);
		ptrTransform->SetScale(m_scal);
		//�`��
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//�Փ˔���
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//�^�O������
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
		//�`��
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//�Փ˔���
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//�^�O������
		AddTag(L"FixedBox");
		AddTag(L"BreakWall");


	}
	void BreakWall::OnUpdate() {

	}
	void BreakWall::ThisDestory() {
		GetStage()->RemoveGameObject<BreakWall>(GetThis<BreakWall>());
	}
}