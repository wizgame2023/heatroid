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
		camera->SetEye(Vec3(0.0f, 2.0f, 15.0f));
		camera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

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
				Vec3(30.0f, 1.0f, 3.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -4.0f, 0.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}
	void GameStageHemmi::CreateEnemy() {
		vector<vector<Vec3>> vec = {
			{
				Vec3(5.0f,5.0f,0.0f),
				Vec3(0.0f,0.0f,0.0f),
				Vec3(1.0f,1.0f,1.0f)
			}
		};

		for (auto v : vec) {
			AddGameObject<Enemy>(v[0], v[1], v[2]);
		}
	}


	void GameStageHemmi::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();

			auto player = AddGameObject<Player>();
			CreateGameBox();
			AddGameObject<Enemy>(player);
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
