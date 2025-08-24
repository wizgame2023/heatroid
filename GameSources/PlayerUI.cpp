
#pragma once
#include "stdafx.h"
#include "PlayerUI.h"

namespace basecross {
	//====================================================================
	// class SpritePlayerUI
	// プレイヤーのゲージ類
	//====================================================================

	void SpritePlayerUI::OnCreate() {
		Col4 color(1, 1, 1, 1);

		m_Vertices = {
			{Vec3(0, 0, 0.0f), color, Vec2(0, 0)},
			{Vec3(m_width, 0, 0.0f), color, Vec2(1, 0)},
			{Vec3(0, -m_height, 0.0f), color, Vec2(0, 1)},
			{Vec3(m_width, -m_height, 0.0f), color, Vec2(1, 1)},
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3,
		};
		m_DrawComp = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
		m_DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
		m_DrawComp->SetTextureResource(m_resKey);
		m_DrawComp->SetDrawActive(true);
		SetDrawLayer(m_layer);
		SetAlphaActive(true);

		GetComponent<Transform>()->SetPosition(windowWidth * -.6, windowHeight * .525, 0);

		if (m_resKey == L"PLAYERUI") {
			m_health = GetStage()->AddGameObject<SpriteHealth>(m_player.lock(), GetThis<SpritePlayerUI>());
			m_charge = GetStage()->AddGameObject<SpriteCharge>(m_player.lock(), GetThis<SpritePlayerUI>());
			m_frame = GetStage()->AddGameObject<SpritePlayerUI>(m_player.lock(), L"PLAYERUI2", 3);
		}
	}

	void SpritePlayerUI::OnUpdate() {

	}

	//====================================================================
	// class PlayerMeterBase
	// プレイヤーのゲージ類の親
	//====================================================================

	void PlayerMeterBase::Init(wstring ResKey) {
		Col4 color(1, 1, 1, 1);

		m_Vertices = {
			{Vec3(0 + m_bottomSlip, 0, 0.0f), color, Vec2(0, 0)},
			{Vec3(m_width + m_bottomSlip, 0, 0.0f), color, Vec2(1, 0)},
			{Vec3(0, -m_height, 0.0f), color, Vec2(0, 1)},
			{Vec3(m_width, -m_height, 0.0f), color, Vec2(1, 1)},
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3,
		};

		m_DrawComp = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
		m_DrawComp->SetDiffuse(color);
		m_DrawComp->SetTextureResource(ResKey);
		m_DrawComp->SetDrawActive(true);
		SetDrawLayer(2);
		SetAlphaActive(true);

		Vec3 pos = m_meter->GetComponent<Transform>()->GetPosition();
		GetComponent<Transform>()->SetPosition(pos + addPos);
	}

	//====================================================================
	// class SpriteHealth
	// プレイヤーのライフ
	//====================================================================

	void SpriteHealth::OnCreate() {
		Col4 color(1, 1, 1, 1);

		m_width = 240.0f;
		m_height = 15.0f;
		m_bottomSlip = -15.0f;

		addPos = Vec3(240.0f, -60.0f, 0.0f);

		Init(L"HEALTH");
	}

	void SpriteHealth::OnUpdate() {
		auto draw = GetComponent<PCTSpriteDraw>();

		auto player = m_player.lock();
		float perc = player->GetHPRate();
		m_Vertices[1].position.x = m_width * perc + m_bottomSlip;
		m_Vertices[3].position.x = m_width * perc;
		m_Vertices[1].textureCoordinate.x = perc;
		m_Vertices[3].textureCoordinate.x = perc;

		draw->UpdateVertices(m_Vertices);
	}
	//====================================================================
	// class SpriteCharge
	// プレイヤーの長押しゲージ
	//====================================================================

	void SpriteCharge::OnCreate() {

		m_width = 240.0f;
		m_height = 13.5f;
		m_bottomSlip = 13.5f;

		addPos = Vec3(287.0f, -82.0f, 0.0f);

		Init(L"CHARGE");
	}

	void SpriteCharge::OnUpdate() {
		auto draw = GetComponent<PCTSpriteDraw>();

		auto player = m_player.lock();
		if (player->IsCharging()) {
			draw->SetDrawActive(true);
			float perc = player->GetChargePerc();
			m_Vertices[1].position.x = (m_width * perc) + m_bottomSlip;
			m_Vertices[3].position.x = (m_width * perc);
			m_Vertices[1].textureCoordinate.x = perc;
			m_Vertices[3].textureCoordinate.x = perc;

			draw->UpdateVertices(m_Vertices);
		}
		else {
			draw->SetDrawActive(false);
		}
	}
}