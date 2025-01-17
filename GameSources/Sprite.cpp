/*!
@file Sprite.cpp
@brief �X�v���C�g����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	Sprite::Sprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos)
	{}

	void Sprite::OnCreate() {
		float helfSize = 1.0f;
		//���_�z��(�c��5���\��)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-helfSize, helfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-helfSize, -helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, -helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_StartScale.x, m_StartScale.y, 0.2f);
		ptrTrans->SetRotation(0, 0, 0);
		ptrTrans->SetPosition(m_StartPos);
		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto ptrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetTextureResource(m_TextureKey);
	}

	SelectCharge::SelectCharge(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos)
	{}

	void SelectCharge::OnCreate() {
		UVCharge = 1.0f;
		float helfSize = 1.0f;
		//���_�z��(�c��5���\��)
		m_Vertices = {
			{ VertexPositionColorTexture(Vec3(-helfSize, helfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-helfSize, -helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, UVCharge)) },
			{ VertexPositionColorTexture(Vec3(helfSize, -helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, UVCharge)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_StartScale.x, m_StartScale.y, 0.2f);
		ptrTrans->SetRotation(0, 0, 0);
		ptrTrans->SetPosition(m_StartPos);
		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto ptrDraw = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
		ptrDraw->SetBlendState(BlendState::AlphaBlend);
		ptrDraw->SetTextureResource(m_TextureKey);

	}
	void SelectCharge::OnUpdate()
	{
		ChargeUV();
	}
	void SelectCharge::ChargeUV()
	{
		auto time = App::GetApp()->GetElapsedTime();
		auto stageMane = GetStage()->GetSharedGameObject<StageManager>(L"StageManager");
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		m_SelectCharge = stageMane->m_SelectCharge;
		m_TitleCharge = stageMane->m_TitleCharge;
		m_PushState = stageMane->GetPushState();

		if (m_PushState == 0) {
			m_fade -= time * m_fadeSpeed;
			for (auto& vtx : m_Vertices) {
				vtx.color = Col4(0.0f, 0.0f, 0.0f, 1.0f);
			}
		}
		else{
			m_fade += time * m_fadeSpeed;
			for (auto& vtx : m_Vertices) {
				vtx.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		if (m_fade > 1.0f) m_fade = 1.0f;
		if (m_fade < 0.0f) m_fade = 0.0f;
		if (m_PushState == 1)
		{
			m_SelectCharge->GetComponent<PCTSpriteDraw>()->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, m_fade));
			m_SelectCharge->GetComponent<PCTSpriteDraw>()->UpdateVertices(m_Vertices);
		}
		if (m_PushState == 2)
		{
			m_TitleCharge->GetComponent<PCTSpriteDraw>()->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, m_fade));
			m_TitleCharge->GetComponent<PCTSpriteDraw>()->UpdateVertices(m_Vertices);
		}
	}

	void SelectCharge::UpdateProgress(float load) {
		//float progress = load;
		//m_Vertices[1].position.x = windowWidth * (progress - .5f);
		//m_Vertices[3].position.x = windowWidth * (progress - .5f);
		//m_Vertices[1].textureCoordinate.x = progress;
		//m_Vertices[3].textureCoordinate.x = progress;

		//GetComponent<PCTSpriteDraw>()->UpdateVertices(m_Vertices);
	}


	void SelectCharge::SetPushState(const bool PushState)
	{
		m_PushState = PushState;
	}

	BlinkingSprite::BlinkingSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec3& StartPos, const float& TransSpeed) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TransSpeed(TransSpeed)
	{}

	void BlinkingSprite::OnCreate() {
		float helfSize = 1.0f;
		//���_�z��(�c��5���\��)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-helfSize, helfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-helfSize, -helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, -helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_StartScale.x, m_StartScale.y, 0.2f);
		ptrTrans->SetRotation(0, 0, 0);
		ptrTrans->SetPosition(m_StartPos);
		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto ptrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetTextureResource(m_TextureKey);
		m_start = false;
	}

	void BlinkingSprite::OnUpdate()
	{
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();

		if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A || KeyState.m_bPressedKeyTbl[VK_SPACE])
		{
			m_start = true;
		}
		if (m_start)
		{
			m_TotalTime += ElapsedTime * 30.0f;
		}
		else {
			m_TotalTime += ElapsedTime * 3.5f;
		}
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0.0f;
		}
		auto PtrDraw = GetComponent<PCTSpriteDraw>();
		Col4 col(1.0, 1.0, 1.0, 1.0);
		col.w = sin(m_TotalTime) * 0.5f + 0.5f;
		PtrDraw->SetDiffuse(col);

	}

}