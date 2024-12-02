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
// ロード画面もどき
//====================================================================

	void LoadScreen::CreateViewLight() {
		//ビューのカメラの設定
		auto cameraView = ObjectFactory::Create<SingleView>(GetThis<Stage>());
		auto ptrMyCamera = ObjectFactory::Create<Camera>();
		cameraView->SetCamera(ptrMyCamera);
		ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
		SetView(cameraView);
	}

	void LoadScreen::OnCreate() {
		/*
		auto dev = App::GetApp()->GetDeviceResources();
		auto view = dev->GetDefaultRenderTarget();
		*/

		float key = 0.0f;
		m_progress.push_back(key);
		key = 0.15f;
		m_progress.push_back(key);
		key = 0.35f;
		m_progress.push_back(key);
		key = 0.4f;
		m_progress.push_back(key);
		key = 0.45f;
		m_progress.push_back(key);
		key = .46f;
		m_progress.push_back(key);
		key = 0.75f;
		m_progress.push_back(key);
		key = 0.96f;
		m_progress.push_back(key);
		key = 0.97f;
		m_progress.push_back(key);
		key = 1.0f;
		m_progress.push_back(key);

		CreateViewLight();

		AddGameObject<SpriteLoadBG>();
		m_loadSpr = AddGameObject<SpriteLoad>();
	}
	
	void LoadScreen::OnUpdate() {
		if (m_loadEnd == false) {

			auto delta = App::GetApp()->GetElapsedTime();
			m_time += delta;

			if (m_time > 1.0f) {
				m_progCnt++;
				m_time = 0;
				if (m_progCnt >= m_progress.size() - 1) {
					m_loadEnd = true;
				}
			}
		}

		float load = Lerp::CalculateLerp(m_progress[m_progCnt - 1], m_progress[m_progCnt], 0.0f, 1.0f, m_time, Lerp::Linear);
		m_loadSpr->UpdateProgress(load);
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

		//switch (m_loadState) {
		////フェートイン
		//case 0:
		//	for (auto v : m_Vertices) {
		//		v.color += add;
		//	}
		//	break;
		////フェードアウト
		//case 1:
		//	for (auto v : m_Vertices) {
		//		v.color += -add;
		//	}
		//	break;
		//default:
		//	break;
		//}
	}

//====================================================================
// class SpriteLoad
// ローディングの文字
//====================================================================

	void SpriteLoad::OnCreate() {

		Col4 color(1, 1, 1, 1);

		m_Vertices = {
			{Vec3(0, m_height, 0.0f), color, Vec2(0, 0)},
			{Vec3(m_width, m_height, 0.0f), color, Vec2(1, 0)},
			{Vec3(0, 0, 0.0f), color, Vec2(0, 1)},
			{Vec3(m_width, 0, 0.0f), color, Vec2(1, 1)},
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3,
		};
		m_DrawComp = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
		m_DrawComp->SetTextureResource(L"LoadText");
		m_DrawComp->SetDrawActive(true);
		SetDrawLayer(2);
		SetAlphaActive(true);

		GetComponent<Transform>()->SetPosition(windowWidth * .3, windowHeight * -.4, 0);
	}

	void SpriteLoad::OnUpdate() {
		auto delta = App::GetApp()->GetElapsedTime();
		m_time += delta;

		//float alpha = 0.0f;
		//if (m_time < 1.0f) {
		//	alpha = Lerp::CalculateLerp(0.0f, 1.0f, 0.0f, 1.0f, m_time, Lerp::EaseOut);
		//}
		//else if (m_time >= 1.0f && m_time < 2.0f) {
		//	alpha = Lerp::CalculateLerp(1.0f, 0.0f, 1.0f, 2.0f, m_time, Lerp::Easein);
		//}
		//else {
		//	m_time += -2.0f;
		//}

		//for (auto& v : m_Vertices) {
		//	v.color.z = alpha;
		//}
	}

	void SpriteLoad::UpdateProgress(float load) {
		 float progress = load;
		 m_Vertices[1].position.x = m_width * progress;
		 m_Vertices[3].position.x = m_width * progress;
		 m_Vertices[1].textureCoordinate.x = progress;
		 m_Vertices[3].textureCoordinate.x = progress;

		 GetComponent<PCTSpriteDraw>()->UpdateVertices(m_Vertices);
	}

}