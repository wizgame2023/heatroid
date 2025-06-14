/*!
@file Sprite.h
@brief スプライト
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Sprite : public GameObject
	{
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
	public:
		Sprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		virtual ~Sprite() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override {}
	};

	class SelectCharge : public GameObject
	{
	public:

		bool m_pushState = false;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;
		shared_ptr<GameObject> m_selectCharge;
		shared_ptr<GameObject> m_titleCharge;
		float m_fade = 1.0f;

		const float m_fadeSpeed = 2.0f;
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
		float UVCharge;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();

		SelectCharge(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		virtual ~SelectCharge() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void ChargeUV(const float& time);
		void UpdateProgress(float time);
		void SetPushState(const bool PushState);
	};

	class BlinkingSprite : public GameObject
	{
		float m_TotalTime;
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
		bool m_start;
	public:	
		float m_TransSpeed;

		BlinkingSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos, const float& m_TransSpeed);
		virtual ~BlinkingSprite() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

}