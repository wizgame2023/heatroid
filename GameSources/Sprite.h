/*!
@file Sprite.h
@brief スプライト
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class TitleSprite : public GameObject
	{
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
	public:
		TitleSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		virtual ~TitleSprite() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override {}
	};

	class StartA : public GameObject
	{
		float m_TotalTime;
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
	public:
		StartA(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		virtual ~StartA() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}