/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void SelectStage::CreateViewLight() {		
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
			//�r���[�ƃ��C�g�̍쐬
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
		//�R���g���[���`�F�b�N���ē��͂�����΃R�}���h�Ăяo��
		InputHandler<SelectStage> m_InputHandler;
		m_InputHandler.PushHandle(GetThis<SelectStage>());
		auto time = App::GetApp()->GetElapsedTime();
		ShowDebug();
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		StageSelect();
	}

	void SelectStage::OnDestroy() {
		//BGM�̃X�g�b�v
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
		}
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
