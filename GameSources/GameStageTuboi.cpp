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
		camera->SetEye(Vec3(0.0f, .2f, 3.0f));
		camera->SetAt(Vec3(0.0f, .2f, 0.0f));

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
				Vec3(0.0f, -.5f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(3.0f, .1f, .3f)
			},
			{
				Vec3(.8f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(1.0f, .1f, .3f)
			},
			{
				Vec3(-.80f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(.1f, 1.0f, .3f)
			},
		};
		int i = 0;
		//オブジェクトの作成
		for (auto v : vec) {
			auto box = AddGameObject<FixedBox>(v[0], v[1], v[2]);
			SetSharedGameObject(L"box" + to_wstring(i++), box);
		}
	}


	void GameStageTsuboi::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();

			//プレイヤーを生成
			shared_ptr<GameObject> ptrPlayer = AddGameObject<Player>();
			SetSharedGameObject(L"Player", ptrPlayer);
			//プレイヤーの攻撃判定
			ptrPlayer = AddGameObject<AttackCollision>(ptrPlayer);

			CreateGameBox();

			auto box = GetSharedGameObject<FixedBox>(L"box0");
			auto player = GetSharedGameObject<Player>(L"Player");
			//auto enemy = AddGameObject<Enemy>(player, box);


		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
