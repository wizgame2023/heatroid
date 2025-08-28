#pragma once
/*!
@file PlayerUI.h
@brief プレイヤーのHP・チャージUI
*/

#pragma once
#include "stdafx.h"
#include "Player.h"

namespace basecross {

	//====================================================================
	// class SpritePlayerUI
	// プレイヤーのゲージ類
	//====================================================================
	class SpriteHealth;
	class SpriteCharge;

	class SpritePlayerUI : public GameObject {
		wstring m_resKey;
		int m_layer;
		weak_ptr<Player> m_player;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		shared_ptr<SpriteHealth> m_health;
		shared_ptr<SpriteCharge> m_charge;
		shared_ptr<SpritePlayerUI> m_frame;

		const float m_width = 600.0f;
		const float m_height = 150.0f;
		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpritePlayerUI(const shared_ptr<Stage>& StagePtr, const shared_ptr<Player>& player, const wstring& resKey, const int& layer = 1) :
			GameObject(StagePtr),
			m_player(player),
			m_resKey(resKey),
			m_layer(layer)
		{
		}

		~SpritePlayerUI() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};


	//====================================================================
	// class PlayerMeterBase
	// プレイヤーのゲージ類の親
	//====================================================================
	class PlayerMeterBase : public GameObject {
	protected:
		weak_ptr<Player> m_player;
		shared_ptr<SpritePlayerUI> m_meter;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		float m_width;
		float m_height;
		float m_bottomSlip;
		Vec3 addPos;
	public:
		PlayerMeterBase(const shared_ptr<Stage>& StagePtr, const shared_ptr<Player>& player, const shared_ptr<SpritePlayerUI>& meter) :
			GameObject(StagePtr),
			m_player(player),
			m_meter(meter)
		{
		}
		void Init(wstring ResKey);
	};

	//====================================================================
	// class SpriteHealth
	// プレイヤーのライフ
	//====================================================================
	class SpriteHealth : public PlayerMeterBase {

	public:
		SpriteHealth(const shared_ptr<Stage>& StagePtr, const shared_ptr<Player>& player, const shared_ptr<SpritePlayerUI>& meter) :
			PlayerMeterBase(StagePtr, player, meter)
		{
		}

		~SpriteHealth() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;
	};

	//====================================================================
	// class SpriteCharge
	// プレイヤーの長押しゲージ
	//====================================================================
	class SpriteCharge : public PlayerMeterBase {

	public:
		SpriteCharge(const shared_ptr<Stage>& StagePtr, const shared_ptr<Player>& player, const shared_ptr<SpritePlayerUI>& meter) :
			PlayerMeterBase(StagePtr, player, meter)
		{
		}

		~SpriteCharge() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;
	};

}