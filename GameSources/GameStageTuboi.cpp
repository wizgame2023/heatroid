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
			vector<Vec3> plVec = {
				Vec3(80.0f, 5.0f, 0.0f),
				Vec3(0.0f, -90.0f, 0.0f),
				Vec3(3.0f, 3.0f, 3.0f)
			};

			auto a = AddGameObject<StageManager>();
			a->CreateViewLight();
			a->CreatePlayer(plVec[0], plVec[1], plVec[2]);
			auto player = GetSharedGameObject<Player>(L"Player");
			AddGameObject<SpriteHealth>(player);
			AddGameObject<SpriteCharge>(player);
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
		m_BGM = m_ptrXA->Start(StageBGM, XAUDIO2_LOOP_INFINITE, 0.8f);
	}

	void GameStageTsuboi::OnDestroy() {
		//BGMのストップ
		m_ptrXA->Stop(m_BGM);
	}

}
//end basecross
