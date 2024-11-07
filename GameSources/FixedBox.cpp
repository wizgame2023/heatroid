/*!
@file FixedBox.cpp
@brief 足場ブロックなど
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"
#include "FixedBox.h"

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
		//�`��
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");

		//OBB衝突j判定を付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		ptrColl->SetDrawActive(true);
		//タグをつける
		AddTag(L"FixedBox");

	}

	Vec3 FixedBox::GetPositoin() {
		return m_pos;
	}
	Vec3 FixedBox::GetScale() {
		return m_scal;
	}

	BoxColl::BoxColl(const shared_ptr<Stage>& stage,
		const Vec3& position
	) :
		GameObject(stage),
		m_position(position)
		

	{}

	void BoxColl::OnCreate()
	{
		auto trans = AddComponent<Transform>();
		trans->SetPosition(m_position);
		trans->SetRotation(Vec3(0,0,0));
		trans->SetScale(Vec3(1,1,1));

		//Coll->SetUpdateActive(false);
		AddTag(L"FixedBox");
	}

	vector<weak_ptr<Transform>> TilingFixedBox::m_moveObject;


	TilingFixedBox::TilingFixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		const float& UPic,
		const float& VPic
		) :
		GameObject(StagePtr),
		m_Position(position),
		m_Rotation(rotation),
		m_Scale(scale),
		m_UPic(UPic),
		m_VPic(VPic)
	{
	}
	TilingFixedBox::~TilingFixedBox() {}
	//初期化
	void TilingFixedBox::OnCreate() {
		auto Trans = AddComponent<Transform>();
		Trans->SetPosition(m_Position);
		Trans->SetRotation(m_Rotation);
		Trans->SetScale(m_Scale);
		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);
		Coll->SetDrawActive(true);

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		float UCount = m_Scale.x / m_UPic;
		float VCount = m_Scale.z / m_VPic;
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
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		//PtrDraw->CreateOriginalMesh(vertices, indices);
		//PtrDraw->SetOriginalMeshUse(true);
		//PtrDraw->SetFogEnabled(true);
		//自分に影が映りこむようにする
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		//タイリング設定
		//->SetSamplerState(SamplerState::LinearWrap);
	}
	void TilingFixedBox::OnUpdate()
	{
		
	}
	//--------------------------------------------------------------------------------------
	//	class BreakWall : public GameObject;
	//--------------------------------------------------------------------------------------
	BreakWall::BreakWall(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale
	) :
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
		AddTag(L"BreakWall");


	}
	void BreakWall::OnUpdate() {

	}
	void BreakWall::ThisDestory() {
		GetStage()->RemoveGameObject<BreakWall>(GetThis<BreakWall>());
	}
}