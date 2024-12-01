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
				Vec3(75.0f, 5.0f,0.0f),
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
				Vec3(65.0f, 5.0f,0.0f),
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
			auto ptrFloor = GetStage()->AddGameObject<TilingFixedBox>(Pos, Rot, Scale, 1.0f, 1.0f, Tokens[10]);
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
			auto ptrFloor = GetStage()->AddGameObject<TilingFixedBox>(Pos, Rot, Scale, 1.0f, 1.0f, Tokens[10]);
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
			auto ptrFloor = GetStage()->AddGameObject<TilingFixedBox>(Pos, Rot, Scale, 1.0f, 1.0f, Tokens[10]);
			ptrFloor->AddTag(L"Floor");
			ptrFloor->AddTag(L"GoalBefore");
		}

	}

	void StageManager::CreateGimmick()
	{
		GetStage()->CreateSharedObjectGroup(L"Door");
		GetStage()->CreateSharedObjectGroup(L"Switch");
		GetStage()->CreateSharedObjectGroup(L"StageDoor");
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
			Vec3 Scale(5,5,5);
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
	}

	void StageManager::CreateEnemy() {
		GetStage()->CreateSharedObjectGroup(L"Enemy");
		vector<wstring> LineVec;
		m_GameStage.GetSelect(LineVec, 0, L"Enemy");
		for (auto& v : LineVec) {
			Enemy::State stateBefore;
			Enemy::State stateAfter;
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Scale(3,3,3);
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
			if (Tokens[10] == L"Enemy::rightMove")			stateBefore = Enemy::rightMove;
			else if (Tokens[10] == L"Enemy::rightMove")		stateBefore = Enemy::fly;
			if (Tokens[11] == L"Enemy::stay")				stateAfter = Enemy::stay;
			else if (Tokens[11] == L"Enemy::stay")			stateAfter = Enemy::fixedStay;
			auto enemy = GetStage()->AddGameObject<EnemyChase>(Pos, Rot, Scale, stateBefore, stateAfter, player);
			auto group = GetStage()->GetSharedObjectGroup(L"Enemy");
			group->IntoGroup(enemy);
		}
	}

	void StageManager::OnCreate() {
		try {
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
				m_GameStage.SetFileName(csvPath + m_StageName);
				m_GameStage.ReadCsv();
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

		switch (m_nowGameStatus) {
		case GameStatus::TITLE:
			if (cntlVec[0].wPressedButtons || KeyState.m_bPressedKeyTbl[VK_SPACE])
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToSlelctStage");
			}
			break;
		case GameStatus::SELECT:

			if (cntlVec[0].wPressedButtons || KeyState.m_bPressedKeyTbl[VK_SPACE])
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
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
				GoalJudge();
				GameOverJudge();
			}
			break;
		default:
			break;
		}

	}

	void StageManager::CreateSprite()
	{
		m_TextDraw = GetStage()->AddGameObject<Sprite>(L"GameClearTEXT", true, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.3f));
		m_SpriteDraw = GetStage()->AddGameObject<Sprite>(L"CLEARBackGround", true, Vec2(640.0f, 400.0f), Vec3(0.0f, 0.0f, 0.3f));
		m_TextDraw->SetDrawActive(false);
		m_SpriteDraw->SetDrawActive(false);
		ToOpeningCamera();
	}

	void StageManager::GoalJudge()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();

		auto playerSh = GetStage()->GetSharedGameObject<Player>(L"Player");
		m_Goaltrue = playerSh->GetArrivedGoal();
		if (m_Goaltrue)
		{
			m_TextDraw->SetDrawActive(true);
			m_SpriteDraw->SetDrawActive(true);
			if (cntlVec[0].wPressedButtons && XINPUT_GAMEPAD_B|| KeyState.m_bPressedKeyTbl[VK_SPACE])
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
				GetTypeStage<GameStage>()->OnDestroy();
			}
			if (cntlVec[0].wPressedButtons && XINPUT_GAMEPAD_A || KeyState.m_bPressedKeyTbl[VK_RETURN])
			{
				scene->SetSelectedMap(scene->m_select + 1);
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
				GetTypeStage<GameStage>()->OnDestroy();
			}

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
			if (m_Flag)
			{
				GetStage()->AddGameObject<GameOverSprite>();
				m_Flag = false;
			}
			if (cntlVec[0].wPressedButtons && XINPUT_GAMEPAD_B || KeyState.m_bPressedKeyTbl[VK_SPACE])
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
				GetTypeStage<GameStage>()->OnDestroy();
			}
			if (cntlVec[0].wPressedButtons && XINPUT_GAMEPAD_A || KeyState.m_bPressedKeyTbl[VK_RETURN])
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
				GetTypeStage<GameStage>()->OnDestroy();
			}
		}
	}
	void StageManager::SetGameStageSelect(const wstring& m_csvFail)
	{
		m_StageName = m_csvFail;
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
	//���݂̃J�����X�e�[�^�X��
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
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayPos =  PtrPlayer->AddComponent<Transform>()->GetPosition();
		Vec3 CameraPos = Vec3(PlayPos.x + 5.0f, PlayPos.y, PlayPos.z);
		Vec3 CameraEndPos = Vec3(PlayPos.x + 5.0f, PlayPos.y + 15.0f, PlayPos.z);
		Vec3 PlayEndpos = Vec3(PlayPos.x - 5.0f, PlayPos.y, PlayPos.z);
		auto view = GetStage()->CreateView<SingleView>();
		//カメラのオープニングの移動(最初のカメラの位置、最後のカメラの位置、
// 　　　　　　　　　　　　　最初に見てる所、最後に見てる所、後半最初に見る位置、
// 　　　　　　　　　　　　　かかる時間(多分)、後半最後にいる位置、後半最後に見てる所)
		auto ptrOpeningCameraman = GetStage()->AddGameObject<OpeningCameraman>(CameraPos, CameraPos,
			PlayPos, PlayEndpos, PlayEndpos,
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
}
//end basecross
