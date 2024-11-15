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
		auto camera = ObjectFactory::Create<MainCamera>();
		camera->SetEye(Vec3(0.0f, 50.00f, -5.0f));
		camera->SetAt(Vec3(0.0f, 2.5f, 0.0f));
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
				Vec3(0.0f, 0.01f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(50.0f, 0.5f, 50.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//プレイヤーの作成
	void GameStageHemmi::CreatePlayer() {
		//プレーヤーの作成
		auto ptrPlayer = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->GetComponent<Transform>()->SetPosition(Vec3(25, 5.0125f, 0));
		ptrPlayer->GetComponent<Transform>()->SetScale(Vec3(3.0f, 3.0f, 3.0f));
		auto playerPos = ptrPlayer->GetComponent<Transform>();

	}

	//ボックスの作成
	void GameStageHemmi::CreateFixedBox() {
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
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);

			//各値がそろったのでオブジェクト作成
			AddGameObject<TilingFixedBox>(Pos, Rot, Scale, 1.0f, 1.0f, Tokens[10]);
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
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);

			//各値がそろったのでオブジェクト作成
			AddGameObject<TilingFixedBox>(Pos, Rot, Scale, 1.0f, 1.0f, Tokens[10]);
		}
	}

	void GameStageHemmi::CreateGimmick()
	{
		CreateSharedObjectGroup(L"Door");
		CreateSharedObjectGroup(L"Switch");
		vector<wstring> LineVec;
		m_GameStage1.GetSelect(LineVec, 0, L"Door");
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
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);

			float Switch = (float)_wtof(Tokens[10].c_str());
			int number = (float)_wtof(Tokens[11].c_str());

			//各値がそろったのでオブジェクト作成
			auto door = AddGameObject<GimmickDoor>(Pos, Rot, Scale, 1.0f, 1.0f, Switch, number, Tokens[12]);
		}

		m_GameStage1.GetSelect(LineVec, 0, L"Switch");
		for (auto& v : LineVec) {
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Scale(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			Vec3 Rot;
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);

			float Button = (float)_wtof(Tokens[10].c_str());
			int number = (float)_wtof(Tokens[11].c_str());

			AddGameObject<GimmickButton>(Pos, Rot, Scale, Button, number, Tokens[12]);
		}

	}

	void GameStageHemmi::CreateEnemy()
	{

		vector<wstring> LineVec;
		m_GameStage1.GetSelect(LineVec, 0, L"Enemy");
		for (auto& v : LineVec) {
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Scale(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			Vec3 Rot;
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);
			auto player = GetSharedGameObject<Player>(L"Player");
			auto enemy = AddGameObject<Enemy>(Pos, Rot, Scale, Enemy::stay, Enemy::stay, player);
			AddGameObject<GaugeSquare>(enemy);
		}
	}

	void GameStageHemmi::OnCreate() {
		try {
			wstring Datadir;
			App::GetApp()->GetDataDirectory(Datadir);
			//CSVパスを取得
			m_GameStage1.SetFileName(Datadir + L"CSV/" + L"GameStage.csv");
			m_GameStage1.ReadCsv();
			//ビューとライトの作成
			CreateViewLight();
			//CreateGameBox();
			//CreateGimmick();

			CreatePlayer();
			CreateFixedBox();
			CreateEnemy();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
