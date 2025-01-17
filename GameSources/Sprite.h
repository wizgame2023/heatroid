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

		bool m_PushState = false;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;
		shared_ptr<GameObject> m_SelectCharge;
		shared_ptr<GameObject> m_TitleCharge;
		float m_fade = 1.0f;

		const float m_fadeSpeed = 2.0f;
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
		float UVCharge;

		SelectCharge(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		virtual ~SelectCharge() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void ChargeUV();
		void UpdateProgress(float load);
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