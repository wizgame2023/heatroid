/*!
@file GameStageTsuboi.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------

	void GameStageTsuboi::OnCreate() {
		try {

			auto a = AddGameObject<StageGenerator>();
			a->CreateViewLight();
			a->CreatePlayer();
			auto player = GetSharedGameObject<Player>(L"Player");
			a->CreateEnemy();
			a->CreateFixedBox();
			a->CreateGimmick();
			a->CreateSprite();
			PlayBGM(L"StageBGM");

		}
		catch (...) {
			throw;
		}
	}

	void GameStageTsuboi::PlayBGM(const wstring& StageBGM)
	{
		m_BGM = m_PtrXA->Start(StageBGM, XAUDIO2_LOOP_INFINITE, 0.8f);
	}

	void GameStageTsuboi::OnDestroy() {
		//BGMのストップ
		m_PtrXA->Stop(m_BGM);
	}

}
//end basecross
