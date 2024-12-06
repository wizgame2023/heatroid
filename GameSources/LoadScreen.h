/*!
@file LoadScreen.h
@brief ロード中にChildStageとして表示する画面
@author 坪井優弥
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class LoadScreen;
	class SpriteLoadBG;
	class SpriteLoad;
	class SpriteLoadCircle;
	class SpriteLoadFade;

	//====================================================================
	// class LoadScreen
	// ロード画面もどき
	//====================================================================
	class LoadScreen : public Stage
	{
		//ロード終了
		bool m_loadEnd;
		//経過時間
		float m_time;
		//進行度合いを管理するためのvector
		vector<float> m_progress;
		//何番目まで進んでいるかのカウント
		int m_progCnt = 1;

		const float loadLength = .05f;

		shared_ptr<SpriteLoad> m_loadSpr;
		shared_ptr<SpriteLoadFade> m_fade;

	public:
		LoadScreen(): Stage() {}
		virtual ~LoadScreen() {}

		void CreateViewLight();
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
		SpriteLoadBG(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr)
		{}

		~SpriteLoadBG() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;
	};

	//====================================================================
	// class SpriteLoadCircle
	// ローディング画面のぐるぐるするやつ
	//====================================================================
	class SpriteLoadCircle : public GameObject {
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		const float m_rotateSpeed = -360.0f;
		const float m_radius = 50.0f;
		float m_rotate = 0.0f;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpriteLoadCircle(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr)
		{}

		~SpriteLoadCircle() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;
	};



	//====================================================================
	// class SpriteLoad
	// ローディングの文字
	//====================================================================
	class SpriteLoad : public GameObject {
		float m_time;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpriteLoad(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr)
		{}

		~SpriteLoad() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;

		//ここに外部から進行度の見た目を変えられる関数を作る
		void UpdateProgress(float load);
	};

	//====================================================================
	// class SpriteLoadFade
	// ローディング画面の画面遷移
	//====================================================================
	class SpriteLoadFade : public GameObject {
		bool m_loadState = false;
		float m_fade = 1.0f;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		const float m_fadeSpeed = 2.0f;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpriteLoadFade(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr)
		{}

		~SpriteLoadFade() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void SetLoadState(int state);
	};
}