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
			CreateStagegenerator();
			SpriteCreate();
			PlayBGM(L"StageBGM");
			EnemyUpdateChange();
			m_soundFlg = true;
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::OnUpdate()
	{
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		auto stageMane = GetSharedGameObject<StageGenerator>(L"StageManager");
		int camerastatus = stageMane->GetNowCameraStatus();
		auto ShEfkInterface = stageMane->GetEfkInterface();
		if (stageMane->m_nowGameStatus == StageGenerator::GameStatus::GAME_PLAYING)
		{
			ShEfkInterface->OnUpdate();
			if (stageMane->m_CameraSelect == StageGenerator::CameraSelect::OPENINGCAMERA)
			{
				EnemyUpdateChange();
			}
			if (stageMane->m_CameraSelect == StageGenerator::CameraSelect::PLAYCAMERA)
			{
				stageMane->EnemyUpdate();
				stageMane->OperationUIMake();
				stageMane->GoalJudge();
				stageMane->GameOverJudge();
				GamePause();
			}
			if (stageMane->m_CameraSelect == StageGenerator::CameraSelect::ENDINGCAMERA)
			{
				EnemyUpdateChange();
				stageMane->GoalJudge();
			}
		}
		if (stageMane->m_nowGameStatus == StageGenerator::GameStatus::TEST_PLAY)
		{
			ShEfkInterface->OnUpdate();

			if (stageMane->m_CameraSelect == StageGenerator::CameraSelect::OPENINGCAMERA)
			{
				EnemyUpdateChange();
			}
			if (stageMane->m_CameraSelect == StageGenerator::CameraSelect::PLAYCAMERA)
			{
				stageMane->EnemyUpdate();
				stageMane->OperationUIMake();
				stageMane->GoalJudge();
				stageMane->GameOverJudge();
				GamePause();
			}
		}
	}

	void GameStage::OnDraw()
	{
		auto& camera = GetView()->GetTargetCamera();
		auto stageMane = GetSharedGameObject<StageGenerator>(L"StageManager");
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
		auto stageMane = GetSharedGameObject<StageGenerator>(L"StageManager");
		bool m_diedtrue = playerSh->GetDied();
		bool m_goaltrue = playerSh->GetArrivedGoal();
		auto group = GetSharedObjectGroup(L"Enemy");
		auto group2 = GetSharedObjectGroup(L"Door");

		if (m_diedtrue){
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
		else if (m_goaltrue) {
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
				m_selectCharge->SetDrawActive(false);
				m_titleCharge->SetDrawActive(false);
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
					m_selectCharge->SetDrawActive(true);
					totaltime += time;
					stageMane->SetPushState(1);
					auto group = GetSharedObjectGroup(L"Sprite");
					auto& vec = group->GetGroupVector();
					if (m_soundFlg)
					{
						//PlaySE(L"ChargeSelect", 0, 1.0f);
						m_soundFlg = false;
					}

					for (auto v : vec)
					{
						auto select = v.lock();
						if (select->FindTag(L"SelectCharge"))
						{
							auto obj = dynamic_pointer_cast<SelectCharge>(select);
							obj->ChargeUV(totaltime);
						}
					}

					if (totaltime > 1.0f)
					{

						OnDestroy();
						PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToSlelctStage");
					}
				}
				if (cntlVec[0].wReleasedButtons & XINPUT_GAMEPAD_A || KeyState.m_bUpKeyTbl[VK_RETURN])
				{
					m_soundFlg = true;
					totaltime = 0;
					stageMane->SetPushState(0);
				}
				if (cntlVec[0].wButtons & XINPUT_GAMEPAD_B || KeyState.m_bPushKeyTbl[VK_SPACE])
				{
					m_titleCharge->SetDrawActive(true);
					totaltime += time;
					stageMane->SetPushState(1);
					auto group = GetSharedObjectGroup(L"Sprite");
					auto& vec = group->GetGroupVector();

					if (m_soundFlg)
					{
						//PlaySE(L"ChargeSelect", 0, 1.0f);
						m_soundFlg = false;
					}
					m_soundFlg = false;

					for (auto v : vec)
					{
						auto select = v.lock();
						if (select->FindTag(L"TitleCharge"))
						{
							auto obj = dynamic_pointer_cast<SelectCharge>(select);
							obj->ChargeUV(totaltime);
						}
					}
					if (totaltime > 1.2f)
					{
						PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
						OnDestroy();
					}
				}
				else if (cntlVec[0].wReleasedButtons & XINPUT_GAMEPAD_B || KeyState.m_bUpKeyTbl[VK_SPACE])
				{
					m_soundFlg = true;
					totaltime = 0;
					stageMane->SetPushState(0);
				}
				if (cntlVec[0].wReleasedButtons & XINPUT_GAMEPAD_B || KeyState.m_bUpKeyTbl[VK_BACK])
				{

					totaltime = 0;
					stageMane->SetPushState(0);
				}

			}
			else {
				m_pauseBackGround->SetDrawActive(false);
				m_PauseSelect->SetDrawActive(false);
				m_PauseTitle->SetDrawActive(false);
				m_PauseBack->SetDrawActive(false);
				m_selectCharge->SetDrawActive(false);
				m_titleCharge->SetDrawActive(false);
				if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_START || KeyState.m_bPressedKeyTbl[VK_TAB])
				{
					auto obj = GetGameObjectVec();
					for (auto object : obj)
					{
						if (!object->FindTag(L"SelectCharge") && !object->FindTag(L"TitleCharge"))
						{
							object->SetUpdateActive(false);
						}
					}
					m_pause = true;
				}
			}

		}
	}

	void GameStage::SpriteCreate()
	{
		m_pauseBackGround = AddGameObject<Sprite>(L"PauseBG", true, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.0f));
		m_pauseBackGround->SetDrawLayer(3);

		m_PauseSelect = AddGameObject<Sprite>(L"PauseSelect", true, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.0f));
		m_PauseSelect->SetDrawLayer(3);

		m_PauseTitle = AddGameObject<Sprite>(L"PauseTitle", true, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.0f));
		m_PauseTitle->SetDrawLayer(3);

		m_PauseBack = AddGameObject<Sprite>(L"PauseBack", true, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.0f));
		m_PauseBack->SetDrawLayer(3);

		auto group = CreateSharedObjectGroup(L"Sprite");

		m_selectCharge = AddGameObject<SelectCharge>(L"PauseSelectCharge", true, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.0f));
		m_selectCharge->SetDrawLayer(4);
		m_selectCharge->AddTag(L"SelectCharge");

		m_titleCharge = AddGameObject<SelectCharge>(L"PauseTitleCharge", true, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.0f));
		m_titleCharge->SetDrawLayer(4);
		m_titleCharge->AddTag(L"TitleCharge");

		m_pauseBackGround->SetDrawActive(false);
		m_PauseSelect->SetDrawActive(false);
		m_PauseTitle->SetDrawActive(false);
		m_PauseBack->SetDrawActive(false);
		m_selectCharge->SetDrawActive(false);
		m_titleCharge->SetDrawActive(false);
	}

	void GameStage::EnemyUpdateChange()
	{
		auto group = GetSharedObjectGroup(L"Enemy");
		auto& vec = group->GetGroupVector();
		for (auto v : vec)
		{
			auto shObj = v.lock();
			if (shObj->GetUpdateActive() == true)
			{
				shObj->SetUpdateActive(false);
			}
		}
	}

	void GameStage::PlayBGM(const wstring& StageBGM)
	{
		m_BGM = m_PtrXA->Start(StageBGM, XAUDIO2_LOOP_INFINITE, 0.8f);
	}

	void GameStage::PlaySE(wstring path, float loopcnt, float volume) {
		m_PtrXA->Start(path, loopcnt, volume);
	}

	void GameStage::OnDestroy() {
		//BGMのストップ
		m_PtrXA->Stop(m_BGM);
	}
	void GameStage::CreateStagegenerator() {
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto ptrStageManager = AddGameObject<StageGenerator>();
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
			ptrStageManager->CreateFixedBox();
			ptrStageManager->CreateEnemy();
			ptrStageManager->CreateSprite();
			ptrStageManager->CreateGimmick();
			ptrStageManager->EnemyRay();
		}
	}


}
//end basecross
