/*!
@file Gimmick.cpp
@brief Gimmickなど
担当：鎌田
*/

#include "stdafx.h"
#include "Project.h"
#include "FixedBox.h"
namespace basecross {


	vector<weak_ptr<Transform>> TilingFixedBox::m_moveObject;


	TilingFixedBox::TilingFixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		const float& UPic,
		const float& VPic,
		const wstring& Texname
	) :
		GameObject(StagePtr),
		m_Position(position),
		m_Rotation(rotation),
		m_Scale(scale),
		m_UPic(UPic),
		m_VPic(VPic),
		m_Texname(Texname)
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
		AddTag(L"FixedBox");
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		//PtrDraw->SetFogEnabled(true);
		//自分に影が映りこむようにする
		//PtrDraw->SetOwnShadowActive(true);
		//描画コンポーネントテクスチャの設定
		PtrDraw->SetTextureResource(m_Texname);
		//タイリング設定
		PtrDraw->SetSamplerState(SamplerState::PointWrap);
	}

	GimmickButton::GimmickButton(const shared_ptr<Stage>& StagePtr,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		float UPic,
		float VPic,
		int Switch,
		int number
	) :
		GameObject(StagePtr),
		m_Position(position),
		m_Rotation(rotation),
		m_Scale(scale),
		m_UPic(UPic),
		m_VPic(VPic),
		m_switch(Switch),
		m_number(number)
	{	}
	GimmickButton::~GimmickButton() {}
	//初期化
	void GimmickButton::OnCreate() {
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
		AddTag(L"FixedBox");
		AddTag(L"GimmickButton");
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
		auto group = GetStage()->GetSharedObjectGroup(L"Switch");
		group->IntoGroup(GetThis<GameObject>());


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
		float VPic,
		float OpenSwitch,
		int number
	) :
		GameObject(StagePtr),
		m_Position(position),
		m_Rotation(rotation),
		m_Scale(scale),
		m_UPic(UPic),
		m_VPic(VPic),
		m_OpenSwitch(OpenSwitch),
		m_number(number)
	{
	}
	GimmickDoor::~GimmickDoor() {}

	//初期化
	void GimmickDoor::OnCreate() {
		m_open = false;
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
		AddTag(L"FixedBox");
		AddTag(L"GimmickDoor");
		AddNumTag(m_OpenSwitch);
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

		auto group = GetStage()->GetSharedObjectGroup(L"Door");
		group->IntoGroup(GetThis<GameObject>());
	}

	void GimmickDoor::OnUpdate()
	{
		OpenDoor();
	}

	void GimmickDoor::OpenDoor()
	{
		auto ptrTransform = GetComponent<Transform>();
		Vec3 pos = ptrTransform->GetPosition();
		auto group = GetStage()->GetSharedObjectGroup(L"Switch");
		auto& vec = group->GetGroupVector();
		for (auto& v : vec) {
			auto shObj = v.lock();
			auto Switchs = dynamic_pointer_cast<GimmickButton>(shObj);
			auto Switch = Switchs->GetSwitch();
			if (Switch == m_OpenSwitch)
			{
				m_open = Switchs->GetButton();
				if (m_open)
				{
					if (m_number == 1)
					{
						for (int i = 0; i < 1; i++)
						{
							ptrTransform->SetPosition(Vec3(pos.x, pos.y += 0.01f, pos.z));
						}
					}
					else if (m_number == 2)
					{
						for (auto& v2 : vec) {
							auto shObj2 = v2.lock();							
							auto Switchs2 = dynamic_pointer_cast<GimmickButton>(shObj2);
							auto Switch2 = Switchs2->GetSwitch();
							if (Switch2 == m_OpenSwitch && Switchs2!=Switchs)
							{
								m_open2 = Switchs2->GetButton();
								if (m_open2)
								{
									for (int i = 0; i < 1; i++)
									{
										ptrTransform->SetPosition(Vec3(pos.x, pos.y += 0.01f, pos.z));
									}		
								}
								else
								{
									ptrTransform->SetPosition(m_Position);
								}									
							}
						}

					}

				}
				else
				{
					ptrTransform->SetPosition(m_Position);
				}

			}
		}
	}

}