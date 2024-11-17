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
		shared_ptr<Transform> m_PlayerObject;
		shared_ptr<GameObject> m_SpriteDraw;
		shared_ptr<GameObject> m_TextDraw;
		bool m_Goaltrue;
		void CreateViewLight(); //ビューの作成
		void CreateGameBox();//GameStageの生成
		void CreatePlayer(); //Playerの生成
		void CreateFixedBox();
		void CreateGimmick();
		void CreateEnemy();
		void CreateSprite();
		void GoalJudge();
	public:
		//構築と破棄
		GameStage() :Stage(){}
		virtual ~GameStage() {}
		
		virtual void OnCreate()override; //初期化
		virtual void OnUpdate()override; //初期化

	};

}
//end basecross

