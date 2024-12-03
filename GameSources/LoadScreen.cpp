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
		AddGameObject<SpriteLoadCircle>();
		m_fade = AddGameObject<SpriteLoadFade>();

	}

	void LoadScreen::OnUpdate() {
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();

		if (m_loadEnd == false) {

			auto delta = App::GetApp()->GetElapsedTime();
			m_time += delta;

			if (m_time > 1.0f) {
				m_progCnt++;
				m_time = 0;
				if (m_progCnt >= m_progress.size()) {
					m_loadEnd = true;
					m_fade->SetLoadState(m_loadEnd);
				}
			}
		}
		else {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
		}
		if (m_progCnt < m_progress.size()) {
			float load = Lerp::CalculateLerp(m_progress[m_progCnt - 1], m_progress[m_progCnt], 0.0f, 1.0f, m_time, Lerp::Linear);
			m_loadSpr->UpdateProgress(load);
		}

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
			{Vec3(-halfW, -halfH, 0.0f), color, Vec2(0, 1)},
			{Vec3(halfW, -halfH, 0.0f), color, Vec2(1, 1)},
			{Vec3(-halfW, halfH, 0.0f), color, Vec2(0, 0)},
			{Vec3(halfW, halfH, 0.0f), color, Vec2(1, 0)},
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

	}

	//====================================================================
	// class SpriteLoadCircle
	// ローディング画面のぐるぐるするやつ
	//====================================================================

	void SpriteLoadCircle::OnCreate() {

		Col4 color(1, 1, 1, 1);

		float width = sinf(XMConvertToRadians(45.0f)) * m_radius;

		m_Vertices = {
			{Vec3(-width, width, 0.0f), color, Vec2(0, 1)},
			{Vec3(width, width, 0.0f), color, Vec2(1, 1)},
			{Vec3(width, -width, 0.0f), color, Vec2(1, 0)},
			{Vec3(-width, -width, 0.0f), color, Vec2(0, 0)},
		};
		vector<uint16_t> indices = {
			0, 1, 3,
			3, 1, 2,
		};
		m_DrawComp = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
		m_DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
		m_DrawComp->SetTextureResource(L"LoadCircle");
		m_DrawComp->SetDrawActive(true);
		SetDrawLayer(3);
		SetAlphaActive(true);

		GetComponent<Transform>()->SetPosition(windowWidth * .44, windowHeight * -.41, 0);
	}

	void SpriteLoadCircle::OnUpdate() {
		auto delta = App::GetApp()->GetElapsedTime();
		m_rotate += delta * m_rotateSpeed;

		int i = 0;
		for (auto& vtx : m_Vertices) {
			vtx.position = Vec3(cosf(XMConvertToRadians(45.0f + m_rotate + (i * 90.0))) * m_radius, sinf(XMConvertToRadians(45.0f + m_rotate + (i * 90.0))) * m_radius, 0);
			i++;
		}
		GetComponent<PCTSpriteDraw>()->UpdateVertices(m_Vertices);
	}

	//====================================================================
	// class SpriteLoad
	// ローディングの文字
	//====================================================================

	void SpriteLoad::OnCreate() {

		Col4 color(1, 1, 1, 1);

		float halfW = windowWidth * .5f;
		float halfH = windowHeight * .5f;

		m_Vertices = {
			{Vec3(-halfW, -halfH, 0.0f), color, Vec2(0, 1)},
			{Vec3(halfW, -halfH, 0.0f), color, Vec2(1, 1)},
			{Vec3(-halfW, halfH, 0.0f), color, Vec2(0, 0)},
			{Vec3(halfW, halfH, 0.0f), color, Vec2(1, 0)},
		};

		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3,
		};
		m_DrawComp = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
		m_DrawComp->SetTextureResource(L"LoadBar");
		m_DrawComp->SetDrawActive(true);
		SetDrawLayer(3);
		SetAlphaActive(true);

		GetComponent<Transform>()->SetPosition(0, 0, 1.0f);
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
		m_Vertices[1].position.x = windowWidth * (progress - .5f);
		m_Vertices[3].position.x = windowWidth * (progress - .5f);
		m_Vertices[1].textureCoordinate.x = progress;
		m_Vertices[3].textureCoordinate.x = progress;

		GetComponent<PCTSpriteDraw>()->UpdateVertices(m_Vertices);
	}

	//====================================================================
	// class SpriteLoadFade
	// ローディング画面の画面遷移
	//====================================================================

	void SpriteLoadFade::OnCreate() {

		Col4 color(1.0f, 1.0f, 1.0f, 1.0f);

		float halfW = windowWidth * .5f;
		float halfH = windowHeight * .5f;

		m_Vertices = {
			{Vec3(-halfW, -halfH, 0.0f), color, Vec2(0, 1)},
			{Vec3(halfW, -halfH, 0.0f), color, Vec2(1, 1)},
			{Vec3(-halfW, halfH, 0.0f), color, Vec2(0, 0)},
			{Vec3(halfW, halfH, 0.0f), color, Vec2(1, 0)},
		};

		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3,
		};
		m_DrawComp = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
		m_DrawComp->SetDrawActive(true);
		m_DrawComp->SetTextureResource(L"White");
		SetDrawLayer(4);
		SetAlphaActive(true);
		m_DrawComp->SetBlendState(BlendState::AlphaBlend);

		GetComponent<Transform>()->SetPosition(0, 0, 1.0f);
	}

	void SpriteLoadFade::OnUpdate() {
		auto delta = App::GetApp()->GetElapsedTime();

		if (!m_loadState) {
			m_fade -= delta * m_fadeSpeed;
			for (auto& vtx : m_Vertices) {
				vtx.color = Col4(0.0f, 0.0f, 0.0f, 1.0f);
			}
		}
		if (m_loadState) {
			m_fade += delta * m_fadeSpeed;
			for (auto& vtx : m_Vertices) {
				vtx.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		if (m_fade > 1.0f) m_fade = 1.0f;
		if (m_fade < 0.0f) m_fade = 0.0f;
		GetComponent<PCTSpriteDraw>()->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, m_fade));
		GetComponent<PCTSpriteDraw>()->UpdateVertices(m_Vertices);
	}

	void SpriteLoadFade::SetLoadState(int state) {
		m_loadState = state;
	}
}