/*!
@file FixedBox.cpp
@brief ����u���b�N�Ȃ�
�S���F�팩
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

	//������
	void FixedBox::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_position);
		ptrTransform->SetRotation(m_rotation);
		ptrTransform->SetScale(m_scale);
		//OBB�Փ�j�����t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//�^�O������
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