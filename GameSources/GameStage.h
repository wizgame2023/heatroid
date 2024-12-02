/*!
鎌田大輝
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		CsvFile m_GameStage1;
		shared_ptr<SoundItem> m_BGM;
		std::shared_ptr<basecross::XAudio2Manager> m_ptrXA = App::GetApp()->GetXAudio2Manager();
		bool m_pause = false;
		shared_ptr<GameObject> m_fadeIn;
		shared_ptr<GameObject> m_fadeOut;

		shared_ptr<GameObject> m_pauseBackGround;
		shared_ptr<GameObject> m_PauseSelect;
		shared_ptr<GameObject> m_PauseTitle;
		shared_ptr<GameObject> m_PauseBack;

	public:
		//構築と破棄
		GameStage() :Stage(){}
		virtual ~GameStage() {}
		
		virtual void OnCreate()override; //初期化
		virtual void OnUpdate()override; //初期化

		void PlayBGM(const wstring& StageBGM);

		void OnDestroy();

		void CreateStageManager();

		void GamePause();
	};

}
//end basecross

