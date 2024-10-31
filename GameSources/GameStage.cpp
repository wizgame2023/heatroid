/*!
鎌田大輝
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------

	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		// カメラの設定
		auto camera = ObjectFactory::Create<Camera>();
		camera->SetEye(Vec3(0.0f, 0.02f, -50.0f));
		camera->SetAt(Vec3(0.0f, 0.25, 0.0f));

		// ビューにカメラを設定
		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		//マルチライトの作成
		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting2(); //デフォルトのライティングを指定
	}

	void GameStage::CreateGameBox() {
		//配列の初期化
		vector< vector<Vec3> > vec = {
			{
				Vec3(3.0f, 0.1f, 0.3f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, -0.4f, 0.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		//プレーヤーの作成
		auto ptrPlayer = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->GetComponent<Transform>()->SetPosition(Vec3(-3, 5.0125f, 0));
		ptrPlayer->GetComponent<Transform>()->SetScale(Vec3(1, 1, 1));
		auto playerPos = ptrPlayer->GetComponent<Transform>();
		TilingFixedBox::m_moveObject.push_back(playerPos);

	}

	//ボックスの作成
	void GameStage::CreateFixedBox() {
		//CSVの行単位の配列
		vector<wstring> LineVec;
		//0番目のカラムがL"FixedBox"である行を抜き出す
		m_GameStage1.GetSelect(LineVec, 0, L"FixedBox");
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
			AddGameObject<TilingFixedBox>();
		}
	}

	void GameStage::CreateGimmick()
	{
		vector<wstring> LineVec;
		m_GameStage1.GetSelect(LineVec, 0, L"GimmickDoor");
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
			AddGameObject<GimmickDoor>(Pos, Rot, Scale, 1.0f, 1.0f);
		}

		m_GameStage1.GetSelect(LineVec, 0, L"GimmickButton");
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
			AddGameObject<GimmickButton>(Pos, Rot, Scale, 1.0f, 1.0f);
		}

	}

	void GameStage::CreateEnemy()
	{
		vector<wstring> LineVec;
		m_GameStage1.GetSelect(LineVec, 0, L"EnemyrightMove");
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
			AddGameObject<Enemy>(Pos, Rot, Scale, Enemy::rightMove, Enemy::runaway, player);
		}
		m_GameStage1.GetSelect(LineVec, 0, L"EnemyrightMoveStay");
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
			AddGameObject<Enemy>(Pos, Rot, Scale, Enemy::stay, Enemy::stay, player);
		}

	}

	void GameStage::OnCreate() {
		try {
			wstring Datadir;
			App::GetApp()->GetDataDirectory(Datadir);

			m_GameStage1.SetFileName(Datadir + L"GameStage1.csv");
			m_GameStage1.ReadCsv();
			//ビューとライトの作成
			CreateViewLight();
			//CreateGameBox();
			//CreateGimmick();
			AddGameObject<TilingFixedBox>();

			CreatePlayer();
			//CreateFixedBox();
			//CreateEnemy();
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::OnUpdate()
	{

		for (auto& object : GetGameObjectVec())
		{
			if (object->FindTag(L"FixedBox"))
			{
				for (auto& weaktrans : TilingFixedBox::m_moveObject)
				{
					auto trans = weaktrans.lock();
					if (trans != nullptr)
					{
						if (length(trans->GetPosition() - object->GetComponent<Transform>()->GetPosition()) < 1.0f) {
							object->SetUpdateActive(true);
						}
						else
						{
							object->SetUpdateActive(false);
						}
					}
				}
			}
		}
	}

}
//end basecross
