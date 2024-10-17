/*!
@file Player.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Player : public GameObject {
	public:
		//構築と破棄

		Player(const shared_ptr<Stage>& StagePtr);

		virtual ~Player() {}
		//アクセサ
		//初期化
		virtual void OnCreate() override;
		//更新
		//virtual void OnUpdate() override;
		//virtual void OnUpdate2() override;
		//virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		//virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
		////Aボタン
		//void OnPushA();
	};

}
//end basecross

