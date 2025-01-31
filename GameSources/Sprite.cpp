/*!
@file Sprite.cpp
@brief スプライト実体
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
		//頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-helfSize, helfSize,  0),Col4(1.0f,1.0f,1.0f,1.0f),      Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, helfSize,   0), Col4(1.0f, 1.0f, 1.0f, 1.0f),   Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-helfSize, -helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, -helfSize,  0), Col4(1.0f, 1.0f, 1.0f, 1.0f),  Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_StartScale.x, m_StartScale.y, 0.2f);
		ptrTrans->SetRotation(0, 0, 0);
		ptrTrans->SetPosition(m_StartPos);
		//頂点とインデックスを指定してスプライト作成
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

		Col4 color(1, 1, 1, 1);

		float halfW = 1.0f;
		float halfH = 1.0f;
		//頂点配列(縦横5個ずつ表示)
		m_Vertices = {
			{Vec3(-halfW, halfH, 0.0f), color,Vec2(0.0f, 0.0f)},
			{Vec3(halfW, halfH,   0.0f), color, Vec2(1.0f, 0.0f)},
			{Vec3(-halfW, -halfH, 0.0f), color, Vec2(0.0f, 1.0f)},
			{Vec3(halfW, -halfH, 0.0f), color,	Vec2(1.0f, 1.0f)},
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_StartScale.x, m_StartScale.y, 0.2f);
		ptrTrans->SetRotation(0, 0, 0);
		ptrTrans->SetPosition(m_StartPos);
		//頂点とインデックスを指定してスプライト作成
		auto ptrDraw = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
		ptrDraw->SetTextureResource(m_TextureKey);

		auto group = GetStage()->GetSharedObjectGroup(L"Sprite");
		group->IntoGroup(GetThis<GameObject>());
	}
	void SelectCharge::OnUpdate()
	{
	}
	void SelectCharge::ChargeUV(const float& time)
	{
		float alpha = 1.0f;
		if (time < 1.0f) {
			//float load = Lerp::CalculateLerp(0.0f,1.0f, 0.0f, 1.0f, time, Lerp::Linear);
			UpdateProgress(time);
		}

		//for (auto& v : m_Vertices) {
		//	v.color.z = alpha;
		//}
	}

	void SelectCharge::UpdateProgress(float load) {
		float progress =  load;
		m_Vertices[1].position.x = (progress * 2) - 1.0f;
		m_Vertices[3].position.x = (progress * 2) - 1.0f;
		m_Vertices[1].textureCoordinate.x = progress;
		m_Vertices[3].textureCoordinate.x = progress;

		GetComponent<PCTSpriteDraw>()->UpdateVertices(m_Vertices);
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
		//頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-helfSize, helfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-helfSize, -helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, -helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_StartScale.x, m_StartScale.y, 0.2f);
		ptrTrans->SetRotation(0, 0, 0);
		ptrTrans->SetPosition(m_StartPos);
		//頂点とインデックスを指定してスプライト作成
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