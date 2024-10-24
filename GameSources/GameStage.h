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
		void CreateViewLight(); //ビューの作成
		void CreateGameBox();//GameStageの生成
		void CreatePlayer(); //Playerの生成
		void CreateGameStage();
		void CreateFixedBox();
	public:
		//構築と破棄
		GameStage() :Stage(){}
		virtual ~GameStage() {}
		
		virtual void OnCreate()override; //初期化
	};

}
//end basecross

