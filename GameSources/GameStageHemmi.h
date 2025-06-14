/*!
@file GameStageHemmi.h
@brief テストゲームステージ
担当：逸見
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStageHemmi : public Stage {

		shared_ptr<SoundItem> m_BGM;
		std::shared_ptr<basecross::XAudio2Manager> m_PtrXA = App::GetApp()->GetXAudio2Manager();
		//CsvFile m_GameStage1;
		//void CreateViewLight(); //ビューの作成
		//void CreateGameBox();//GameStageの生成
		//void CreatePlayer(); //Playerの生成
		//void CreateFixedBox();
		//void CreateGimmick();
		//void CreateEnemy();
		//void InstanceStage();
		//void ToMainCamera();
	public:
		//構築と破棄
		GameStageHemmi() :Stage(){}
		virtual ~GameStageHemmi() {}
		virtual void OnCreate()override; //初期化
		
		void PlayBGM(const wstring& StageBGM);
		void CreateStageManager();
		void CreateEnemy();
		void OnDestroy();
	};

}
//end basecross

