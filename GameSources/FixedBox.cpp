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

	TilingFixedBox::TilingFixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position,
		float UPic,
		float VPic
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position),
		m_UPic(UPic),
		m_VPic(VPic)
	{}
	TilingFixedBox::~TilingFixedBox() {}
	//初期化
	void TilingFixedBox::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(m_Rotation);
		PtrTrans->SetPosition(m_Position);
		auto Coll = AddComponent<CollisionObb>();
		AddTag(L"FixedBox");
		Coll->SetFixed(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		float UCount = m_Scale.x / m_UPic;
		float VCount = m_Scale.y / m_VPic;
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
		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//自分に影が映りこむようにする
		PtrDraw->SetOwnShadowActive(true);
		//タイリング設定
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
	}

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
	//初期化
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
		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//自分に影が映りこむようにする
		PtrDraw->SetOwnShadowActive(true);
		//タイリング設定
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

	//初期化
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
		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//自分に影が映りこむようにする
		PtrDraw->SetOwnShadowActive(true);
		//タイリング設定
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
			for (int i = 0; i < 10; i++)
			{
				ptrTransform->SetPosition(Vec3(pos.x, pos.y++, pos.z));
			}
		}
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