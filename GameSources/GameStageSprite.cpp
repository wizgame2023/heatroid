/*!
@file GameStageSprite.cpp
@brief ゲームステージのスプライトなど実体
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	GameSprite::GameSprite(const shared_ptr<Stage>& stage,
		float width, float heigth,
		wstring meshName, Vec3 pos,
		Col4 color
	):
		GameObject(stage),
		m_width(width),
		m_height(heigth),
		m_meshName(meshName),
		m_pos(pos),
		m_color(color),
		m_time(1.0f),
		m_layer(1)
	{}
	void GameSprite::OnCreate() {
		m_vertices = {
		{Vec3(-m_width * 0.5f,m_height * 0.5f,0),m_color,Vec2(0.0f,0.0f)},
		{Vec3(m_width * 0.5f,m_height * 0.5f,0),m_color,Vec2(1.0f,0.0f)},
		{Vec3(-m_width * 0.5f,-m_height * 0.5f,0),m_color,Vec2(0.0f,1.0f)},
		{Vec3(m_width * 0.5f,-m_height * 0.5f,0),m_color,Vec2(1.0f,1.0f)}
		};
		m_indices = {
			0,1,2,
			2,1,3
		};
		m_draw = AddComponent<PCTSpriteDraw>(m_vertices, m_indices);
		m_draw->SetTextureResource(m_meshName);
		m_draw->SetSamplerState(SamplerState::LinearWrap);
		m_draw->SetDiffuse(Col4(m_color));
		SetAlphaActive(true);
		m_draw->SetDrawActive(true);

		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);

		SetDrawLayer(m_layer);
	}
	void GameSprite::OnUpdate() {

	}
	void GameSprite::SetColor(Col4 color) {
		m_draw->SetDiffuse(color);
	}
	FadeOut::FadeOut(const shared_ptr<Stage>& stage
	):
		GameObject(stage),
		m_time(0.0f),
		m_touka(0.9f)
	{}
	void FadeOut::OnCreate() {
		auto stage = GetStage();
		m_sprite = stage->AddGameObject<GameSprite>(1280, 800,
			L"White", Vec3(0.0f), Col4(0.0f, 0.0f, 0.0f, 1.0f));

	}
	void FadeOut::OnUpdate() {
		float elpased = App::GetApp()->GetElapsedTime();
		m_time += elpased * 0.5f;
		if (m_time >= m_touka) {
			m_time = m_touka;
		}
		m_sprite->SetColor(Col4(0.0f, 0.0f, 0.0f, m_time));
	}
	bool FadeOut::GetFadeOutEnd() {
		if (m_time >= m_touka) {
			return true;
		}
		else {
			return false;
		}
	}
	void FadeOut::SetColor(Col4 color) {
		m_sprite->SetColor(color);
	}

	FadeIn::FadeIn(const shared_ptr<Stage>& stage
	) :
		GameObject(stage),
		m_time(1.0f)
	{}
	void FadeIn::OnCreate() {
		auto stage = GetStage();
		m_sprite = stage->AddGameObject<GameSprite>(1280, 800,
			L"White", Vec3(0.0f), Col4(1.0f, 1.0f, 1.0f, 1.0f));

	}
	void FadeIn::OnUpdate() {
		float elpased = App::GetApp()->GetElapsedTime();
		m_time -= elpased * 0.5f;
		if (m_time <=0.0f) {
			m_time = 0.0f;
		}
		m_sprite->SetColor(Col4(1.0f, 1.0f, 1.0f, m_time));
	}

	GameOverSprite::GameOverSprite(const shared_ptr<Stage>& stage
	):
		GameObject(stage)
	{}
	void GameOverSprite::OnCreate() {
		auto stage = GetStage();
		m_fadeOut = stage->AddGameObject<FadeOut>();
		m_fadeOut->SetDrawLayer(1);
		m_textSprite = stage->AddGameObject<GameSprite>(1280,800,L"GameOverText",Vec3(0.0f));
		m_textSprite->SetDrawActive(false);
		m_backEffSprite = stage->AddGameObject<GameSprite>(1280, 800, L"GameOverBackEffect", Vec3(0.0f));
		m_backEffSprite->SetDrawActive(false);
		m_backEffSprite->SetColor(Col4(1.0f, 1.0f, 1.0f, 0.5f));
		m_backEffSprite->SetDrawLayer(1);
		
	}
	void GameOverSprite::OnUpdate() {
		if (m_fadeOut->GetFadeOutEnd()) {
			m_textSprite->SetDrawActive(true);
			m_backEffSprite->SetDrawActive(true);
			m_textSprite->SetDrawLayer(2);
			m_fadeOut->SetColor(Col4(0.0f, 0.0f, 0.0f, 0.6f));
		}
	}
}

