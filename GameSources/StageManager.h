/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class StageManager : public  GameObject
	{
	public:
		CsvFile m_GameStage;
		shared_ptr<Transform> m_PlayerObject;
		shared_ptr<GameObject> m_SpriteDraw;
		shared_ptr<GameObject> m_TextDraw;
		bool m_Goaltrue = false;
		bool m_Diedtrue = false;
		bool m_Flag = true;
		void CreateViewLight(); //ビューの作成
		void CreatePlayer(Vec3& pos , Vec3& rot, Vec3& scale); //Playerの生成
		void CreateFixedBox();
		void CreateGimmick();
		void CreateEnemy();
		void CreateSprite();
		void GoalJudge();
		void GameOverJudge();
		//構築と破棄
		StageManager(const shared_ptr<Stage>& stage) :
			GameObject(stage)
		{
		}
		virtual ~StageManager() {}

		virtual void OnCreate()override; //初期化
		virtual void OnUpdate()override; //初期化

	};
}