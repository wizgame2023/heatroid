/*!
@file Gimmick.cpp
@brief Gimmickなど
担当：鎌田
*/

#include "stdafx.h"
#include "Project.h"
#include "FixedBox.h"
#include "Gimmick.h"
namespace basecross {


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
		//Coll->SetDrawActive(true);

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
		auto PtrDraw = AddComponent<PNTStaticDraw>();
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
		int Switch,
		int number,
		const wstring& Texname
	) :
		GameObject(StagePtr),
		m_Position(position),
		m_Rotation(rotation),
		m_Scale(scale),
		m_switch(Switch),
		m_number(number),
		m_Texname(Texname)
	{	}

	GimmickButton::~GimmickButton() {}
	//初期化
	void GimmickButton::OnCreate() {
		m_open = false;
		auto Trans = AddComponent<Transform>();
		Trans->SetPosition(m_Position);
		Trans->SetRotation(m_Rotation);
		Trans->SetScale(m_Scale);
		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);
		//Coll->SetDrawActive(true);

		AddTag(L"FixedBox");
		AddTag(L"Floor");
		AddTag(L"GimmickButton");

		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		Mat4x4 meshMat;
		meshMat.affineTransformation(
			Vec3(1.25f, 1.25f, 1.25f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.4f, 0.0f)
		);

		ptrDraw->SetMeshResource(m_Texname);
		ptrDraw->SetMeshToTransformMatrix(meshMat);
		ptrDraw->SetOwnShadowActive(true);

		auto group = GetStage()->GetSharedObjectGroup(L"Switch");
		group->IntoGroup(GetThis<GameObject>());
		//エフェクトの初期化
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring TestEffectStr = DataDir + L"Effects\\Switch.efk";
		auto stageMane = GetStage()->GetSharedGameObject<StageManager>(L"StageManager");
		auto ShEfkInterface = stageMane->GetEfkInterface();
		m_EfkEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, TestEffectStr);


	}
	
	void GimmickButton::OnUpdate()
	{
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		float time = 0;
		if (m_open && m_flag == false)
		{
			PlaySE(L"SwitchSE", 0, 1.0);
			EfectPlay();
			m_flag = true;
			time += ElapsedTime;
			if (time > 5.0f || m_open == false)
			{
				m_flag = false;
			}
		}
	}

	void GimmickButton::PlaySE(wstring path, float loopcnt, float volume) {
		auto playSE = App::GetApp()->GetXAudio2Manager();
		playSE->Start(path, loopcnt, volume);
	}

	void GimmickButton::EfectPlay()
	{
		auto pos = GetComponent<Transform>()->GetPosition();
		auto stageMane = GetStage()->GetSharedGameObject<StageManager>(L"StageManager");
		auto ShEfkInterface = stageMane->GetEfkInterface();
		m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_EfkEffect, pos);
		m_EfkPlay->SetScale(Vec3(2, 2, 2));
		m_EfkPlay->SetAllColor(Col4(0.5f, 0.5f, 0.5f, 1.0f));
	}



	GimmickDoor::GimmickDoor(const shared_ptr<Stage>& StagePtr,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		float UPic,
		float VPic,
		float OpenSwitch,
		int number,
		const wstring& Texname
	) :
		GameObject(StagePtr),
		m_Position(position),
		m_Rotation(rotation),
		m_Scale(scale),
		m_UPic(UPic),
		m_VPic(VPic),
		m_OpenSwitch(OpenSwitch),
		m_number(number),
		m_Texname(Texname)
	{
	}
	GimmickDoor::~GimmickDoor() {}

	//初期化
	void GimmickDoor::OnCreate() {
		m_open = false;
		m_open2 = false;
		m_Flag = false;
		auto Trans = AddComponent<Transform>();
		Trans->SetPosition(m_Position);
		Trans->SetRotation(m_Rotation);
		Trans->SetScale(m_Scale);
		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);
		//Coll->SetDrawActive(true);
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
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetTextureResource(m_Texname);
		//タイリング設定
		PtrDraw->SetSamplerState(SamplerState::PointWrap);

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
							if (m_Flag == false)
							{
								PlaySE(L"DoorSE", 0, 0.5f);
								m_Flag = true;
							}
							if (m_Scale.x < m_Scale.z)
							{
								ptrTransform->SetPosition(Vec3(pos.x, pos.y, pos.z += 0.05f));
							}
							else
							{
								ptrTransform->SetPosition(Vec3(pos.x += 0.05f, pos.y, pos.z));
							}
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
										if (m_Flag == false)
										{
											PlaySE(L"DoorSE", 0, 0.5f);
											m_Flag = true;
										}
										if (m_Scale.x < m_Scale.z)
										{
											ptrTransform->SetPosition(Vec3(pos.x, pos.y, pos.z += 0.05f));
										}
										else
										{
											ptrTransform->SetPosition(Vec3(pos.x += 0.05f, pos.y, pos.z));
										}
									}
								}
								else
								{
									ptrTransform->SetPosition(m_Position);
									m_Flag == false;
								}									
							}
						}
					}

				}
				else
				{
					ptrTransform->SetPosition(m_Position);
					m_Flag == false;
				}

			}
		}
	}

	GimmickUp::GimmickUp(
		const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		float UPic,
		float VPic,
		float m_OpenSwitch,
		int number,
		const wstring& m_Texname,
		float Max) :
		GameObject(stage),
		m_Position(position),
		m_Rotation(rotation),
		m_Scale(scale),
		m_UPic(UPic),
		m_VPic(VPic),
		m_OpenSwitch(m_OpenSwitch),
		m_number(number),
		m_Texname(m_Texname),
		m_Max(Max)
	{
	}
	GimmickUp::~GimmickUp() {}

	//初期化
	void GimmickUp::OnCreate() {
		m_open = false;
		m_open2 = false;
		m_Flag = false;
		auto Trans = AddComponent<Transform>();
		Trans->SetPosition(m_Position);
		Trans->SetRotation(m_Rotation);
		Trans->SetScale(m_Scale);
		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);
		//Coll->SetDrawActive(true);
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
		AddTag(L"GimmickUp");
		AddTag(L"Floor");
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetTextureResource(m_Texname);
		//タイリング設定
		PtrDraw->SetSamplerState(SamplerState::PointWrap);

		auto group = GetStage()->GetSharedObjectGroup(L"GimmickUp");
		group->IntoGroup(GetThis<GameObject>());
	}

	void GimmickUp::OnUpdate()
	{
		OpenDoor();
	}

	void GimmickUp::OpenDoor()
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
						if(pos.y < m_Max)
						{
							ptrTransform->SetPosition(Vec3(pos.x, pos.y += 0.05f, pos.z));
						}
						
					}
					else if (m_number == 2)
					{
						for (auto& v2 : vec) {
							auto shObj2 = v2.lock();
							auto Switchs2 = dynamic_pointer_cast<GimmickButton>(shObj2);
							auto Switch2 = Switchs2->GetSwitch();
							if (Switch2 == m_OpenSwitch && Switchs2 != Switchs)
							{
								m_open2 = Switchs2->GetButton();
								if (m_open2)
								{
									if (pos.y < 10.0f)
									{
										ptrTransform->SetPosition(Vec3(pos.x, pos.y += 0.05f, pos.z));
									}
								}
								else
								{
									ptrTransform->SetPosition(m_Position);
									m_Flag == false;
								}
							}
						}
					}

				}
				else
				{
					ptrTransform->SetPosition(m_Position);
					m_Flag == false;
				}

			}
		}
	}


	void GimmickUp::PlaySE(wstring path, float loopcnt, float volume) {
		auto SE = App::GetApp()->GetXAudio2Manager();
		SE->Start(path, loopcnt, volume);
	}


	void GimmickDoor::PlaySE(wstring path, float loopcnt, float volume) {
			auto SE = App::GetApp()->GetXAudio2Manager();
			SE->Start(path, loopcnt, volume);
	}


	Door::Door(const shared_ptr<Stage>& StagePtr,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		const wstring& Texname
	):
		GameObject(StagePtr),
		m_Position(position),
		m_Rotation(rotation),
		m_Scale(scale),
		m_Texname(Texname)

	{}
	Door::~Door() {}

	void Door::OnCreate()
	{
		m_open = false;
		m_Goaltrue = false;
		auto Trans = AddComponent<Transform>();
		Trans->SetPosition(m_Position);
		Trans->SetRotation(m_Rotation);
		Trans->SetScale(m_Scale);

		auto ptrDraw = AddComponent<PNTBoneModelDraw>();
		Mat4x4 meshMat;
		meshMat.affineTransformation(
			Vec3(.4f, .4f, .4f), //(.1f, .1f, .1f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		ptrDraw->SetMeshResource(m_Texname);
		ptrDraw->SetMeshToTransformMatrix(meshMat);
		AddAnim();
	}
	void Door::OnUpdate()
	{
		auto playerSh = GetStage()->GetSharedGameObject<Player>(L"Player");
		m_Goaltrue = playerSh->GetArrivedGoal();

		if (m_open && !m_Goaltrue)
		{
			SetAnim(L"Open");
		}
		if (m_Goaltrue)
		{
			SetAnim(L"Close");
		}
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(_delta);
	}

	void Door::AddAnim() {
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		auto anim_fps = 30.0f;

		//移動関連
		ptrDraw->AddAnimation(L"Open", 0, 30, false, anim_fps);
		ptrDraw->AddAnimation(L"Close", 60, 90, false, anim_fps);
	}



}