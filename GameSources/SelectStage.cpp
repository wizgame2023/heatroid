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

	void SelectStage::OnCreate() {
		try {
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

	}

	void SelectStage::OnDestroy() {
		//BGM�̃X�g�b�v
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
