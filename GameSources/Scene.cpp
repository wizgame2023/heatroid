
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
	void Scene::CreateResources() {

		auto& app = App::GetApp();

		// mediaパスを取得
		auto path = app->GetDataDirWString();
		// texturesパスを取得
		auto texPath = path + L"Textures/";
		// Modelsパス
		auto mdlPath = path + L"Models/";

		// プレイヤー
		auto modelMesh = MeshResource::CreateBoneModelMesh(mdlPath, L"HeatRoid.bmf");
		app->RegisterResource(L"PLAYER", modelMesh);

		//敵のモデル
		auto enemyAruki = MeshResource::CreateBoneModelMesh(mdlPath, L"ZAKO_ARUKU.bmf");
		app->RegisterResource(L"ENEMYARUKU", enemyAruki);


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
		app->RegisterTexture(L"HEALTH", texPath + L"health.png");
		app->RegisterTexture(L"CHARGE", texPath + L"charge.png");
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

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//ゲームステージの設定
			ResetActiveStage<GameStage>();
		}
		if (event->m_MsgStr == L"ToTitleStage") {
			//ゲームステージの設定
			ResetActiveStage<TitleStage>();
		}
		else if (event->m_MsgStr == L"ToGameStageHemmi") {
			//ゲームステージの設定
			ResetActiveStage<GameStageHemmi>();
		}
		else if (event->m_MsgStr == L"ToGameStageTsuboi") {
			//ゲームステージの設定
			ResetActiveStage<GameStageTsuboi>();
		}
	}



}
//end basecross
