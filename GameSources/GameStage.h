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
		vector<shared_ptr<Transform>> m_PlayerObject;
		void CreateViewLight(); //ビューの作成
		void CreateGameBox();//GameStageの生成
		void CreatePlayer(); //Playerの生成
		void CreateFixedBox();
		void CreateGimmick();
		void CreateEnemy();
		void InstanceStage();
	public:
		//構築と破棄
		GameStage() :Stage(){}
		virtual ~GameStage() {}
		
		virtual void OnCreate()override; //初期化
		virtual void OnUpdate()override; //初期化
		void GetRay(Vec3& Near, Vec3& Far);

	};

}
//end basecross

