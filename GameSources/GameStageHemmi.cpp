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
	void GameStageHemmi::OnCreate() {
		try {
			CreateStageManager();
			auto player = GetSharedGameObject<Player>(L"Player");

			//AddGameObject<FadeIn>();
			PlayBGM(L"StageBGM");
		}
		catch (...) {
			throw;
		}
	}

	void GameStageHemmi::PlayBGM(const wstring& StageBGM)
	{
		m_BGM = m_PtrXA->Start(StageBGM, XAUDIO2_LOOP_INFINITE, 0.8f);
	}

	void GameStageHemmi::OnDestroy() {
		//BGMのストップ
		m_PtrXA->Stop(m_BGM);
	}
	void GameStageHemmi::CreateStageManager() {
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto ptrStageManager = AddGameObject<StageGenerator>();
		SetSharedGameObject(L"StageManager", ptrStageManager);
		auto Status = ptrStageManager->GameStatus::GAME_PLAYING;
		ptrStageManager->SetNowGameStatus(Status);
		ptrStageManager->SetGameStageSelect(scene->GetSelectedMap());
		ptrStageManager->CreatePlayer();
		ptrStageManager->CreateViewLight();
		//ptrStageManager->CreateEnemy();
		ptrStageManager->CreateFixedBox();
		ptrStageManager->CreateGimmick();
		ptrStageManager->CreateSprite();
	}

	//ビューとライトの作成
	//void GameStageHemmi::CreateViewLight() {
	//	// カメラの設定
	//	auto camera = ObjectFactory::Create<MainCamera>();
	//	camera->SetEye(Vec3(-50.0f, 3.00f, 0.0f));
	//	camera->SetAt(Vec3(0.0f, 0.25, 0.0f));
	//	//camera->SetCameraObject(cameraObject);
	//	// ビューにカメラを設定
	//	auto view = CreateView<SingleView>();
	//	view->SetCamera(camera);

	//	//マルチライトの作成
	//	auto light = CreateLight<MultiLight>();
	//	light->SetDefaultLighting(); //デフォルトのライティングを指定
	//	auto cameraObject = AddGameObject<CameraCollision>();

	//}

	//void GameStageHemmi::CreateGameBox() {
	//	//配列の初期化
	//	vector< vector<Vec3> > vec = {
	//		{
	//			Vec3(0.0f, 0.01f, 0.0f),
	//			Vec3(0.0f, 0.0f, 0.0f),
	//			Vec3(-50.0f, 0.5f, 70.0f)
	//		},
	//	};
	//	//オブジェクトの作成
	//	for (auto v : vec) {
	//		AddGameObject<FixedBox>(v[0], v[1], v[2]);
	//	}
	//}

	////プレイヤーの作成
	//void GameStageHemmi::CreatePlayer() {
	//	vector<Vec3> plVec = {
	//		Vec3(-20.0f, 5.0f, 0.0f),
	//		Vec3(0.0f, 0.0f, 0.0f),
	//		Vec3(3.0f, 3.0f, 3.0f)
	//	};
	//	//プレーヤーの作成
	//	shared_ptr<GameObject> ptrPlayer = AddGameObject<Player>(plVec[0], plVec[1], plVec[2]);
	//	//シェア配列にプレイヤーを追加
	//	SetSharedGameObject(L"Player", ptrPlayer);
	//	auto playerPos = ptrPlayer->GetComponent<Transform>();
	//}

	////ボックスの作成
	//void GameStageHemmi::CreateFixedBox() {
	//	//CSVの行単位の配列
	//	vector<wstring> LineVec;
	//	//0番目のカラムがL"FixedBox"である行を抜き出す
	//	m_GameStage1.GetSelect(LineVec, 0, L"Floor");
	//	for (auto& v : LineVec) {
	//		//トークン（カラム）の配列
	//		vector<wstring> Tokens;
	//		//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
	//		Util::WStrToTokenVector(Tokens, v, L',');
	//		//各トークン（カラム）をスケール、回転、位置に読み込む
	//		Vec3 Scale(
	//			(float)_wtof(Tokens[7].c_str()),
	//			(float)_wtof(Tokens[8].c_str()),
	//			(float)_wtof(Tokens[9].c_str())
	//		);
	//		Vec3 Rot;
	//		//回転は「XM_PIDIV2」の文字列になっている場合がある
	//		Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
	//		Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
	//		Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

	//		Vec3 Pos(
	//			(float)_wtof(Tokens[1].c_str()),
	//			(float)_wtof(Tokens[2].c_str()),
	//			(float)_wtof(Tokens[3].c_str())
	//		);

	//		//各値がそろったのでオブジェクト作成
	//		auto ptrFloor = AddGameObject<TilingFixedBox>(Pos, Rot, Scale, 1.0f, 1.0f, Tokens[10]);
	//		ptrFloor->AddTag(L"Floor");
	//	}
	//	m_GameStage1.GetSelect(LineVec, 0, L"Wall");
	//	for (auto& v : LineVec) {
	//		//トークン（カラム）の配列
	//		vector<wstring> Tokens;
	//		//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
	//		Util::WStrToTokenVector(Tokens, v, L',');
	//		//各トークン（カラム）をスケール、回転、位置に読み込む
	//		Vec3 Scale(
	//			(float)_wtof(Tokens[7].c_str()),
	//			(float)_wtof(Tokens[8].c_str()),
	//			(float)_wtof(Tokens[9].c_str())
	//		);
	//		Vec3 Rot;
	//		//回転は「XM_PIDIV2」の文字列になっている場合がある
	//		Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
	//		Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
	//		Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

	//		Vec3 Pos(
	//			(float)_wtof(Tokens[1].c_str()),
	//			(float)_wtof(Tokens[2].c_str()),
	//			(float)_wtof(Tokens[3].c_str())
	//		);

	//		//各値がそろったのでオブジェクト作成
	//		auto PtrWall = AddGameObject<TilingFixedBox>(Pos, Rot, Scale, Scale.x, Scale.y, Tokens[10]);
	//		PtrWall->AddTag(L"Wall");
	//	}
	//	m_GameStage1.GetSelect(LineVec, 0, L"GoalFloor");
	//	for (auto& v : LineVec) {
	//		//トークン（カラム）の配列
	//		vector<wstring> Tokens;
	//		//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
	//		Util::WStrToTokenVector(Tokens, v, L',');
	//		//各トークン（カラム）をスケール、回転、位置に読み込む
	//		Vec3 Scale(
	//			(float)_wtof(Tokens[7].c_str()),
	//			(float)_wtof(Tokens[8].c_str()),
	//			(float)_wtof(Tokens[9].c_str())
	//		);
	//		Vec3 Rot;
	//		//回転は「XM_PIDIV2」の文字列になっている場合がある
	//		Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
	//		Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
	//		Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

	//		Vec3 Pos(
	//			(float)_wtof(Tokens[1].c_str()),
	//			(float)_wtof(Tokens[2].c_str()),
	//			(float)_wtof(Tokens[3].c_str())
	//		);

	//		//各値がそろったのでオブジェクト作成
	//		auto ptrFloor = AddGameObject<TilingFixedBox>(Pos, Rot, Scale, 1.0f, 1.0f, Tokens[10]);
	//		ptrFloor->AddTag(L"Floor");
	//		ptrFloor->AddTag(L"Goal");
	//	}
	//	m_GameStage1.GetSelect(LineVec, 0, L"StageDoor");
	//	for (auto& v : LineVec) {
	//		//トークン（カラム）の配列
	//		vector<wstring> Tokens;
	//		//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
	//		Util::WStrToTokenVector(Tokens, v, L',');
	//		//各トークン（カラム）をスケール、回転、位置に読み込む
	//		Vec3 Scale(
	//			(float)_wtof(Tokens[7].c_str()),
	//			(float)_wtof(Tokens[8].c_str()),
	//			(float)_wtof(Tokens[9].c_str())
	//		);
	//		Vec3 Rot;
	//		//回転は「XM_PIDIV2」の文字列になっている場合がある
	//		Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
	//		Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
	//		Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

	//		Vec3 Pos(
	//			(float)_wtof(Tokens[1].c_str()),
	//			(float)_wtof(Tokens[2].c_str()),
	//			(float)_wtof(Tokens[3].c_str())
	//		);

	//		//各値がそろったのでオブジェクト作成
	//		auto ptrFloor = AddGameObject<Door>(Pos, Rot, Scale, Tokens[10]);
	//		ptrFloor->AddTag(L"StageDoor");
	//	}

	//}

	//void GameStageHemmi::CreateGimmick()
	//{
	//	CreateSharedObjectGroup(L"Door");
	//	CreateSharedObjectGroup(L"Switch");
	//	vector<wstring> LineVec;
	//	m_GameStage1.GetSelect(LineVec, 0, L"Door");
	//	for (auto& v : LineVec) {
	//		//トークン（カラム）の配列
	//		vector<wstring> Tokens;
	//		//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
	//		Util::WStrToTokenVector(Tokens, v, L',');
	//		//各トークン（カラム）をスケール、回転、位置に読み込む
	//		Vec3 Scale(
	//			(float)_wtof(Tokens[7].c_str()),
	//			(float)_wtof(Tokens[8].c_str()),
	//			(float)_wtof(Tokens[9].c_str())
	//		);
	//		Vec3 Rot;
	//		//回転は「XM_PIDIV2」の文字列になっている場合がある
	//		Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
	//		Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
	//		Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

	//		Vec3 Pos(
	//			(float)_wtof(Tokens[1].c_str()),
	//			(float)_wtof(Tokens[2].c_str()),
	//			(float)_wtof(Tokens[3].c_str())
	//		);

	//		float Switch = (float)_wtof(Tokens[10].c_str());
	//		int number = (float)_wtof(Tokens[11].c_str());

	//		//各値がそろったのでオブジェクト作成
	//		auto door = AddGameObject<GimmickDoor>(Pos, Rot, Scale, Scale.x, Scale.y, Switch, number, Tokens[12]);
	//	}

	//	m_GameStage1.GetSelect(LineVec, 0, L"Switch");
	//	for (auto& v : LineVec) {
	//		vector<wstring> Tokens;
	//		Util::WStrToTokenVector(Tokens, v, L',');
	//		Vec3 Scale(
	//			(float)_wtof(Tokens[7].c_str()),
	//			(float)_wtof(Tokens[8].c_str()),
	//			(float)_wtof(Tokens[9].c_str())
	//		);
	//		Vec3 Rot;
	//		Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
	//		Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
	//		Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

	//		Vec3 Pos(
	//			(float)_wtof(Tokens[1].c_str()),
	//			(float)_wtof(Tokens[2].c_str()),
	//			(float)_wtof(Tokens[3].c_str())
	//		);

	//		float Button = (float)_wtof(Tokens[10].c_str());
	//		int number = (float)_wtof(Tokens[11].c_str());

	//		AddGameObject<GimmickButton>(Pos, Rot, Scale, Button, number, Tokens[12]);
	//	}
	//}

	void GameStageHemmi::CreateEnemy()
	{
		CreateSharedObjectGroup(L"Enemy");
		//vector<wstring> LineVec;
		//m_GameStage1.GetSelect(LineVec, 0, L"Enemy");
		//for (auto& v : LineVec) {
		//	vector<wstring> Tokens;
		//	Util::WStrToTokenVector(Tokens, v, L',');
		//	Vec3 Scale(
		//		(float)_wtof(Tokens[7].c_str()),
		//		(float)_wtof(Tokens[8].c_str()),
		//		(float)_wtof(Tokens[9].c_str())
		//	);
		//	Vec3 Rot;
		//	Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
		//	Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
		//	Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

		//	Vec3 Pos(
		//		(float)_wtof(Tokens[1].c_str()),
		//		(float)_wtof(Tokens[2].c_str()),
		//		(float)_wtof(Tokens[3].c_str())
		//	);

		auto Pos = Vec3(-10.0f, 5.0f, 0.0f);
		auto Rot = Vec3(0.0f, 0.0f, 0.0f);
		auto Scale = Vec3(3.0f, 3.0f, 3.0f);
		auto player = GetSharedGameObject<Player>(L"Player");
		auto enemy = AddGameObject<Enemy>(Pos, Rot, Scale, Enemy::rightMove, Enemy::stay, player);
		auto group = GetSharedObjectGroup(L"Enemy");

		group->IntoGroup(enemy);
		
	}
	//void GameStageHemmi::OnCreate() {
	//	try {
	//		wstring Datadir;
	//		App::GetApp()->GetDataDirectory(Datadir);
	//		//CSVパスを取得
	//		m_GameStage1.SetFileName(Datadir + L"CSV/" + L"GameStage.csv");
	//		m_GameStage1.ReadCsv();
	//		//ビューとライトの作成
	//		CreateViewLight();
	//		CreateGameBox();
	//		CreateGimmick();

	//		CreatePlayer();
	//		auto player = GetSharedGameObject<Player>(L"Player");
	//		AddGameObject<SpriteHealth>(player);
	//		AddGameObject<SpriteCharge>(player);
	//		CreateFixedBox();
	//		//CreateSprite();
	//		CreateEnemy();
	//		//AddGameObject<GameOverSprite>();
	//	}
	//	catch (...) {
	//		throw;
	//	}
	//}

}
//end basecross
