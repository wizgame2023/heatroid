/*!
鎌田大輝
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void GameStage::OnCreate() {
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
			
		}
		catch (...) {
			throw;
		}
	}
	void GameStage::OnUpdate()
	{
	}

	void GameStage::PlayBGM(const wstring& StageBGM)
	{
		m_BGM = m_ptrXA->Start(StageBGM, XAUDIO2_LOOP_INFINITE, 0.8f);
	}
}
//end basecross
