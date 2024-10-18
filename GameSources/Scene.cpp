
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
	void Scene::CreateResourses() {

	}

	void Scene::OnCreate(){
		try {
			// 背景色を設定
			SetClearColor(Col4(0.0f, 0.11328125f, 0.2578125, 1.0f)); // ミッドナイトブルー
			
			//リソース作成
			CreateResourses();

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
			ResetActiveStage<GameStage>();
		}
	}



}
//end basecross
