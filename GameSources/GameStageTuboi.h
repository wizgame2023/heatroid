/*!
@file GameStageTsuboi.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStageTsuboi : public Stage {
		CsvFile m_GameStage1;
		shared_ptr<SoundItem> m_BGM;
		std::shared_ptr<basecross::XAudio2Manager> m_PtrXA = App::GetApp()->GetXAudio2Manager();

		void PlayBGM(const wstring& StageBGM);

	public:
		//構築と破棄
		GameStageTsuboi() :Stage(){}
		virtual ~GameStageTsuboi() {}

		virtual void OnCreate() override; //初期化
		virtual void OnDestroy() override; //初期化
	};

}
//end basecross

