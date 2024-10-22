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

	//ビューとライトの作成
	void GameStageTsuboi::CreateViewLight() {
		// カメラの設定
		auto camera = ObjectFactory::Create<Camera>();
		camera->SetEye(Vec3(0.0f, 2.0f, 30.0f));
		camera->SetAt(Vec3(0.0f, 2.0f, 0.0f));

		// ビューにカメラを設定
		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		//マルチライトの作成
		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting(); //デフォルトのライティングを指定
	}

	void GameStageTsuboi::CreateGameBox() {
		//配列の初期化
		vector< vector<Vec3> > vec = {
			{
				Vec3(30.0f, 1.0f, 3.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -4.0f, 0.0f)
			},
			{
				Vec3(10.0f, 1.0f, 3.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(8.0f, 0.0f, 0.0f)
			},
			{
				Vec3(1.0f, 10.0f, 3.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-8.0f, 0.0f, 0.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}


	void GameStageTsuboi::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();

			auto ptrPlayer = AddGameObject<Player>();
			SetSharedGameObject(L"Player", ptrPlayer);

			CreateGameBox();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
