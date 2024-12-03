/*!
���c��P
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		CsvFile m_GameStage1;
		shared_ptr<SoundItem> m_BGM;
		std::shared_ptr<basecross::XAudio2Manager> m_ptrXA = App::GetApp()->GetXAudio2Manager();
		shared_ptr<GameObject> m_fadeIn;
		shared_ptr<GameObject> m_fadeOut;

		shared_ptr<GameObject> m_pauseBackGround;
		shared_ptr<GameObject> m_PauseSelect;
		shared_ptr<GameObject> m_PauseTitle;
		shared_ptr<GameObject> m_PauseBack;
		shared_ptr<GameObject> m_SelectCharge;
		shared_ptr<GameObject> m_TitleCharge;
		float totaltime = 0.0f;

	public:
		bool m_pause = false;

		//�\�z�Ɣj��
		GameStage() :Stage(){}
		virtual ~GameStage() {}
		
		virtual void OnCreate()override; //������
		virtual void OnUpdate()override; //������

		void PlayBGM(const wstring& StageBGM);

		void OnDestroy();

		void CreateStageManager();

		void GamePause();
	};

}
//end basecross

