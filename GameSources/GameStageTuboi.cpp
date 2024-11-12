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
		auto camera = ObjectFactory::Create<MainCamera>();
		camera->SetEye(Vec3(0.0f, 80.00f, -5.0f));
		camera->SetAt(Vec3(0.0f, 0.25, 0.0f));
		//auto cameraObject = AddGameObject<CameraCollsion>(Vec3(1, 1, 1));
		//camera->SetCameraObject(cameraObject);
		// ビューにカメラを設定
		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		//マルチライトの作成
		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting2(); //デフォルトのライティングを指定
	}

	void GameStageTsuboi::CreateGameBox() {
		//CSVの行単位の配列
		vector<wstring> LineVec;
		//0番目のカラムがL"FixedBox"である行を抜き出す
		m_GameStage1.GetSelect(LineVec, 0, L"Floor");
		for (auto& v : LineVec) {
			//トークン（カラム）の配列
			vector<wstring> Tokens;
			//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
			Util::WStrToTokenVector(Tokens, v, L',');
			//各トークン（カラム）をスケール、回転、位置に読み込む
			Vec3 Scale(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			Vec3 Rot;
			//回転は「XM_PIDIV2」の文字列になっている場合がある
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Pos(
				-(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);

			//各値がそろったのでオブジェクト作成
			AddGameObject<TilingFixedBox>(Pos, Rot, Scale, 1.0f, 1.0f);
		}
		m_GameStage1.GetSelect(LineVec, 0, L"Wall");
		for (auto& v : LineVec) {
			//トークン（カラム）の配列
			vector<wstring> Tokens;
			//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
			Util::WStrToTokenVector(Tokens, v, L',');
			//各トークン（カラム）をスケール、回転、位置に読み込む
			Vec3 Scale(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			Vec3 Rot;
			//回転は「XM_PIDIV2」の文字列になっている場合がある
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Pos(
				-(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);

			//各値がそろったのでオブジェクト作成
			AddGameObject<TilingFixedBox>(Pos, Rot, Scale, 1.0f, 1.0f);
		}
	}

	//プレイヤーを生成
	void GameStageTsuboi::CreatePlayer() {
		vector<Vec3> plVec = {
			Vec3(2.0f, 5.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(3.0f, 3.0f, 3.0f)
		};

		shared_ptr<GameObject> ptrPlayer = AddGameObject<Player>(plVec[0], plVec[1], plVec[2]);
		SetSharedGameObject(L"Player", ptrPlayer);
		////プレイヤーの攻撃判定
		//auto ptrColl = AddGameObject<AttackCollision>(ptrPlayer);
		//ptrPlayer = AddGameObject<AttackCollision>(ptrPlayer);

	}

	void GameStageTsuboi::CreateEnemy() {

		auto rad = XMConvertToRadians(30.0f);
		vector<vector<Vec3>> vec = {
			{
				Vec3(0.6f,10.0f,3.0f),
				Vec3(0.0f,0.0f,0.0f),
				Vec3(3.0f,3.0f,3.0f),
			},
			//{
			//	Vec3(-0.6f,0.2f,0.0f),
			//	Vec3(0.0f,0.0f,0.0f),
			//	Vec3(0.1f,0.1f,0.1f),
			//}

		};

		auto player = GetSharedGameObject<Player>(L"Player");
		for (auto v : vec) {
			auto enemy = AddGameObject<Enemy>(v[0], v[1], v[2], Enemy::rightMove, Enemy::runaway, player);
			AddGameObject<GaugeSquare>(enemy);
		}
	}

	void GameStageTsuboi::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();

			wstring Datadir;
			App::GetApp()->GetDataDirectory(Datadir);
			m_GameStage1.SetFileName(Datadir + L"GameStage.csv");
			m_GameStage1.ReadCsv();
			
			CreatePlayer();

			auto player = GetSharedGameObject<Player>(L"Player");
			AddGameObject<SpriteHealth>(player);
			AddGameObject<SpriteCharge>(player);

			CreateGameBox();
			CreateEnemy();


		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
