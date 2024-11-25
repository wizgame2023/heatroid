/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class SelectStage : public Stage
	{
		int m_select = 0;
		bool m_Flag = false;
		int m_maxSelect;
		shared_ptr<SoundItem> m_BGM;
		std::shared_ptr<basecross::XAudio2Manager> m_ptrXA = App::GetApp()->GetXAudio2Manager();
		void CreateViewLight(); //�r���[�̍쐬		
		void ShowDebug();

	public:
		SelectStage() : Stage() {}
		virtual ~SelectStage() {}

		virtual void OnCreate()override; //������
		virtual void OnUpdate()override; //������
		void OnDestroy();
		void StageSelect();
		void OnPushA();
		void OnSelectSprite();
		void PlayBGM(const wstring& StageBGM);
		void CreateStageManager();
	};


}