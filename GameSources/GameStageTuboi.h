/*!
@file GameStageTsuboi.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStageTsuboi : public Stage {
		CsvFile m_GameStage1;
		shared_ptr<SoundItem> m_BGM;
		std::shared_ptr<basecross::XAudio2Manager> m_PtrXA = App::GetApp()->GetXAudio2Manager();

		void PlayBGM(const wstring& StageBGM);

	public:
		//�\�z�Ɣj��
		GameStageTsuboi() :Stage(){}
		virtual ~GameStageTsuboi() {}

		virtual void OnCreate() override; //������
		virtual void OnDestroy() override; //������
	};

}
//end basecross

