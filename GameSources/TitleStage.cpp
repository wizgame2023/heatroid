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

	void TitleStage::OnDraw()
	{
		auto ptrSofa = AddGameObject<Draw>(Vec3(0), Vec3());
	}

	void TitleStage::OnTitleSprite()
	{
		AddGameObject<Sprite>(L"TITLEBACKGROUND", true, Vec2(640.0f, 410.0f), Vec3(0.0f, -5.0f, 0.3f));
		AddGameObject<BlinkingSprite>(L"TITLETEXT", true, Vec2(640.0f, 410.0f), Vec3(0.0f, -250.0f, 0.1f));
		AddGameObject<Sprite>(L"TITLEEFFECT", true, Vec2(640.0f, 205.0f), Vec3(0.0f, 0.0f, 0.2f));
		AddGameObject<Sprite>(L"TITLEROGO", true, Vec2(640.0f, 410.0f), Vec3(0.0f, 0.0f, 0.1f));
	}

	void TitleStage::OnUpdate() {
		//コントローラチェックして入力があればコマンド呼び出し
		InputHandler<TitleStage> m_InputHandler;
		m_InputHandler.PushHandle(GetThis<TitleStage>());
		auto time = App::GetApp()->GetElapsedTime();

	}

	void TitleStage::OnDestroy() {
		//BGMのストップ
		m_ptrXA->Stop(m_BGM);
	}

	void TitleStage::CreateStageManager() {
		auto ptrStageManager = AddGameObject<StageManager>();
		SetSharedGameObject(L"StageManager", ptrStageManager);
		auto Status =  ptrStageManager->GameStatus::TITLE;
		ptrStageManager->SetNowGameStatus(Status);
	}

	void TitleStage::OnPushA() {
		OnDestroy();
	}

	void TitleStage::PlayBGM(const wstring& StageBGM)
	{
		m_BGM = m_ptrXA->Start(StageBGM, XAUDIO2_LOOP_INFINITE, 0.8f);
	}

	Draw::Draw(const shared_ptr<Stage>& stage,
		const Vec3& Scale,
		const Vec3& Position
	) :
		GameObject(stage),
		m_scale(Scale),
		m_position(Position)
	{}

	void Draw::OnCreate()
	{
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(Vec3(0));
		ptrTransform->SetPosition(Vec3(0));
	}


}
//end basecross
