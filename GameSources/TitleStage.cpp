/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void TitleStage::CreateViewLight() {		
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

	void TitleStage::OnCreate() {
		try {
			CreateStageManager();
			//ビューとライトの作成
			CreateViewLight();
			//OnDraw();
			OnTitleSprite();
			PlayBGM(L"TitleBGM");
		}
		catch (...) {
			throw;
		}
	}

	void TitleStage::OnTitleSprite()
	{
		AddGameObject<BlinkingSprite>(L"TITLETEXT", true, Vec2(640.0f, 410.0f), Vec3(0.0f, -250.0f, 0.1f), 3.5f);
		AddGameObject<Sprite>(L"TITLEBACKGROUND", true, Vec2(640.0f, 410.0f), Vec3(0.0f, -5.0f, 0.3f));
		AddGameObject<Sprite>(L"TITLEEFFECT", true, Vec2(640.0f, 205.0f), Vec3(0.0f, 0.0f, 0.2f));
		AddGameObject<Sprite>(L"TITLEROGO", true, Vec2(640.0f, 410.0f), Vec3(0.0f, 0.0f, 0.1f));
	}

	void TitleStage::OnUpdate() {
		//コントローラチェックして入力があればコマンド呼び出し
		InputHandler<TitleStage> m_InputHandler;
		m_InputHandler.PushHandle(GetThis<TitleStage>());
		auto time = App::GetApp()->GetElapsedTime();
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A || KeyState.m_bPressedKeyTbl[VK_SPACE])
		{
			m_startFlag = true;
			PlaySE(L"DecisionSE", 0, 1.0f);
		}
		if (m_startFlag)
		{
			m_updateTime += ElapsedTime;
			if (m_updateTime > 0.5f)
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToSlelctStage");
			}
		}
	}

	void TitleStage::OnDestroy() {
		//BGMのストップ
		m_PtrXA->Stop(m_BGM);
	}

	void TitleStage::CreateStageManager() {
		auto ptrStageManager = AddGameObject<StageGenerator>();
		SetSharedGameObject(L"StageManager", ptrStageManager);
	}

	void TitleStage::OnPushA() {
		OnDestroy();
	}

	void TitleStage::PlayBGM(const wstring& StageBGM)
	{
		m_BGM = m_PtrXA->Start(StageBGM, XAUDIO2_LOOP_INFINITE, 0.8f);
	}

	void TitleStage::PlaySE(wstring path, float loopcnt, float volume)
	{
		auto playSE = App::GetApp()->GetXAudio2Manager();
		playSE->Start(path, loopcnt, volume);
	}

}
//end basecross
