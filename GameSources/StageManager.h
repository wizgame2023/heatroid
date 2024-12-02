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
		enum CameraSelect {
			openingCamera,
			myCamera,
		};

		enum GameStatus {
			// タイトルシーン表示時
			TITLE,
			// ステージセレクトシーン表示時
			SELECT,
			// ゲームが開始してから終了するまで
			GAME_PLAYING,

			TEST_PLAY
		};
		CameraSelect m_CameraSelect;
		shared_ptr<SingleView> m_OpeningCameraView;
		shared_ptr<SingleView> m_MyCameraView;
		int m_nowGameStatus;
		wstring m_StageName;
		CsvFile m_GameStage;
		//スプライト
		shared_ptr<Transform> m_PlayerObject;
		shared_ptr<GameObject> m_SpriteDraw;
		shared_ptr<GameObject> m_TextDraw;
		shared_ptr<GameObject> m_StageUI;
		shared_ptr<GameObject> m_nextStageUI;
		shared_ptr<GameObject> m_clearSelectStage;
		shared_ptr<GameObject> m_retryStageUI;
		shared_ptr<GameObject> m_overSelectStage;
		float m_totalTime = 0.0f;
		int m_select = 0;

		bool m_Goaltrue = false;
		bool m_Diedtrue = false;
		bool m_pause = false;
		bool m_Flag = true;

		void CreateViewLight(); //ビューの作成
		void CreatePlayer(); //Playerの生成
		void CreateFixedBox();
		void CreateGimmick();
		void CreateEnemy();
		void CreateSprite();
		void GoalJudge();
		void GameOverJudge();
		void GamePause();
		void SetGameStageSelect(const wstring& m_csvFail);
		void MoveSprite(const shared_ptr<GameObject> nextStageUI, const shared_ptr<GameObject> SelectStage);
		void SelectMoveSprite(const shared_ptr<GameObject> nextStageUI, const shared_ptr<GameObject> SelectStage);
		wstring GetGameStageSelect();
		int GetNowGameStatus();
		void SetNowGameStatus(int afterGameStatus);
		int GetNowCameraStatus();
		void ToMainCamera();
		void ToOpeningCamera();

		void PlaySE(wstring path, float loopcnt, float volume);
		void PlaySE2(wstring path, float loopcnt, float volume);
		
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