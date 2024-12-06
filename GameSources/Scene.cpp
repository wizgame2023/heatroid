
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	Scene::~Scene() {
	}
	
	void Scene::CreateResources() {

		auto& app = App::GetApp();

		// mediaパスを取得
		auto path = app->GetDataDirWString();
		// texturesパスを取得
		auto texPath = path + L"Textures/";
		// Modelsパス
		auto mdlPath = path + L"Models/";
		// Soundsパス
		auto sdPath = path + L"Sounds/";

		// プレイヤー
		auto modelMesh = MeshResource::CreateBoneModelMesh(mdlPath, L"HR.bmf");
		app->RegisterResource(L"PLAYER", modelMesh);

		//敵のモデル
		auto enemyAruki = MeshResource::CreateBoneModelMesh(mdlPath, L"ZAKO_ARUKU.bmf");
		app->RegisterResource(L"ENEMYARUKU", enemyAruki);
		auto enemyTobi = MeshResource::CreateBoneModelMesh(mdlPath, L"ZAKO_TOBU.bmf");
		app->RegisterResource(L"ENEMYTOBU", enemyTobi);



		//ステージオブジェクトのモデル
		auto door = MeshResource::CreateBoneModelMesh(mdlPath, L"Door.bmf");
		app->RegisterResource(L"DOOR", door);

		auto blackSwitch = MeshResource::CreateStaticModelMesh(mdlPath, L"BLUCK_TILE.bmf");
		app->RegisterResource(L"BLUCKSWITCH", blackSwitch);
		auto blueSwitch = MeshResource::CreateStaticModelMesh(mdlPath, L"BLUE_TILE.bmf");
		app->RegisterResource(L"BLUESWITCH", blueSwitch);
		auto RedSwitch = MeshResource::CreateStaticModelMesh(mdlPath, L"RED_TILE.bmf");
		app->RegisterResource(L"REDSWITCH", RedSwitch);

		// 仮テクスチャ
		app->RegisterTexture(L"White", texPath + L"White.png");
		app->RegisterTexture(L"FIRE", texPath + L"fire_kari.png");
		app->RegisterTexture(L"PLAYERUI", texPath + L"PlayerMeterBack.png");
		app->RegisterTexture(L"PLAYERUI2", texPath + L"PlayerMeterFront.png");
		app->RegisterTexture(L"HEALTH", texPath + L"PlayerMeterHP.png");
		app->RegisterTexture(L"CHARGE", texPath + L"PlayerMeterCharge.png");
		//ロード画面テクスチャ
		app->RegisterTexture(L"LoadBG", texPath + L"LoadBG.png");
		app->RegisterTexture(L"LoadText", texPath + L"loadtext_wip.png");
		app->RegisterTexture(L"LoadCircle", texPath + L"LoadCircle.png");
		app->RegisterTexture(L"LoadBar", texPath + L"LoadBar.png");
		//ステージオブジェクトテクスチャ
		app->RegisterTexture(L"Wall", texPath + L"UV_WALL.png");
		app->RegisterTexture(L"Floor", texPath + L"UV_FLOOR.png");
		app->RegisterTexture(L"AreaDoorBLUCK", texPath + L"UV_AD_BLUCK.png");
		app->RegisterTexture(L"AreaDoorBLUE", texPath + L"UV_AD_BLUE.png");
		app->RegisterTexture(L"AreaDoorRED", texPath + L"UV_AD_RED.png");
		//タイトルスプライト
		app->RegisterTexture(L"TITLETEXT", texPath + L"TitleText.png");
		app->RegisterTexture(L"TITLEROGO", texPath + L"TitleRogo.png");
		app->RegisterTexture(L"TITLEEFFECT", texPath + L"TitleEffect.png");
		app->RegisterTexture(L"TITLEBACKGROUND", texPath + L"TitleBackGround.png");
		//ゲームクリア
		app->RegisterTexture(L"GameClearTEXT", texPath + L"CLEAR.Text.png");
		app->RegisterTexture(L"CLEARBackGround", texPath + L"CLEARBackGround.png");
		app->RegisterTexture(L"NextStage", texPath + L"NextStage.png");
		app->RegisterTexture(L"ClearSelectStage", texPath + L"ClearSelectStage.png");

		//ゲームオーバー
		app->RegisterTexture(L"GameOverText", texPath + L"GAMEOVER_Text.png");
		app->RegisterTexture(L"GameOverBackEffect", texPath + L"GAMEOVER_BackEffect.png");
		app->RegisterTexture(L"OverSelectStage", texPath + L"OverSelectStage.png");
		app->RegisterTexture(L"Retry", texPath + L"Retry.png");

		//オーバーヒート
		app->RegisterTexture(L"OverHeatText", texPath + L"OverHeatText.png");

		//セレクトステージテクスチャ
		app->RegisterTexture(L"SelectStageBack", texPath + L"SelectStageBack.png");
		app->RegisterTexture(L"SelectFram", texPath + L"SelectFram.png");
		app->RegisterTexture(L"SelectText", texPath + L"SelectText.png");
		app->RegisterTexture(L"SelectNumber", texPath + L"SelectNumber.png");
		app->RegisterTexture(L"SelectTitle", texPath + L"SelectTitle.png");
		app->RegisterTexture(L"OverHeatFram", texPath + L"OverHeatFram.png");
		app->RegisterTexture(L"OverHeatGauge", texPath + L"OverHeatGauge.png");

		//GameStageUI
		app->RegisterTexture(L"GameStageUI", texPath + L"UIGameStage.png");
		app->RegisterTexture(L"PauseBG", texPath + L"PauseBackGround.png");
		app->RegisterTexture(L"PauseSelect", texPath + L"PauseSelect.png");
		app->RegisterTexture(L"PauseTitle", texPath + L"PauseTitle.png");
		app->RegisterTexture(L"PauseBack", texPath + L"PauseBack.png");
		app->RegisterTexture(L"PauseSelectCharge", texPath + L"PauseSelectCharge.png");
		app->RegisterTexture(L"PauseTitleCharge", texPath + L"PauseTitleCharge.png");

		//サウンド
		//SE
		wstring soundWav = sdPath + L"OverHeatSE.wav";
		App::GetApp()->RegisterWav(L"OverHeatSE", soundWav);
		
		soundWav = sdPath + L"PlayerJump.wav";
		App::GetApp()->RegisterWav(L"PlayerJump", soundWav);
		soundWav = sdPath + L"PlayerProj.wav";
		App::GetApp()->RegisterWav(L"PlayerProj", soundWav);
		soundWav = sdPath + L"PlayerWalk.wav";
		App::GetApp()->RegisterWav(L"PlayerWalk", soundWav);
		soundWav = sdPath + L"PlayerLand.wav";
		App::GetApp()->RegisterWav(L"PlayerLand", soundWav);
		soundWav = sdPath + L"PlayerDamage.wav";
		App::GetApp()->RegisterWav(L"PlayerDamage", soundWav);

		soundWav = sdPath + L"Switch.wav";
		App::GetApp()->RegisterWav(L"SwitchSE", soundWav);
		soundWav = sdPath + L"Decision.wav";
		App::GetApp()->RegisterWav(L"DecisionSE", soundWav);

		soundWav = sdPath + L"Confirm.wav";
		App::GetApp()->RegisterWav(L"Confirm", soundWav);

		soundWav = sdPath + L"GameClear.wav";
		App::GetApp()->RegisterWav(L"GameClearSE", soundWav);
		soundWav = sdPath + L"Door.wav";
		App::GetApp()->RegisterWav(L"DoorSE", soundWav);
		//BGM
		soundWav = sdPath + L"TitleTheme.wav";
		App::GetApp()->RegisterWav(L"TitleBGM", soundWav);
		soundWav = sdPath + L"StageBGM.wav";
		App::GetApp()->RegisterWav(L"StageBGM", soundWav);
	}

	void Scene::OnCreate(){
		try {
			// 背景色を設定
			SetClearColor(Col4(0.0f, 0.11328125f, 0.2578125, 1.0f)); // ミッドナイトブルー
			
			//リソース作成
			CreateResources();

			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToTitleStage");
		}
		catch (...) {
			throw;
		}
	}
	
	void Scene::OnEvent(const shared_ptr<Event>& event) {

		//ゲームステージの設定
		if (event->m_MsgStr == L"ToGameStage") {
			ResetActiveStage<GameStage>();
		}
		else if (event->m_MsgStr == L"ToTitleStage") {
			ResetActiveStage<TitleStage>();
		}
		else if (event->m_MsgStr == L"ToSlelctStage") {
			ResetActiveStage<SelectStage>();
		}
		else if (event->m_MsgStr == L"ToGameStageHemmi") {
			ResetActiveStage<GameStageHemmi>();
		}
		else if (event->m_MsgStr == L"ToGameStageTsuboi") {
			ResetActiveStage<GameStageTsuboi>();
		}
		else if (event->m_MsgStr == L"ToTestStage") {
			ResetActiveStage<GameStage>();
		}
		else if (event->m_MsgStr == L"ToLoad") {
			ResetActiveStage<LoadScreen>();
		}
	}

	void Scene::SetSelectedMap(const int& select) {

		switch (select)
		{
		case 0:
			m_SelectedMap = L"ToTitle";
			m_select = select;
			break;
		case 1:
			m_SelectedMap =  L"GameStage.csv";
			m_select = select;
			break;
		case 2:
			m_SelectedMap = L"Stagedata1.csv";
			m_select = select;
			break;
		case 3:
			m_SelectedMap = L"Stagedata2.csv";
			m_select = select;
			break;
		case 4:
			m_SelectedMap = L"Stagedata3.csv";
			m_select = select;
			break;
		case 5:
			m_SelectedMap = L"Stagedata4.csv";
			m_select = select;
			break;
		}
	}
	wstring Scene::GetSelectedMap() {
		return m_SelectedMap;
	}
}
//end basecross
