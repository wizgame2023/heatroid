/*!
@file LoadScreen.h
@brief ロード中にChildStageとして表示する画面
@author 坪井優弥
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//====================================================================
	// class LoadScreen
	// ロード時に表示するトランジション画面
	//====================================================================
	class LoadScreen : public Stage
	{
		//0 = ロード開始、1 = ロード終了
		int m_loadState;
	public:
		LoadScreen();
		virtual ~LoadScreen() {}

		virtual void OnCreate()override; //初期化
		virtual void OnUpdate()override; //初期化
	};

	//====================================================================
	// class SpriteLoadBG
	// ローディング画面背景
	//====================================================================
	class SpriteLoadBG : public GameObject {
		int m_loadState;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpriteLoadBG(const shared_ptr<Stage>& StagePtr, int loadState) :
			GameObject(StagePtr),
			m_loadState(loadState)
		{}

		~SpriteLoadBG() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;
	};

	//====================================================================
	// class SpriteLoad
	// ローディングの文字
	//====================================================================
	class SpriteLoad : public GameObject {
		int m_loadState;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		const float m_width = 256.0f;
		const float m_height = 128.0f;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpriteLoad(const shared_ptr<Stage>& StagePtr, int loadState) :
			GameObject(StagePtr),
			m_loadState(loadState)
		{}

		~SpriteLoad() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;
	};
}