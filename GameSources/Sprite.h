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

	class BlinkingSprite : public GameObject
	{
		float m_TotalTime;
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
	public:
		BlinkingSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		virtual ~BlinkingSprite() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	class ClearSprite : public GameObject
	{
		float m_TotalTime;
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
		int m_Goal;
		vector<VertexPositionColorTexture> m_vertices;
	public:
		ClearSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		virtual ~ClearSprite() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}