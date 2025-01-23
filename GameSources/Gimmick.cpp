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
		auto Shadow = AddComponent<Shadowmap>();
		Shadow->SetLightHeight(100.0f);
		Shadow->SetViewWidth(128.0f);
		Shadow->SetViewHeight(128.0f);
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
		auto Shadowm = AddComponent<Shadowmap>();
		//エフェクトの初期化
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring TestEffectStr = DataDir + L"Effects\\Switch.efk";
		wstring TestEffectStrLoop = DataDir + L"Effects\\SwitchLoop.efk";
		auto stageMane = GetStage()->GetSharedGameObject<StageManager>(L"StageManager");
		auto ShEfkInterface = stageMane->GetEfkInterface();
		m_EfkEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, TestEffectStr);
		m_EfkEffectLoop = ObjectFactory::Create<EfkEffect>(ShEfkInterface, TestEffectStrLoop);
	}
	
	void GimmickButton::OnUpdate()
	{
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (m_open && m_flag == false)
		{
			PlaySE(L"SwitchSE", 0, 1.0);
			EfectPlay();
			m_flag = true;
			time += ElapsedTime;
		}
		if (time > 5.0f || m_open == false)
		{
			m_flag = false;
			time = 0.0f;
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
		m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_EfkEffect, pos, 0);
		m_EfkPlay->SetScale(Vec3(2, 2, 2));
		if (m_Texname == L"BLUCKSWITCH")
		{
			m_EfkPlay->SetAllColor(Col4(0.5f, 0.5f, 0.5f, 1.0f));
		}
		else if (m_Texname == L"BLUESWITCH")
		{
			m_EfkPlay->SetAllColor(Col4(0.0f, 1.0f, 1.0f, 0.5f));
		}
		else if (m_Texname == L"REDSWITCH")
		{
			m_EfkPlay->SetAllColor(Col4(1.0f, 0.0f, 0.25f, 0.5f));
		}
	}

	void GimmickButton::EfectLoopPlay()
	{
		auto pos = GetComponent<Transform>()->GetPosition();
		auto stageMane = GetStage()->GetSharedGameObject<StageManager>(L"StageManager");
		auto ShEfkInterface = stageMane->GetEfkInterface();
		m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_EfkEffectLoop, pos, 0);
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

		auto Shadowm = AddComponent<Shadowmap>();
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
						State = 1;
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
									State = 2;
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
									State = 1;
								}									
							}
						}
					}

				}
				else
				{
					ptrTransform->SetPosition(m_Position);
					m_Flag == false;
					State = 0;
				}

			}
		}
	}

	void GimmickDoor::PlaySE(wstring path, float loopcnt, float volume) {
		auto SE = App::GetApp()->GetXAudio2Manager();
		SE->Start(path, loopcnt, volume);
	}

	int GimmickDoor::GetState()
	{
		return State;
	}

	int GimmickDoor::GetNumber()
	{
		return m_OpenSwitch;
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

		auto Shadow = AddComponent<Shadowmap>();
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
						switch (ido)
						{
						case 0:
							ptrTransform->SetPosition(Vec3(pos.x, pos.y += 0.05f, pos.z));
							if (pos.y > m_Max) 
								ido = 1;
							break;
						case 1:
							ptrTransform->SetPosition(Vec3(pos.x, pos.y -= 0.05f, pos.z));
							if (pos.y < 0)ido = 0;
							break;
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
									switch (ido)
									{
									case 0:
										ptrTransform->SetPosition(Vec3(pos.x, pos.y += 0.05f, pos.z));
										if (pos.y > 10.0f) ido = 1;
										break;
									case 1:
										ptrTransform->SetPosition(Vec3(pos.x, pos.y -= 0.05f, pos.z));
										if (pos.y < 0)ido = 0;
										break;
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
			m_animTime += _delta;
			// プレイヤーがエレベータに入ったら閉じる
			if (m_animTime >= 3.5f) {
				SetAnim(L"Close");
			}
		}
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(_delta);
		_delta = App::GetApp()->GetElapsedTime();
	}

	void Door::AddAnim() {
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		auto anim_fps = 30.0f;

		//移動関連
		ptrDraw->AddAnimation(L"Open", 0, 30, false, anim_fps);
		ptrDraw->AddAnimation(L"Close", 60, 90, false, anim_fps);
	}
	DoorGimmick::DoorGimmick(const shared_ptr<Stage>& stage, const Vec3& position, const Vec3& UV, const Vec3& scale, const float& number, const wstring& color):
		GameObject(stage),
		m_Position(position),
		m_UV(UV),
		m_Scale(scale),
		m_number(number),
		color(color)
	{
	}
	void DoorGimmick::OnCreate()
	{

		if (color == L"Black")
		{
			m_colorSwitch = 0;
		}
		if (color == L"Red")
		{
			m_colorSwitch = 1;
		}
		if (color == L"Blue")
		{
			m_colorSwitch = 2;
		}

		float helfSize = 1.0f;
		//頂点配列(縦横5個ずつ表示)
		m_vertices = {
			{Vec3(-m_UV.x * 0.5f, m_UV.y * 0.5f,-m_UV.z * 0.5f), bsm::Vec3(1.0f, 1.0f, 1.0f),Vec2(0.0f,0.0f)},
			{Vec3(m_UV.x * 0.5f, m_UV.y * 0.5f,m_UV.z * 0.5f), bsm::Vec3(1.0f, 1.0f, 1.0f),Vec2(1.0f,0.0f)},
			{Vec3(-m_UV.x * 0.5f,-m_UV.y * 0.5f,-m_UV.z * 0.5f), bsm::Vec3(1.0f, 1.0f, 1.0f),Vec2(0.0f,1.0f)},
			{Vec3(m_UV.x * 0.5f,-m_UV.y * 0.5f,m_UV.z* 0.5f), bsm::Vec3(1.0f, 1.0f, 1.0f),Vec2(1.0f,1.0f)}
		};
		//インデックス配列
	    m_indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_Scale);
		ptrTrans ->SetRotation(Vec3(0.0f, 0.0f, 0.0f));
		ptrTrans->SetPosition(m_Position);
		m_squareMesh = MeshResource::CreateMeshResource<VertexPositionColorTexture>(m_vertices, m_indices, true);

		//頂点とインデックスを指定してスプライト作成
		auto ptrDraw = AddComponent<PCTStaticDraw>();
		ptrDraw->SetMeshResource(m_squareMesh);
		ptrDraw->SetOriginalMeshResource(m_squareMesh);

		if (m_colorSwitch == 0)
		{
			ptrDraw->SetTextureResource(L"BlackDoorGimmick");
		}
		else if (m_colorSwitch == 1)
		{
			ptrDraw->SetTextureResource(L"RedDoorGimmick");
		}
		else if (m_colorSwitch == 1)
		{
			ptrDraw->SetTextureResource(L"BlueDoorGimmick");
		}
	}
	void DoorGimmick::OnUpdate()
	{
		auto group = GetStage()->GetSharedObjectGroup(L"Door");
		auto& vec = group->GetGroupVector();
		for (auto& v : vec) {
			auto shObj = v.lock();
			auto Doors = dynamic_pointer_cast<GimmickDoor>(shObj);
			int state = Doors->GetState();
			float number = Doors->GetNumber();
			int Switch = Doors->m_number;
			if (number == m_number)
			{
				auto ptrDraw = GetComponent<PCTStaticDraw>();
				if (Switch == 1)
				{
					if (m_colorSwitch == 0)
					{
						if (state == 0) {
							ptrDraw->SetTextureResource(L"BlackDoorGimmick3");
						}
						if (state == 1) {
							ptrDraw->SetTextureResource(L"BlackDoorGimmick4");
						}
					}
					else if (m_colorSwitch == 1)
					{
						if (state == 0) {
							ptrDraw->SetTextureResource(L"RedDoorGimmick3");
						}
						if (state == 1) {
							ptrDraw->SetTextureResource(L"RedDoorGimmick4");
						}
					}
					else if (m_colorSwitch == 2)
					{
						if (state == 0) {
							ptrDraw->SetTextureResource(L"BlueDoorGimmick3");
						}
						if (state == 1) {
							ptrDraw->SetTextureResource(L"BlueDoorGimmick4");
						}
					}

				}
				if (Switch == 2)
				{
					if (m_colorSwitch == 0)
					{
						if (state == 0) {
							ptrDraw->SetTextureResource(L"BlackDoorGimmick");
						}
						if (state == 1) {
							ptrDraw->SetTextureResource(L"BlackDoorGimmick1");
						}
						if (state == 2) {
							ptrDraw->SetTextureResource(L"BlackDoorGimmick2");
						}
					}
					else if (m_colorSwitch == 1)
					{
						if (state == 0) {
							ptrDraw->SetTextureResource(L"RedDoorGimmick");
						}
						if (state == 1) {
							ptrDraw->SetTextureResource(L"RedDoorGimmick1");
						}
						if (state == 2) {
							ptrDraw->SetTextureResource(L"RedDoorGimmick2");
						}
					}
					else if (m_colorSwitch == 2)
					{
						if (state == 0) {
							ptrDraw->SetTextureResource(L"BlueDoorGimmick");
						}
						if (state == 1) {
							ptrDraw->SetTextureResource(L"BlueDoorGimmick1");
						}
						if (state == 2) {
							ptrDraw->SetTextureResource(L"BlueDoorGimmick2");
						}
					}
				}
			}
		}
	}
}