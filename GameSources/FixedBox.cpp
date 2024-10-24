/*!
@file FixedBox.cpp
@brief ����u���b�N�Ȃ�
�S���F�팩
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_scale(Scale),
		m_rotation(Rotation),
		m_position(Position)
	{
	}
	FixedBox::~FixedBox() {}

	//������
	void FixedBox::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_scale);
		ptrTransform->SetRotation(m_rotation);
		ptrTransform->SetPosition(m_position);
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

}