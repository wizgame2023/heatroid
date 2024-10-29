/*!
@file Gimmick.cpp
@brief Gimmick�Ȃ�
�S���F���c
*/

#include "stdafx.h"
#include "Project.h"
#include "FixedBox.h"
namespace basecross {

	bool GimmickButton::m_open = false;

	GimmickButton::GimmickButton(const shared_ptr<Stage>& StagePtr,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		float UPic,
		float VPic
	) :
		GameObject(StagePtr),
		m_position(position),
		m_rotation(rotation),
		m_scale(scale),
		m_UPic(UPic),
		m_VPic(VPic)
	{	}
	GimmickButton::~GimmickButton() {}
	//������
	void GimmickButton::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_scale);
		PtrTrans->SetRotation(m_rotation);
		PtrTrans->SetPosition(m_position);
		auto Coll = AddComponent<CollisionObb>();
		AddTag(L"GimmickButton");
		Coll->SetFixed(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		float UCount = m_scale.x / m_UPic;
		float VCount = m_scale.y / m_VPic;
		for (size_t i = 0; i < vertices.size(); i++) {
			if (vertices[i].textureCoordinate.x >= 1.0f) {
				vertices[i].textureCoordinate.x = UCount;
			}
			if (vertices[i].textureCoordinate.y >= 1.0f) {
				float FrontBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, 1, 0));
				float BackBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, -1, 0));
				if (FrontBetween < 0.01f || BackBetween < 0.01f) {
					vertices[i].textureCoordinate.y = VCount;
				}
			}
		}
		//�`��R���|�[�l���g�̒ǉ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//�����ɉe���f�肱�ނ悤�ɂ���
		PtrDraw->SetOwnShadowActive(true);
		//�^�C�����O�ݒ�
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
	}

	void GimmickButton::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if ((Other->FindTag(L"Player")))
		{
			m_open = true;
		}
	}

	void GimmickButton::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
		//if ((Other->FindTag(L"Player")) || Other->FindTag(L"Enemy"))
		//{
		//	m_open = false;
		//}
	}

	GimmickDoor::GimmickDoor(const shared_ptr<Stage>& StagePtr,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		float UPic,
		float VPic
	) :
		GameObject(StagePtr),
		m_position(position),
		m_rotation(rotation),
		m_scale(scale),
		m_UPic(UPic),
		m_VPic(VPic)
	{
	}
	GimmickDoor::~GimmickDoor() {}

	//������
	void GimmickDoor::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_scale);
		PtrTrans->SetRotation(m_rotation);
		PtrTrans->SetPosition(m_position);
		auto Coll = AddComponent<CollisionObb>();
		AddTag(L"GimmickDoor");
		Coll->SetFixed(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		float UCount = m_scale.x / m_UPic;
		float VCount = m_scale.y / m_VPic;
		for (size_t i = 0; i < vertices.size(); i++) {
			if (vertices[i].textureCoordinate.x >= 1.0f) {
				vertices[i].textureCoordinate.x = UCount;
			}
			if (vertices[i].textureCoordinate.y >= 1.0f) {
				float FrontBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, 1, 0));
				float BackBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, -1, 0));
				if (FrontBetween < 0.01f || BackBetween < 0.01f) {
					vertices[i].textureCoordinate.y = VCount;
				}
			}
		}
		//�`��R���|�[�l���g�̒ǉ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//�����ɉe���f�肱�ނ悤�ɂ���
		PtrDraw->SetOwnShadowActive(true);
		//�^�C�����O�ݒ�
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
	}

	void GimmickDoor::OnUpdate()
	{
		m_open = GimmickButton::GetButton();
		auto Coll = GetComponent<CollisionObb>();
		Coll->SetSleepActive(true);
		auto ptrTransform = GetComponent<Transform>();
		Vec3 pos = ptrTransform->GetPosition();

		if (m_open)
		{
			for (int i = 0; i < 1; i++)
			{
				ptrTransform->SetPosition(Vec3(pos.x, pos.y += 0.01f, pos.z));
			}
		}
		else
		{
			ptrTransform->SetPosition(m_position);
		}
	}

}