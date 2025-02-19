/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void SelectStage::CreateViewLight() {		
		//ビューのカメラの設定
		auto cameraView = ObjectFactory::Create<SingleView>(GetThis<Stage>());
		auto ptrMyCamera = ObjectFactory::Create<Camera>();
		cameraView->SetCamera(ptrMyCamera);
		ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
		SetView(cameraView);
	}

	void SelectStage::ShowDebug()
	{
		float delta = App::GetApp()->GetElapsedTime();
		wstringstream wss;

		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wss << "fps : " << App::GetApp()->GetStepTimer().GetFramesPerSecond() << " delta : " << delta << endl;
		wss << "select : " << m_select <<  endl;

		auto scene = App::GetApp()->GetScene<Scene>();
		scene->SetDebugString(L"SelectStage\n" + wss.str());

	}

	void SelectStage::OnCreate() {
		try {
			m_Flag = true;
			m_maxSelect = 4;
			CreateStageManager();
			//ビューとライトの作成
			CreateViewLight();
			OnSelectSprite();
			PlayBGM(L"TitleBGM");
			auto backGround = AddGameObject<GameSprite>(1280, 800, L"SelectStageBack", Vec3(0.0f));
			auto backGround2 = AddGameObject<GameSprite>(1280, 800, L"SelectStageBack2", Vec3(0.0f));
			auto backGround3 = AddGameObject<GameSprite>(1280, 800, L"SelectStageBack3", Vec3(0.0f));
			EffectGround = AddGameObject<GameSprite>(1280, 800, L"SelectStageEffect", Vec3(0.0f,-400.0f,0.0f));
			backGround->SetDrawLayer(-1);
			backGround2->SetDrawLayer(-2);
			backGround3->SetDrawLayer(-3);
			EffectGround->SetDrawLayer(-2);
			auto selectSprite =AddGameObject<SelectSprite>();
			SetSharedGameObject(L"selectSprite", selectSprite);
			m_escapeSelect = m_select;
			SetSelect(selectSprite->GetSelectNum());
			//AddGameObject<Timer>(Vec3(0.0f));
		}
		catch (...) {
			throw;
		}
	}


	void SelectStage::OnSelectSprite()
	{
	}

	void SelectStage::OnUpdate() {
		//コントローラチェックして入力があればコマンド呼び出し
		InputHandler<SelectStage> m_InputHandler;
		m_InputHandler.PushHandle(GetThis<SelectStage>());
		auto time = App::GetApp()->GetElapsedTime();
		//ShowDebug();
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		StageSelect();
		auto selectSprite = GetSharedGameObject<SelectSprite>(L"selectSprite");
		m_escapeSelect = m_select;
		SetSelect(selectSprite->GetSelectNum());
		int a = 0;
		SelectEffect();
	}

	void SelectStage::OnDestroy() {
		//BGMのストップ
		m_ptrXA->Stop(m_BGM);
	}

	void SelectStage::CreateStageManager() {
		auto ptrStageManager = AddGameObject<StageManager>();
		SetSharedGameObject(L"StageManager", ptrStageManager);
		auto Status = ptrStageManager->GameStatus::SELECT;
		ptrStageManager->SetNowGameStatus(Status);
	}

	void SelectStage::StageSelect()
	{
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();

		switch (m_select)
		{
		case 0:
			scene->SetSelectedMap(0);
			break;
		case 1:
			scene->SetSelectedMap(1);
			break;
		case 2:
			scene->SetSelectedMap(2);
			break;
		case 3:
			scene->SetSelectedMap(3);
			break;
		case 4:
			scene->SetSelectedMap(4);
			break;
		case 5:
			scene->SetSelectedMap(5);
			break;
		default:
			break;
		}
	}

	void SelectStage::SelectEffect()
	{
		auto SelectStage = EffectGround->GetComponent<Transform>();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Easing<Vec3> easing;
		float totaltime = 3.0f;
		Vec3 pos = SelectStage->GetPosition();
		pos -= Vec3(0, 15.0f, 0);
		if (pos.y < -600)
		{
			pos.y = 400.0f;
		}
		SelectStage->SetPosition(pos);
	}

	void SelectStage::OnPushA() {
		OnDestroy();
	}

	void SelectStage::PlayBGM(const wstring& StageBGM)
	{
		m_BGM = m_ptrXA->Start(StageBGM, XAUDIO2_LOOP_INFINITE, 0.8f);
	}

}
//end basecross
