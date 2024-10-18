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
		void CreateViewLight(); //ビューの作成
		void CreateGameBox();//GameStageの生成

	public:
		//構築と破棄
		GameStageTsuboi() :Stage(){}
		virtual ~GameStageTsuboi() {}
		
		virtual void OnCreate() override; //初期化
	};

}
//end basecross

