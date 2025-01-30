/*!
@file Character.cpp
@brief キャラクターなど実体
鎌田大輝
*/

#include "stdafx.h"
#include "Project.h"
#include "StageManager.h"

namespace basecross {


	void StageManager::CreateViewLight()
	{
		m_OpeningCameraView = ObjectFactory::Create<SingleView>(GetTypeStage<GameStage>());
		auto ptrOpeningCamera = ObjectFactory::Create<OpeningCamera>();
		m_OpeningCameraView->SetCamera(ptrOpeningCamera);
		m_EndingCameraView = ObjectFactory::Create<SingleView>(GetTypeStage<GameStage>());
		auto ptrEndingCamera = ObjectFactory::Create<EndingCamera>();
		m_EndingCameraView->SetCamera(ptrEndingCamera);
		// カメラの設定
		m_MyCameraView = ObjectFactory::Create<SingleView>(GetTypeStage<GameStage>());
		auto camera = ObjectFactory::Create<MainCamera>();
		m_MyCameraView->SetCamera(camera);

		GetStage()->SetView(m_OpeningCameraView);

		//マルチライトの作成
		auto light = GetStage()->CreateLight<MultiLight>();
		light->SetDefaultLighting(); //デフォルトのライティングを指定
	}

	void StageManager::CreatePlayer()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		m_StageName = scene->GetSelectedMap();
		vector<Vec3> plVec;

		if (m_StageName == L"GameStage.csv")
		{
			plVec = {
				Vec3(80.0f, 5.0f,0.0f),
				Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f),
				Vec3(3.0f, 3.0f, 3.0f)
			};
		}
		else if (m_StageName == L"Stagedata1.csv")
		{
			plVec = {
				Vec3(80.0f, 5.0f,0.0f),
				Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f),
				Vec3(3.0f, 3.0f, 3.0f)
			};
		}
		else if (m_StageName == L"Stagedata2.csv")
		{
			plVec = {
				Vec3(80.0f, 5.0f,0.0f),
				Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f),
				Vec3(3.0f, 3.0f, 3.0f)
			};
		}
		else if (m_StageName == L"Stagedata3.csv")
		{
			plVec = {
				Vec3(80.0f, 5.0f,0.0f),
				Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f),
				Vec3(3.0f, 3.0f, 3.0f)
			};
		}
		else if (m_StageName == L"Stagedata4.csv")
		{
			plVec = {
				Vec3(80.0f, 5.0f,0.0f),
				Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f),
				Vec3(3.0f, 3.0f, 3.0f)
			};
		}
		else if (m_StageName == L"ToTitle")
		{
			plVec = {
				Vec3(+20.0f, 5.0f,0.0f),
				Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f),
				Vec3(3.0f, 3.0f, 3.0f)
			};
		}
		//プレーヤーの作成
		shared_ptr<GameObject> ptrPlayer = GetStage()->AddGameObject<Player>(plVec[0], plVec[1], plVec[2]);
		GetStage()->AddGameObject<SpritePlayerUI>(dynamic_pointer_cast<Player>(ptrPlayer), L"PLAYERUI", 1);
		//シェア配列にプレイヤーを追加
		GetStage()->SetSharedGameObject(L"Player", ptrPlayer);
		auto playerPos = ptrPlayer->GetComponent<Transform>();
	}

	void StageManager::CreateFixedBox()
	{
		//CSVの行単位の配列
		vector<wstring> LineVec;
		//0番目のカラムがL"FixedBox"である行を抜き出す
		m_GameStage.GetSelect(LineVec, 0, L"Floor");
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
			auto ptrFloor = GetStage()->AddGameObject<TilingFixedBox>(Pos, Rot, Scale, Scale.x / 1, Scale.z / 1, Tokens[10]);
			ptrFloor->AddTag(L"Floor");
			ptrFloor->GetComponent<PNTStaticDraw>()->SetOwnShadowActive(true);
		}
		m_GameStage.GetSelect(LineVec, 0, L"Wall");
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
			auto PtrWall = GetStage()->AddGameObject<TilingFixedBox>(Pos, Rot, Scale, Scale.x, Scale.y, Tokens[10]);
			PtrWall->AddTag(L"Wall");
			PtrWall->GetComponent<PNTStaticDraw>()->SetOwnShadowActive(true);

		}
		m_GameStage.GetSelect(LineVec, 0, L"GoalFloor");
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
			auto ptrFloor = GetStage()->AddGameObject<TilingFixedBox>(Pos, Rot, Scale, Scale.x / 1, Scale.z / 1, Tokens[10]);
			ptrFloor->AddTag(L"Floor");
			ptrFloor->AddTag(L"Goal");
		}
		m_GameStage.GetSelect(LineVec, 0, L"GoalBeforeFloor");
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
			auto ptrFloor = GetStage()->AddGameObject<TilingFixedBox>(Pos, Rot, Scale, Scale.x / 1, Scale.z / 1, Tokens[10]);
			ptrFloor->AddTag(L"Floor");
			ptrFloor->AddTag(L"GoalBefore");
		}

	}

	void StageManager::CreateGimmick()
	{
		GetStage()->CreateSharedObjectGroup(L"Door");
		GetStage()->CreateSharedObjectGroup(L"Switch");
		GetStage()->CreateSharedObjectGroup(L"StageDoor");
		GetStage()->CreateSharedObjectGroup(L"GimmickUp");
		vector<wstring> LineVec;
		m_GameStage.GetSelect(LineVec, 0, L"Door");
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
			auto door = GetStage()->AddGameObject<GimmickDoor>(Pos, Rot, Scale, Scale.x, Scale.y, Switch, number, Tokens[12]);
		}

		m_GameStage.GetSelect(LineVec, 0, L"Switch");
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

			GetStage()->AddGameObject<GimmickButton>(Pos, Rot, Scale, Button, number, Tokens[12]);
		}
		m_GameStage.GetSelect(LineVec, 0, L"StageDoor");
		for (auto& v : LineVec) {
			//トークン（カラム）の配列
			vector<wstring> Tokens;
			//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
			Util::WStrToTokenVector(Tokens, v, L',');
			//各トークン（カラム）をスケール、回転、位置に読み込む
			Vec3 Scale(5, 5, 5);
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
			auto ptrStageDoor = GetStage()->AddGameObject<Door>(Pos, Rot, Scale, Tokens[10]);
			ptrStageDoor->AddTag(L"StageDoor");
			auto group = GetStage()->GetSharedObjectGroup(L"StageDoor");
			group->IntoGroup(ptrStageDoor);
		}
		m_GameStage.GetSelect(LineVec, 0, L"GimmickUp");
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
			int max = (float)_wtof(Tokens[13].c_str());

			//各値がそろったのでオブジェクト作成
			auto door = GetStage()->AddGameObject<GimmickUp>(Pos, Rot, Scale, Scale.x, Scale.y, Switch, number, Tokens[12], max);
		}
		m_GameStage.GetSelect(LineVec, 0, L"DoorGimmick");
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
			Vec3 UV;
			//回転は「XM_PIDIV2」の文字列になっている場合がある
			UV.x = (float)_wtof(Tokens[4].c_str());
			UV.y = (float)_wtof(Tokens[5].c_str());
			UV.z = (float)_wtof(Tokens[6].c_str());

			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);
			float number = (float)_wtof(Tokens[10].c_str());
			//各値がそろったのでオブジェクト作成
			auto doorgimmick = GetStage()->AddGameObject<DoorGimmick>(Pos, UV, Scale, number, Tokens[11]);
		}
	}

	void StageManager::CreateEnemy() {
		if (m_nowGameStatus == GameStatus::GAME_PLAYING)
		{
			GetStage()->CreateSharedObjectGroup(L"Enemy");
			vector<wstring> LineVec;
			m_GameStage.GetSelect(LineVec, 0, L"Enemy");
			for (auto& v : LineVec) {
				Enemy::State stateBefore;
				Enemy::State stateAfter;
				vector<wstring> Tokens;
				Util::WStrToTokenVector(Tokens, v, L',');
				Vec3 Scale(3, 3, 3);
				Vec3 Rot;
				Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
				Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
				Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

				Vec3 Pos(
					(float)_wtof(Tokens[1].c_str()),
					(float)_wtof(Tokens[2].c_str()),
					(float)_wtof(Tokens[3].c_str())
				);
				auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
				if (Tokens[10] == L"Enemy::rightMove" && Tokens[11] == L"Enemy::stay") {
					stateBefore = Enemy::rightMove;
					stateAfter = Enemy::stay;
					auto enemy = GetStage()->AddGameObject<EnemyChase>(Pos, Rot, Scale, stateBefore, stateAfter, player);
					auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
					group->IntoGroup(enemy);
				}
				else if (Tokens[10] == L"Enemy::fly" && Tokens[11] == L"Enemy::fixedStay") {
					stateBefore = Enemy::fly;
					stateAfter = Enemy::fixedStay;
					auto enemy = GetStage()->AddGameObject<Enemy>(Pos, Rot, Scale, stateBefore, stateAfter, player);
					auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
					group->IntoGroup(enemy);
				}
				else if (Tokens[10] == L"Enemy::bullet" && Tokens[11] == L"Enemy::stay") {
					stateBefore = Enemy::bullet;
					stateAfter = Enemy::stay;
					auto enemy = GetStage()->AddGameObject<Enemy>(Pos, Rot, Scale, stateBefore, stateAfter, player);
					auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
					group->IntoGroup(enemy);
				}
				else if (Tokens[10] == L"Enemy::bulletMove" && Tokens[11] == L"Enemy::stay") {
					stateBefore = Enemy::bulletMove;
					stateAfter = Enemy::stay;
					auto enemy = GetStage()->AddGameObject<Enemy>(Pos, Rot, Scale, stateBefore, stateAfter, player);
					auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
					group->IntoGroup(enemy);
					if (Tokens[12] == L"X")
					{
						Vec2 X = Vec2(1, 0);
						enemy->SetBulletDirec(Vec2(X));
						if (Pos.z < 0)
						{
							enemy->GetComponent<Transform>()->SetRotation(0, 0, 0);
						}
						else {
							enemy->GetComponent<Transform>()->SetRotation(0, 135, 0);
						}
					}
					else if (Tokens[12] == L"Z")
					{
						Vec2 Z = Vec2(0, 1);
						enemy->SetBulletDirec(Vec2(Z));
					}
				}
				else if (Tokens[10] == L"Enemy::slide" && Tokens[11] == L"Enemy::stay") {
					stateBefore = Enemy::slide;
					stateAfter = Enemy::stay;
					auto enemy = GetStage()->AddGameObject<Enemy>(Pos, Rot, Scale, stateBefore, stateAfter, player);
					auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
					group->IntoGroup(enemy);

				}
			}
		}
		if (m_nowGameStatus == GameStatus::TEST_PLAY)
		{
			GetStage()->CreateSharedObjectGroup(L"Enemy");
			vector<wstring> LineVec;
			m_GameStage.GetSelect(LineVec, 0, L"Enemy");
			for (auto& v : LineVec) {
				Enemy::State stateBefore;
				Enemy::State stateAfter;
				vector<wstring> Tokens;
				Util::WStrToTokenVector(Tokens, v, L',');
				Vec3 Scale(3, 3, 3);
				Vec3 Rot;
				Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
				Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
				Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

				Vec3 Pos(
					(float)_wtof(Tokens[1].c_str()),
					(float)_wtof(Tokens[2].c_str()),
					(float)_wtof(Tokens[3].c_str())
				);
				auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
				auto enemy = GetStage()->AddGameObject<Enemy>(Pos, Rot, Scale, Enemy::rightMove, Enemy::stay, player);
				auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
				group->IntoGroup(enemy);
			}

		}

	}
	void StageManager::OnCreate() {
		try {
			//エフェクト作成
			m_EfkInterface = ObjectFactory::Create<EfkInterface>();
			m_titleSprite = GetStage()->AddGameObject<BlinkingSprite>(L"TITLETEXT", true, Vec2(640.0f, 410.0f), Vec3(0.0f, -250.0f, 0.1f), 3.5f);

			auto& app = App::GetApp();
			auto scene = app->GetScene<Scene>();
			// mediaパスを取得
			auto path = app->GetDataDirWString();
			// texturesパスを取得
			auto csvPath = path + L"CSV/";
			m_StageName = scene->GetSelectedMap();
			//CSVパスを取得
			if (m_StageName != L"")
			{
				if (m_StageName == L"ToTitle")
				{
					m_GameStage.SetFileName(csvPath + L"TestStage.csv");
					m_GameStage.ReadCsv();
				}
				else {
					m_GameStage.SetFileName(csvPath + m_StageName);
					m_GameStage.ReadCsv();
				}
			}
		}
		catch (...) {
			throw;
		}
	}

	void StageManager::OnUpdate()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		switch (m_nowGameStatus) {
		case GameStatus::TITLE:
			m_titleSprite->SetDrawActive(true);
			if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A || KeyState.m_bPressedKeyTbl[VK_SPACE])
			{
				m_startFlag = true;
				PlaySE(L"DecisionSE", 0, 1.0f);
			}
			if (m_startFlag)
			{
				m_updateTime += ElapsedTime;
				if (m_updateTime > 0.5f)
				{
					PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToSlelctStage");
				}
			}
			break;
		case GameStatus::SELECT:
			m_titleSprite->SetDrawActive(false);
			if (scene->m_select == 0)
			{
				if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A || KeyState.m_bPressedKeyTbl[VK_SPACE])
				{
					PlaySE(L"DecisionSE", 0, 1.0f);
					PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
				}
				if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_START || KeyState.m_bPressedKeyTbl[VK_TAB])
				{
					PlaySE(L"DecisionSE", 0, 1.0f);
					PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTestStage");
				}

			}
			else if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A || KeyState.m_bPressedKeyTbl[VK_SPACE])
			{
				PlaySE(L"DecisionSE", 0, 1.0f);
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToLoad");
			}
			break;
		case GameStatus::GAME_PLAYING: 
			if (m_CameraSelect == CameraSelect::openingCamera)
			{
				auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
				auto& vec = group->GetGroupVector();
				for (auto v : vec)
				{
					auto shObj = v.lock();
					if (shObj)
					{
						shObj->SetUpdateActive(false);
					}
				}
			}
			if (m_CameraSelect == CameraSelect::myCamera)
			{
				auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
				auto& vec = group->GetGroupVector();
				for (auto v : vec)
				{
					auto shObj = v.lock();
					if (shObj->GetUpdateActive() == false)
					{
						shObj->SetUpdateActive(true);
					}
				}
				UImake();
				GoalJudge();
				GameOverJudge();
			}
			if (m_CameraSelect == CameraSelect::endingCamera)
			{
				auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
				auto& vec = group->GetGroupVector();
				for (auto v : vec)
				{
					auto shObj = v.lock();
					if (shObj->GetUpdateActive() == true)
					{
						shObj->SetUpdateActive(false);
					}
				}
				GoalJudge();
			}
			break;

		case GameStatus::TEST_PLAY:
			if (m_CameraSelect == CameraSelect::openingCamera)
			{
				auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
				auto& vec = group->GetGroupVector();
				for (auto v : vec)
				{
					auto shObj = v.lock();
					if (shObj)
					{
						shObj->SetUpdateActive(false);
					}
				}
			}
			if (m_CameraSelect == CameraSelect::myCamera)
			{
				auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
				auto& vec = group->GetGroupVector();
				for (auto v : vec)
				{
					auto shObj = v.lock();
					if (shObj->GetUpdateActive() == false)
					{
						shObj->SetUpdateActive(true);
					}
				}
				UImake();
				GoalJudge();
				GameOverJudge();
				OnDraw();
			}
			break;

		default:
			break;
		}

	}

	void StageManager::OnDraw()
	{
	}

	void StageManager::CreateSprite()
	{

		m_TextDraw = GetStage()->AddGameObject<Sprite>(L"GameClearTEXT", true, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.0f));
		m_TextDraw->SetDrawLayer(3);

		m_SpriteDraw = GetStage()->AddGameObject<Sprite>(L"CLEARBackGround", true, Vec2(640.0f, 400.0f), Vec3(.0f, 0.0f, 0.0f));
		m_SpriteDraw->SetDrawLayer(1);

		m_StageUI = GetStage()->AddGameObject<Sprite>(L"GameStageUI", true, Vec2(640.0f, 400.0f), Vec3(10, 0, 0.0f));
		m_StageUI->SetDrawLayer(3);

		m_kakaeruUI = GetStage()->AddGameObject<Sprite>(L"Kakaeru", true, Vec2(150.0f, 22.5f), Vec3(300, 0, 0.0f));
		m_kakaeruUI->SetDrawLayer(3);

		m_blowUI = GetStage()->AddGameObject<Sprite>(L"blowUI", true, Vec2(150.0f, 22.5f), Vec3(300, 0, 0.0f));
		m_blowUI->SetDrawLayer(3);

		m_nextStageUI = GetStage()->AddGameObject<Sprite>(L"NextStage", true, Vec2(400.0f, 300.0f), Vec3(1000.0f, -275.0f, 0.0f));
		m_nextStageUI->SetDrawLayer(3);

		m_clearSelectStage = GetStage()->AddGameObject<Sprite>(L"ClearSelectStage", true, Vec2(400.0f, 300.0f), Vec3(-1000.0f, -200.0f, 0.0f));
		m_clearSelectStage->SetDrawLayer(3);

		m_retryStageUI = GetStage()->AddGameObject<Sprite>(L"Retry", true, Vec2(400.0f, 300.0f), Vec3(1000.0f, -275.0f, 0.0f));
		m_retryStageUI->SetDrawLayer(4);

		m_overSelectStage = GetStage()->AddGameObject<Sprite>(L"OverSelectStage", true, Vec2(400.0f, 300.0f), Vec3(-1000.0f, -200.0f, 0.0f));
		m_overSelectStage->SetDrawLayer(4);

		m_SelectCharge = GetStage()->AddGameObject<SelectCharge>(L"PauseSelectCharge", false, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.0f));
		m_SelectCharge->SetDrawLayer(4);

		m_TitleCharge = GetStage()->AddGameObject<SelectCharge>(L"PauseTitleCharge", false, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.0f));
		m_TitleCharge->SetDrawLayer(4);

		m_titleSprite->SetDrawActive(false);
		m_TextDraw->SetDrawActive(false);
		m_SpriteDraw->SetDrawActive(false);
		m_nextStageUI->SetDrawActive(false);
		m_clearSelectStage->SetDrawActive(false);
		m_retryStageUI->SetDrawActive(false);
		m_overSelectStage->SetDrawActive(false);
		m_SelectCharge->SetDrawActive(false);
		m_TitleCharge->SetDrawActive(false);
		m_kakaeruUI->SetDrawActive(false);
		m_blowUI->SetDrawActive(false);

		ToOpeningCamera();
	}

	void StageManager::GoalJudge()
	{

		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		auto nextUI = m_nextStageUI->GetComponent<Transform>();
		auto selectUI = m_clearSelectStage->GetComponent<Transform>();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		float totaltime = 1.0f;
		Easing<Vec3> easing;


		auto playerSh = GetStage()->GetSharedGameObject<Player>(L"Player");
		m_Goaltrue = playerSh->GetArrivedGoal();
		if (m_Goaltrue)
		{
			if (m_CameraSelect != CameraSelect::endingCamera) {
				ToEndingCamera();
			}

			if (m_select == 0)
			{
				if (m_Flag)
				{
					PlaySE(L"GameClearSE", 0, 1.0f);
					m_Flag = false;
				}
				else {
					m_clearTotalTime += ElapsedTime;
				}
				if (m_fadeoutFlag && m_clearTotalTime >= 6.0f) {
					m_BGfade = GetStage()->AddGameObject<FadeOut>();
					m_BGfade->SetDrawLayer(3);
					m_fadeoutFlag = false;
				}

				GetTypeStage<GameStage>()->OnDestroy();
				m_kakaeruUI->SetDrawActive(false);

				m_StageUI->SetDrawActive(false);
				m_TextDraw->SetDrawActive(true);
				m_SpriteDraw->SetDrawActive(true);
				m_nextStageUI->SetDrawActive(true);
				m_clearSelectStage->SetDrawActive(true);
				MoveSprite(m_nextStageUI, m_clearSelectStage);
				if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B || KeyState.m_bPressedKeyTbl[VK_SPACE])
				{
					PlaySE(L"Confirm", 0, 1.0f);
					m_select = 1;
					m_totalTime = 0.0f;
				}
				if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A || KeyState.m_bPressedKeyTbl[VK_RETURN])
				{
					PlaySE(L"Confirm", 0, 1.0f);
					m_select = 2;
					m_totalTime = 0.0f;
				}
			}
			SelectMoveSprite(m_nextStageUI, m_clearSelectStage);
		}
	}

	void StageManager::GameOverJudge()
	{
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		auto playerSh = GetStage()->GetSharedGameObject<Player>(L"Player");
		m_Diedtrue = playerSh->GetDied();
		if (m_Diedtrue)
		{
			auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
			auto group2 = GetStage()->GetSharedObjectGroup(L"Door");
			auto& vec = group->GetGroupVector();
			auto& vec2 = group2->GetGroupVector();
			for (auto v : vec,vec2)
			{
				auto shObj = v.lock();
				if (shObj)
				{
					shObj->SetUpdateActive(false);
				}
			}
			if (m_Flag)
			{
				GetTypeStage<GameStage>()->OnDestroy();
				m_StageUI->SetDrawActive(false);
				m_retryStageUI->SetDrawActive(true);
				m_overSelectStage->SetDrawActive(true);
				m_kakaeruUI->SetDrawActive(false);
				GetStage()->AddGameObject<GameOverSprite>();
				m_Flag = false;
			}
			if (m_select == 0)
			{
				MoveSprite(m_retryStageUI, m_overSelectStage);
				if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B || KeyState.m_bPressedKeyTbl[VK_SPACE])
				{
					PlaySE(L"Confirm", 0, 1.0f);
					m_select = 1;
					m_totalTime = 0.0f;
				}
				if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A || KeyState.m_bPressedKeyTbl[VK_RETURN])
				{
					PlaySE(L"Confirm", 0, 1.0f);
					m_select = 3;
					m_totalTime = 0.0f;
				}
			}
			SelectMoveSprite(m_retryStageUI, m_overSelectStage);
		}
	}

	void StageManager::SetPushState(const int PushState)
	{
		m_PushState = PushState;
	}

	int StageManager::GetPushState()
	{
		return m_PushState;
	}

	void StageManager::SetGameStageSelect(const wstring& m_csvFail)
	{
		m_StageName = m_csvFail;
	}

	void StageManager::MoveSprite(const shared_ptr<GameObject> nextStageUI, const shared_ptr<GameObject> SelectStageUI)
	{
		auto nextStage = nextStageUI->GetComponent<Transform>();
		auto SelectStage = SelectStageUI->GetComponent<Transform>();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		float totaltime = 1.0f;
		m_totalTime += ElapsedTime;
		Easing<Vec3> easing;
		Vec3 nextPos = easing.EaseInOut(EasingType::Exponential, Vec3(1000.0f, -275.0f, 0.0f), Vec3(150.0f, -275.0f, 0.0f), m_totalTime, totaltime);
		nextStage->SetPosition(nextPos);
		Vec3 selecttPos = easing.EaseInOut(EasingType::Exponential, Vec3(-1000.0f, -200.0f, 0.0f), Vec3(-200.0f, -200.0f, 0.0f), m_totalTime, totaltime);
		SelectStage->SetPosition(selecttPos);

	}

	void StageManager::SelectMoveSprite(const shared_ptr<GameObject> nextStageUI, const shared_ptr<GameObject> SelectStage)
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();

		float ElapsedTime = App::GetApp()->GetElapsedTime();
		float totaltime = 1.0f;
		Easing<Vec3> easing;

		auto selectUI = SelectStage->GetComponent<Transform>();
		auto nextStage = nextStageUI->GetComponent<Transform>();

		if (m_select == 1)
		{
			m_totalTime += ElapsedTime;
			Vec3 select = easing.EaseInOut(EasingType::Exponential, Vec3(-200.0f, -200.0f, 0.0f), Vec3(-1000.0f, -200.0f, 0.0f), m_totalTime, totaltime);
			selectUI->SetPosition(select);
			if (m_totalTime > 1.0f)
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToSlelctStage");
			}
		}
		else if (m_select == 2)
		{
			m_totalTime += ElapsedTime;
			Vec3 m_nxsttPos = easing.EaseInOut(EasingType::Exponential, Vec3(150.0f, -275.0f, 0.0f), Vec3(1000.0f, -275.0f, 0.0f), m_totalTime, totaltime);
			nextStage->SetPosition(m_nxsttPos);
			if (m_totalTime > 1.0f)
			{
				scene->SetSelectedMap(scene->m_select + 1);
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToLoad");
			}
		}
		else if (m_select == 3) {
			m_totalTime += ElapsedTime;
			Vec3 m_nxsttPos = easing.EaseInOut(EasingType::Exponential, Vec3(150.0f, -275.0f, 0.0f), Vec3(1000.0f, -275.0f, 0.0f), m_totalTime, totaltime);
			nextStage->SetPosition(m_nxsttPos);
			if (m_totalTime > 1.0f)
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToLoad");
			}
		}
	}

	wstring StageManager::GetGameStageSelect()
	{
		return m_StageName;
	}

	// 現在のゲームステータスを取得
	int StageManager::GetNowGameStatus() {
		return m_nowGameStatus;
	}

	// 現在のゲームステータスを設定
	void StageManager::SetNowGameStatus(int afterGameStatus) {
		m_nowGameStatus = afterGameStatus;
	}

	int StageManager::GetNowCameraStatus() {
		return m_nowGameStatus;
	}

	void StageManager::ToMainCamera()
	{
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto ptrCamera = dynamic_pointer_cast<MainCamera>(m_MyCameraView->GetCamera());
		auto PlayPos = PtrPlayer->AddComponent<Transform>()->GetPosition();
		Vec3 CameraEndPos = Vec3(90.0f, 15.0f, 0);
		if (ptrCamera) {
			GetStage()->SetView(m_MyCameraView);
			auto cameraObject = GetStage()->AddGameObject<CameraCollision>();
			ptrCamera->SetTargetObject(PtrPlayer);
			ptrCamera->SetTargetToAt(Vec3(0, 3.0f, 0));
			m_CameraSelect = CameraSelect::myCamera;
		}

	}
	void StageManager::ToOpeningCamera()
	{
		GetStage()->AddGameObject<FadeIn>();
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayPos = PtrPlayer->AddComponent<Transform>()->GetPosition();
		Vec3 CameraPos = Vec3(PlayPos.x + 10.0f, PlayPos.y + 5.0f, PlayPos.z);
		Vec3 CameraStartEndPos = Vec3(PlayPos.x - 5.0f, PlayPos.y + 5.0f, PlayPos.z);
		Vec3 CameraEndPos = Vec3(PlayPos.x + 15.0f, PlayPos.y + 10.0f, PlayPos.z);
		Vec3 PlayEndpos = Vec3(PlayPos.x - 5.0f, PlayPos.y, PlayPos.z);
		Vec3 PlayStartpos = Vec3(PlayPos.x, PlayPos.y + 3.0f, PlayPos.z);
		auto view = GetStage()->CreateView<SingleView>();
		//カメラのオープニングの移動(最初のカメラの位置、最後のカメラの位置、
// 　　　　　　　　　　　　　最初に見てる所、最後に見てる所、後半最初に見る位置、
// 　　　　　　　　　　　　　かかる時間(多分)、後半最後にいる位置、後半最後に見てる所)
		auto ptrOpeningCameraman = GetStage()->AddGameObject<OpeningCameraman>(CameraPos, CameraStartEndPos,
			PlayStartpos, Vec3(0), PlayEndpos,
			0.0f, CameraEndPos, PlayEndpos);
		//シェア配列にOpeningCameramanを追加
		GetStage()->SetSharedGameObject(L"OpeningCameraman", ptrOpeningCameraman);

		auto ptrOpeningCamera = dynamic_pointer_cast<OpeningCamera>(m_OpeningCameraView->GetCamera());
		if (ptrOpeningCamera) {
			ptrOpeningCamera->SetCameraObject(ptrOpeningCameraman);
			GetStage()->SetView(m_OpeningCameraView);
			m_CameraSelect = CameraSelect::openingCamera;
		}
	}

	void StageManager::ToEndingCamera()
	{
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayPos = PtrPlayer->AddComponent<Transform>()->GetPosition();
		Vec3 fwd = PtrPlayer->GetComponent<Transform>()->GetForward();
		float face = atan2f(fwd.z, fwd.x);
		//位置(eye, at)の決定
		Vec3 pos = Vec3(12.0f, -3.0f, -9.0f);
		Vec3 addpos = Vec3((pos.x * cosf(face)) - (pos.z * sinf(face)), pos.y, (pos.z * cosf(face)) + (pos.x * sinf(face)));
		Vec3 CameraStartPos = PlayPos + addpos;
		pos = Vec3(15.0f, -1.0f, -3.0f);
		addpos = Vec3((pos.x * cosf(face)) - (pos.z * sinf(face)), pos.y, (pos.z * cosf(face)) + (pos.x * sinf(face)));
		Vec3 CameraEndPos = PlayPos + addpos;
		pos = Vec3(-6.0f, 2.5f, 0);
		addpos = Vec3((pos.x * cosf(face)) - (pos.z * sinf(face)), pos.y, (pos.z * cosf(face)) + (pos.x * sinf(face)));
		Vec3 AtStartPos = PlayPos + addpos;
		Vec3 AtEndPos = PlayPos + addpos;
		//カメラのオープニングの移動(最初のカメラの位置、最後のカメラの位置、
// 　　　　　　　　　　　　　最初に見てる所、最後に見てる所、かかる時間)
		auto ptrEndingCameraman = GetStage()->AddGameObject<EndingCameraman>(CameraStartPos, CameraEndPos,
			AtStartPos, AtEndPos,
			0.0f);
		//シェア配列に追加
		GetStage()->SetSharedGameObject(L"EndingCameraman", ptrEndingCameraman);

		auto ptrEndingCamera = dynamic_pointer_cast<EndingCamera>(m_EndingCameraView->GetCamera());
		if (ptrEndingCamera) {
			ptrEndingCamera->SetCameraObject(ptrEndingCameraman);
			GetStage()->SetView(m_EndingCameraView);
			m_CameraSelect = CameraSelect::endingCamera;
		}
	}

	void StageManager::PlaySE(wstring path, float loopcnt, float volume) {
		auto playSE = App::GetApp()->GetXAudio2Manager();
		playSE->Start(path, loopcnt, volume);
	}

	void StageManager::PlayBGM(const wstring& StageBGM)
	{
		m_BGM = m_ptrXA->Start(StageBGM, XAUDIO2_LOOP_INFINITE, 0.8f);
	}

	void StageManager::UImake()
	{
		auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
		auto& vec = group->GetGroupVector();
		for (auto& v : vec)
		{
			auto shObj = v.lock();
			auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
			auto pos = player->GetComponent<Transform>()->GetPosition();
			
			Vec3 enemypos = shObj->GetComponent<Transform>()->GetWorldPosition();
			float lenth = length(enemypos - pos);
			if (lenth < 9.0f) {
				auto enemy = dynamic_pointer_cast<Enemy>(shObj);
				bool overheat = enemy->GetOverHeat();
				if (overheat){
					m_kakaeruUI->SetDrawActive(true);
					m_blowUI->SetDrawActive(true);
					break;
				}
				else {
					m_kakaeruUI->SetDrawActive(false);
					break;
				}
			}
			else {
				m_kakaeruUI->SetDrawActive(false);
			}
		}
	}
}
//end basecross
