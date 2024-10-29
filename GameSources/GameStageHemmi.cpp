/*!
@file GameStageHemmi.cpp
@brief テストゲームステージ実体
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------

	//ビューとライトの作成
	void GameStageHemmi::CreateViewLight() {
		// カメラの設定
		auto camera = ObjectFactory::Create<Camera>();
		camera->SetEye(Vec3(0.0f, 0.5f, 3.0f));
		camera->SetAt(Vec3(0.0f, 0.5f, 0.0f));

		// ビューにカメラを設定
		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		//マルチライトの作成
		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting(); //デフォルトのライティングを指定
	}

	void GameStageHemmi::CreateGameBox() {
		//配列の初期化
		vector< vector<Vec3> > vec = {
			{
				Vec3(0.0f, -0.4f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(3.0f, 0.1f, 0.3f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec) {
			auto box = AddGameObject<FixedBox>(v[0], v[1], v[2]);
			SetSharedGameObject(L"box",box);
		}
	}
	void GameStageHemmi::CreateBreakWall() {
		vector<vector<Vec3>> vec = {
			{
				Vec3(0.0f,0.5f,0.0f),
				Vec3(0.0f,0.0f,0.0f),
				Vec3(0.5f,0.1f,0.1f)
			},
			{
				Vec3(-1.3f,-0.2f,0.0f),
				Vec3(0.0f,0.0f,0.0f),
				Vec3(0.1f,0.3f,0.1f)
			},
		};

		for (auto v : vec) {
			AddGameObject<BreakWall>(v[0], v[1], v[2]);
		}
	}
	void GameStageHemmi::CreateEnemy() {
		vector<vector<Vec3>> vec = {
			{
				Vec3(0.0f,0.0f,0.0f),
				Vec3(0.0f,0.0f,0.0f),
				Vec3(0.1f,0.1f,0.1f)
			}
		};

		auto player = GetSharedGameObject<Player>(L"Player");
		for (auto v : vec) {
			AddGameObject<Enemy>(v[0], v[1], v[2],Enemy::rightMove,Enemy::runaway,player);
		}
	}


	void GameStageHemmi::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();

			auto player = AddGameObject<Player>();
			SetSharedGameObject(L"Player", player);
			CreateGameBox();
			auto box = GetSharedGameObject<FixedBox>(L"box");
			//auto enemy = AddGameObject<Enemy>(player,box);
			//enemy->SetState(Enemy::rightMove);
			CreateEnemy();
			CreateBreakWall();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
