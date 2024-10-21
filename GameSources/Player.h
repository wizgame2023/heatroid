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
		void ShowDebug();
		//入力ハンドラー
		InputHandler<Player> m_InputHandler;
		//スピード
		float m_Speed;
		//ジャンプ高度
		float m_jumpHeight;
		//移動距離と、それを算出するための座標保管用変数
		Vec3 m_moveVel, m_prevPos;
		float frameElapsed = .0f;
		const float _frame = .166f;


		enum Stats {
			stand,		//地上
			air,		//空中
			fall,		//落下中
			hit			//やられ
		};

		Stats m_stateType;

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
		
		//何かに接触している判定(接地判定に活用？)
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		
		////Aボタン
		void OnPushA();
	};

//--------------------------------------------------------------------------------------
//	プレイヤー 接地判定オブジェクト(使わない可能性大)
//--------------------------------------------------------------------------------------
	class LandingCollider : public GameObject
	{
		weak_ptr<GameObject> m_Player;
		Vec3 m_VecToParent;
	public:
		// 構築と破棄
		LandingCollider(const shared_ptr<Stage>& stage,
			const shared_ptr<GameObject>& parent,
			const Vec3& VecToParent) :
			GameObject(stage),
			m_Player(static_cast<shared_ptr<GameObject>>(parent))
		{
		}
		virtual ~LandingCollider()
		{
		}

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		//virtual void OnDraw() override; // 描画

		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

		void FollowPlayer();
	};

}
//end basecross

