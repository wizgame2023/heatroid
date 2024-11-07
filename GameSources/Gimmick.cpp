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
		AddTag(L"FixedBox");
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
		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<PNTStaticInstanceDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		wstring Datadir;
		App::GetApp()->GetDataDirectory(Datadir);

		CsvFile m_GameStage1;
		vector<wstring> LineVec;
		m_GameStage1.GetSelect(LineVec, 0);
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 160; j++)
			{
				switch (LineVec[i][j])
				{
				case 4:
					float x = -25.0f + j + 25.0f;
					float z = +25.0f + (-i) - 25.0f;
					// インスタンス用の行列を作成する
					Mat4x4 matrix;
					matrix.translation(Vec3(x, 0.0f, z));

					PtrDraw->AddMatrix(matrix); // ブロックを表示したい数だけ行列を追加します。この行列が示す位置・向き・大きさで指定したメッシュが描画されます


					break;
				}
			}
		}

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
		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<PNTStaticInstanceDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		wstring Datadir;
		App::GetApp()->GetDataDirectory(Datadir);

		CsvFile m_GameStage1;
		vector<wstring> LineVec;
		m_GameStage1.GetSelect(LineVec, 0);
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 160; j++)
			{
				switch (LineVec[i][j])
				{
				case 3:
					float x = -25.0f + j + 25.0f;
					float z = +25.0f + (-i) - 25.0f;
					// インスタンス用の行列を作成する
					Mat4x4 matrix;
					matrix.translation(Vec3(x, 0.0f, z));

					PtrDraw->AddMatrix(matrix); // ブロックを表示したい数だけ行列を追加します。この行列が示す位置・向き・大きさで指定したメッシュが描画されます


					break;
				}
			}
		}

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