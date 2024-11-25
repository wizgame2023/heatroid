/*!
@file Character.h
@brief キャラクターなど
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
		void CreateViewLight(); //ビューの作成		
		void ShowDebug();

	public:
		SelectStage() : Stage() {}
		virtual ~SelectStage() {}

		virtual void OnCreate()override; //初期化
		virtual void OnUpdate()override; //初期化
		void OnDestroy();
		void StageSelect();
		void OnPushA();
		void OnSelectSprite();
		void PlayBGM(const wstring& StageBGM);
		void CreateStageManager();
	};


}