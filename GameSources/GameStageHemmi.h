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
		void CreateViewLight(); //ビューの作成
		void CreateGameBox();//GameStageの生成
		void CreateEnemy();
		void CreateBreakWall();
	public:
		//構築と破棄
		GameStageHemmi() :Stage(){}
		virtual ~GameStageHemmi() {}
		
		virtual void OnCreate()override; //初期化
	};

}
//end basecross

