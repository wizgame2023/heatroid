/*!
@file LoadScreen.cpp
@brief キャラクターなど実体
@author 坪井優弥
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

//====================================================================
// class LoadScreen
// ロード時に表示するトランジション画面
//====================================================================
	LoadScreen::LoadScreen(): 
		Stage()
	{
	}

	void LoadScreen::OnCreate() {
		auto a = AddGameObject<StageManager>();
		a->CreateViewLight();
		m_loadState = App::GetApp()->GetScene<Scene>()->GetLoadState();

		AddGameObject<SpriteLoadBG>(m_loadState);
		AddGameObject<SpriteLoad>(m_loadState);
	}
	
	void LoadScreen::OnUpdate() {

	}

//====================================================================
// class SpriteLoadBG
// ローディング画面背景
//====================================================================

	void SpriteLoadBG::OnCreate() {

		Col4 color(1, 1, 1, 0);
		if (m_loadState == 1) color.z = 1.0f;

		float halfW = windowWidth * .5f;
		float halfH = windowHeight * .5f;

		m_Vertices = {
			{Vec3(-halfW, -halfH, 0.0f), color, Vec2(0, 0)},
			{Vec3(halfW, -halfH, 0.0f), color, Vec2(1, 0)},
			{Vec3(-halfW, halfH, 0.0f), color, Vec2(0, 1)},
			{Vec3(halfW, halfH, 0.0f), color, Vec2(1, 1)},
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3,
		};
		m_DrawComp = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
		m_DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
		m_DrawComp->SetTextureResource(L"LoadBG");
		m_DrawComp->SetDrawActive(true);
		SetDrawLayer(1);
		SetAlphaActive(true);

		GetComponent<Transform>()->SetPosition(0, 0, 1.0f);
	}

	void SpriteLoadBG::OnUpdate() {
		auto delta = App::GetApp()->GetElapsedTime();
		Col4 add(0, 0, 0, delta);

		switch (m_loadState) {
		//フェートイン
		case 0:
			for (auto v : m_Vertices) {
				v.color += add;
			}
			break;
		//フェードアウト
		case 1:
			for (auto v : m_Vertices) {
				v.color += -add;
			}
			break;
		default:
			break;
		}
	}

//====================================================================
// class SpriteLoad
// ローディングの文字
//====================================================================

	void SpriteLoad::OnCreate() {

		Col4 color(1, 1, 1, 0);
		if (m_loadState == 1) color.z = 1.0f;

		m_Vertices = {
			{Vec3(0, 0, 0.0f), color, Vec2(0, 0)},
			{Vec3(m_width, 0, 0.0f), color, Vec2(1, 0)},
			{Vec3(0, m_height, 0.0f), color, Vec2(0, 1)},
			{Vec3(m_width, m_height, 0.0f), color, Vec2(1, 1)},
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3,
		};
		m_DrawComp = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
		m_DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
		m_DrawComp->SetTextureResource(L"LoadText");
		m_DrawComp->SetDrawActive(true);
		SetDrawLayer(1);
		SetAlphaActive(true);

		GetComponent<Transform>()->SetPosition(windowWidth * .45, windowHeight * -.45, 0);
	}

	void SpriteLoad::OnUpdate() {
		auto delta = App::GetApp()->GetElapsedTime();
		Col4 add(0, 0, 0, delta);

		switch (m_loadState) {
			//フェートイン
		case 0:
			for (auto v : m_Vertices) {
				v.color += add;
			}
			break;
			//フェードアウト
		case 1:
			for (auto v : m_Vertices) {
				v.color += -add;
			}
			break;
		default:
			break;
		}
	}
}