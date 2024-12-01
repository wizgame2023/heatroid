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
			//AddGameObject<FadeIn>();
			PlayBGM(L"StageBGM");
		}
		catch (...) {
			throw;
		}
	}
	void GameStage::OnUpdate()
	{
		GamePause();
	}

	void GameStage::GamePause()
	{
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (m_pause)
		{
			if (cntlVec[0].wPressedButtons && XINPUT_GAMEPAD_START || KeyState.m_bPressedKeyTbl[VK_TAB])
			{
				auto obj = GetGameObjectVec();
				for (auto object : obj)
				{
					object->SetUpdateActive(true);
				}
				m_pause = false;
			}
		}
		else {
			if (cntlVec[0].wPressedButtons && XINPUT_GAMEPAD_START || KeyState.m_bPressedKeyTbl[VK_TAB])
			{
				auto obj = GetGameObjectVec();
				for (auto object : obj)
				{
					object->SetUpdateActive(false);
				}
				m_pause = true;
			}
		}
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
		ptrStageManager->CreatePlayer();
		ptrStageManager->CreateViewLight();
		ptrStageManager->CreateEnemy();
		ptrStageManager->CreateFixedBox();
		ptrStageManager->CreateGimmick();
		ptrStageManager->CreateSprite();
	}


}
//end basecross
