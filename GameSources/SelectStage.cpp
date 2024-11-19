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

	void SelectStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
			OnSelectSprite();
			PlayBGM(L"TitleBGM");
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

	}

	void SelectStage::OnDestroy() {
		//BGMのストップ
		m_ptrXA->Stop(m_BGM);
	}


	void SelectStage::OnPushA() {
		OnDestroy();
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
	}

	void SelectStage::PlayBGM(const wstring& StageBGM)
	{
		m_BGM = m_ptrXA->Start(StageBGM, XAUDIO2_LOOP_INFINITE, 0.8f);
	}

}
//end basecross
