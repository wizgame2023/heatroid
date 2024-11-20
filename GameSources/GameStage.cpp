/*!
鎌田大輝
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"
#include "GameStage.h"

namespace basecross {

	void GameStage::OnCreate() {
		try {
			CreateStageManager();
			auto player = GetSharedGameObject<Player>(L"Player");
			AddGameObject<SpriteHealth>(player);
			AddGameObject<SpriteCharge>(player);
			AddGameObject<FadeIn>();
			PlayBGM(L"StageBGM");
		}
		catch (...) {
			throw;
		}
	}
	void GameStage::OnUpdate()
	{
		
	}

	void GameStage::PlayBGM(const wstring& StageBGM)
	{
		m_BGM = m_ptrXA->Start(StageBGM, XAUDIO2_LOOP_INFINITE, 0.8f);
	}

	void GameStage::OnDestroy() {
		//BGMのストップ
		m_ptrXA->Stop(m_BGM);
	}
	void GameStage::CreateStageManager() {
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto ptrStageManager = AddGameObject<StageManager>();
		SetSharedGameObject(L"StageManager", ptrStageManager);
		auto Status = ptrStageManager->GameStatus::GAME_PLAYING;
		ptrStageManager->SetNowGameStatus(Status);
		ptrStageManager->SetGameStageSelect(scene->GetSelectedMap());
		vector<Vec3> plVec = {
			Vec3(0.0f, 5.0f,0.0f),
			Vec3(0.0f, -90.0f, 0.0f),
			Vec3(3.0f, 3.0f, 3.0f)
		};
		ptrStageManager->CreateViewLight();
		ptrStageManager->CreatePlayer(plVec[0], plVec[1], plVec[2]);
		ptrStageManager->CreateEnemy();
		ptrStageManager->CreateFixedBox();
		ptrStageManager->CreateGimmick();
		ptrStageManager->CreateSprite();

	}


}
//end basecross
