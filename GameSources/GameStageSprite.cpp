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
		Col4 color, int layer
	):
		GameObject(stage),
		m_width(width),
		m_height(heigth),
		m_meshName(meshName),
		m_pos(pos),
		m_color(color),
		m_time(1.0f),
		m_layer(layer)
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

	GoalFade::GoalFade(const shared_ptr<Stage>& stage
	) :
		GameObject(stage),
		m_time(0.0f)
	{}
	void GoalFade::OnCreate() {
		auto stage = GetStage();
		m_sprite = stage->AddGameObject<GameSprite>(1280, 800,
			L"White", Vec3(0.0f), Col4(1.0f));

	}
	void GoalFade::OnUpdate() {
		float delta = App::GetApp()->GetElapsedTime();
		m_time += m_time >= m_fadeinTime ? delta * 2.0f : delta;

		float trans = m_time >= m_fadeinTime ? m_endTime - m_time : m_time;

		m_sprite->SetColor(Col4(1.0f, 1.0f, 1.0f, trans));
	}

	FadeIn::FadeIn(const shared_ptr<Stage>& stage
	) :
		GameObject(stage),
		m_time(1.0f)
	{}
	void FadeIn::OnCreate() {
		auto stage = GetStage();
		m_sprite = stage->AddGameObject<GameSprite>(1280, 800,
			L"White", Vec3(0.0f), Col4(1.0f, 1.0f, 1.0f, 1.0f), 10);

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

	NumberSprite::NumberSprite(const shared_ptr<Stage>& stage,
		const int& number,
		const Vec3& pos
	):
		GameObject(stage),
		m_width(70),
		m_height(140),
		m_moveW((800.0f/10)/800.0f),
		m_moveH(0.0f),
		m_meshName(L"SelectNumber"),
		m_color(Col4(1.0f)),
		m_number(number - 1),
		m_pos(pos)
	{}
	void NumberSprite::OnCreate() {
		m_vertices = {
			{Vec3(0.0f,0.0f,0.0f),m_color,Vec2((m_moveW * m_number),0.0f)},
			{Vec3(m_width,0.0f,0.0f),m_color,Vec2((m_moveW * (m_number + 1)),0.0f)},
			{Vec3(0.0f,-m_height,0.0f),m_color,Vec2((m_moveW * m_number),1.0f)},
			{Vec3(m_width,-m_height,0.0f),m_color,Vec2((m_moveW * (m_number + 1)),1.0f)},
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
		SetDrawLayer(3);

		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);

	}
	void NumberSprite::OnUpdate() {
		
	}
	void NumberSprite::UpdateVar(int num) {
		m_vertices[0].textureCoordinate.x = m_moveW * num;
		m_vertices[1].textureCoordinate.x = m_moveW * (num + 1);
		m_vertices[2].textureCoordinate.x = m_moveW * num;
		m_vertices[3].textureCoordinate.x = m_moveW * (num + 1);

		m_draw->UpdateVertices(m_vertices);
	}
}

