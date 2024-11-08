
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

		// 火、仮テクスチャ
		app->RegisterTexture(L"FIRE", texPath + L"fire_kari.png");

	}

	void Scene::OnCreate(){
		try {
			// 背景色を設定
			SetClearColor(Col4(0.0f, 0.11328125f, 0.2578125, 1.0f)); // ミッドナイトブルー
			
			//リソース作成
			CreateResources();

			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
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
