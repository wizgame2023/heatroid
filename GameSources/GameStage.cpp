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

			m_pauseBackGround = AddGameObject<Sprite>(L"PauseBG", true, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.0f));
			m_pauseBackGround->SetDrawLayer(3);

			m_PauseSelect = AddGameObject<Sprite>(L"PauseSelect", true, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.0f));
			m_PauseSelect->SetDrawLayer(3);

			m_PauseTitle = AddGameObject<Sprite>(L"PauseTitle", true, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.0f));
			m_PauseTitle->SetDrawLayer(3);

			m_PauseBack = AddGameObject<Sprite>(L"PauseBack", true, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.0f));
			m_PauseBack->SetDrawLayer(3);

			m_pauseBackGround->SetDrawActive(false);
			m_PauseSelect->SetDrawActive(false);
			m_PauseTitle->SetDrawActive(false);
			m_PauseBack->SetDrawActive(false);

			//エフェクトの初期化
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			wstring TestEffectStr = DataDir + L"Effects\\Laser01.efk";
			auto stageMane = GetSharedGameObject<StageManager>(L"StageManager");
			auto ShEfkInterface = stageMane->GetEfkInterface();
			m_EfkEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, TestEffectStr);
		}
		catch (...) {
			throw;
		}
	}
	void GameStage::OnUpdate()
	{
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		auto stageMane = GetSharedGameObject<StageManager>(L"StageManager");
		int camerastatus = stageMane->GetNowCameraStatus();
		auto ShEfkInterface = stageMane->GetEfkInterface();
		if (stageMane->m_CameraSelect == StageManager::CameraSelect::myCamera)
		{
			ShEfkInterface->OnUpdate();
			GamePause();
			if (KeyState.m_bPressedKeyTbl[VK_TAB])
			{
				EffectPlay();
			}
		}
	}

	void GameStage::OnDraw()
	{
		auto& camera = GetView()->GetTargetCamera();
		auto stageMane = GetSharedGameObject<StageManager>(L"StageManager");
		auto ShEfkInterface = stageMane->GetEfkInterface();
		ShEfkInterface->SetViewProj(camera->GetViewMatrix(), camera->GetProjMatrix());
		ShEfkInterface->OnDraw();
	}

	void GameStage::OnPushA()
	{
	}

	void GameStage::GamePause()
	{
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		auto playerSh = GetSharedGameObject<Player>(L"Player");
		auto stageMane = GetSharedGameObject<StageManager>(L"StageManager");
		bool m_Diedtrue = playerSh->GetDied();
		bool m_Goaltrue = playerSh->GetArrivedGoal();
		auto group = GetSharedObjectGroup(L"Enemy");
		auto group2 = GetSharedObjectGroup(L"Door");

		if (m_Diedtrue){
			auto& vec = group->GetGroupVector();
			for (auto v : vec)
			{
				auto shObj = v.lock();
				if (shObj)
				{
					shObj->SetUpdateActive(false);
				}
			}
			auto& vec2 = group2->GetGroupVector();
			for (auto v : vec2)
			{
				auto shObj = v.lock();
				if (shObj)
				{
					shObj->SetUpdateActive(false);
				}
			}
		}
		else if (m_Goaltrue) {
			auto& vec = group->GetGroupVector();
			for (auto v : vec)
			{
				auto shObj = v.lock();
				if (shObj)
				{
					shObj->SetUpdateActive(false);
				}
			}
			auto& vec2 = group2->GetGroupVector();
			for (auto v : vec2)
			{
				auto shObj = v.lock();
				if (shObj)
				{
					shObj->SetUpdateActive(false);
				}
			}
		}
		else {
			if (m_pause)
			{
				m_pauseBackGround->SetDrawActive(true);
				m_PauseSelect->SetDrawActive(true);
				m_PauseTitle->SetDrawActive(true);
				m_PauseBack->SetDrawActive(true);
				stageMane->m_SelectCharge->SetDrawActive(false);
				stageMane->m_TitleCharge->SetDrawActive(false);
				auto time = App::GetApp()->GetElapsedTime();
				stageMane->SetPushState(0);
				if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_START || KeyState.m_bPressedKeyTbl[VK_TAB])
				{
					auto obj = GetGameObjectVec();
					for (auto& object : obj)
					{
						object->SetUpdateActive(true);
					}
					m_pause = false;
				}
				if (cntlVec[0].wButtons & XINPUT_GAMEPAD_A || KeyState.m_bPushKeyTbl[VK_RETURN])
				{
					totaltime += time;
					stageMane->SetPushState(1);
					if (totaltime > 1.0f)
					{
						PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToSlelctStage");
						OnDestroy();
					}
				}
				if (cntlVec[0].wReleasedButtons & XINPUT_GAMEPAD_A || KeyState.m_bUpKeyTbl[VK_RETURN])
				{
					stageMane->SetPushState(0);
				}
				if (cntlVec[0].wButtons & XINPUT_GAMEPAD_B || KeyState.m_bPushKeyTbl[VK_BACK])
				{
					stageMane->SetPushState(2);
					totaltime += time;
					if (totaltime > 1.0f)
					{
						PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
						OnDestroy();
					}
				}
				if (cntlVec[0].wReleasedButtons & XINPUT_GAMEPAD_B || KeyState.m_bUpKeyTbl[VK_BACK])
				{
					stageMane->SetPushState(0);
				}

			}
			else {
				m_pauseBackGround->SetDrawActive(false);
				m_PauseSelect->SetDrawActive(false);
				m_PauseTitle->SetDrawActive(false);
				m_PauseBack->SetDrawActive(false);
				stageMane->m_SelectCharge->SetDrawActive(false);
				stageMane->m_TitleCharge->SetDrawActive(false);
				if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_START || KeyState.m_bPressedKeyTbl[VK_TAB])
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
	}
	void GameStage::EffectPlay()
	{
		auto stageMane = GetSharedGameObject<StageManager>(L"StageManager");
		auto ShEfkInterface = stageMane->GetEfkInterface();
		m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_EfkEffect, Vec3(0, 1, 0));
		m_EfkPlay->SetRotation(Vec3(0, 0, XMConvertToRadians(90.0f)), 0.0f);
		m_EfkPlay->SetAllColor(Col4(1.0f, 0.0f, 1.0f, 1.0f));
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
		if (scene->GetSelectedMap() == L"ToTitle")
		{
			auto Status = ptrStageManager->GameStatus::TEST_PLAY;
			ptrStageManager->SetNowGameStatus(Status);
			ptrStageManager->SetGameStageSelect(scene->GetSelectedMap());
			ptrStageManager->CreateViewLight();
			ptrStageManager->CreatePlayer();
			ptrStageManager->CreateFixedBox();
			ptrStageManager->CreateEnemy();
			ptrStageManager->CreateSprite();
			ptrStageManager->CreateGimmick();
		}
		else
		{
			auto Status = ptrStageManager->GameStatus::GAME_PLAYING;
			ptrStageManager->SetNowGameStatus(Status);
			ptrStageManager->SetGameStageSelect(scene->GetSelectedMap());
			ptrStageManager->CreatePlayer();
			ptrStageManager->CreateViewLight();
			ptrStageManager->CreateEnemy();
			ptrStageManager->CreateFixedBox();
			ptrStageManager->CreateSprite();
			ptrStageManager->CreateGimmick();
		}
	}


}
//end basecross
