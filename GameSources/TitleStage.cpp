/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void TitleStage::CreateViewLight() {		
		//�r���[�̃J�����̐ݒ�
		auto cameraView = ObjectFactory::Create<SingleView>(GetThis<Stage>());
		auto ptrMyCamera = ObjectFactory::Create<Camera>();
		cameraView->SetCamera(ptrMyCamera);
		ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		ptrMultiLight->SetDefaultLighting();
		SetView(cameraView);
	}

	void TitleStage::OnCreate() {
		try {
			CreateStageManager();
			//�r���[�ƃ��C�g�̍쐬
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
		AddGameObject<Sprite>(L"TITLEBACKGROUND", true, Vec2(640.0f, 410.0f), Vec3(0.0f, -5.0f, 0.3f));
		AddGameObject<Sprite>(L"TITLEEFFECT", true, Vec2(640.0f, 205.0f), Vec3(0.0f, 0.0f, 0.2f));
		AddGameObject<Sprite>(L"TITLEROGO", true, Vec2(640.0f, 410.0f), Vec3(0.0f, 0.0f, 0.1f));
	}

	void TitleStage::OnUpdate() {
		//�R���g���[���`�F�b�N���ē��͂�����΃R�}���h�Ăяo��
		InputHandler<TitleStage> m_InputHandler;
		m_InputHandler.PushHandle(GetThis<TitleStage>());
		auto time = App::GetApp()->GetElapsedTime();

	}

	void TitleStage::OnDestroy() {
		//BGM�̃X�g�b�v
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

}
//end basecross
