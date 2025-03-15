/*!
@file GameStageHemmi.h
@brief �e�X�g�Q�[���X�e�[�W
�S���F�팩
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStageHemmi : public Stage {

		shared_ptr<SoundItem> m_BGM;
		std::shared_ptr<basecross::XAudio2Manager> m_PtrXA = App::GetApp()->GetXAudio2Manager();
		//CsvFile m_GameStage1;
		//void CreateViewLight(); //�r���[�̍쐬
		//void CreateGameBox();//GameStage�̐���
		//void CreatePlayer(); //Player�̐���
		//void CreateFixedBox();
		//void CreateGimmick();
		//void CreateEnemy();
		//void InstanceStage();
		//void ToMainCamera();
	public:
		//�\�z�Ɣj��
		GameStageHemmi() :Stage(){}
		virtual ~GameStageHemmi() {}
		virtual void OnCreate()override; //������
		
		void PlayBGM(const wstring& StageBGM);
		void CreateStageManager();
		void CreateEnemy();
		void OnDestroy();
	};

}
//end basecross

