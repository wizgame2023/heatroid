/*!
@file Player.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Player : public GameObject {
		//プレイヤーが使用するコントローラとキーボードの入力
		Vec2 GetInputState() const;
		// コントローラから方向ベクトルを得る
		Vec3 GetMoveVector() const;
		//プレイヤーの移動
		void MovePlayer();
		//文字列の表示
		void DrawStrings();
		//入力ハンドラー
		InputHandler<Player> m_InputHandler;
		//スピード
		float m_Speed;
		//ジャンプ高度
		float m_jumpHeight;

		enum Stats {
			stand,		//地上
			air,		//空中
			hit			//やられ
		};

		Stats m_stat;

	public:
		//構築と破棄

		Player(const shared_ptr<Stage>& StagePtr);

		virtual ~Player() {}
		//アクセサ
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//virtual void OnUpdate2() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		//virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
		////Aボタン
		void OnPushA();
	};

//--------------------------------------------------------------------------------------
//	プレイヤー 接地判定オブジェクト
//--------------------------------------------------------------------------------------
	class LandingCollider : public GameObject
	{
		weak_ptr<Player> m_Player;
		Vec3 m_VecToParent;
	public:
		// 構築と破棄
		LandingCollider(const shared_ptr<Stage>& stage) :
			GameObject(stage)
		{
		}
		virtual ~LandingCollider()
		{
		}

		virtual void OnCreate() override; // 初期化
		//virtual void OnUpdate() override; // 更新
		//virtual void OnDraw() override; // 描画

		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

		void FollowPlayer();
	};

}
//end basecross

