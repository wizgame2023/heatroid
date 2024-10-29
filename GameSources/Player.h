/*!
@file Player.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "Collision.h"

namespace basecross {

//====================================================================
// class Player
// プレイヤークラス
//====================================================================

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
		//スピード(最高速)
		float m_speed;
		//加速度
		float m_accel;
		//摩擦係数(地上)
		float m_friction;
		float m_frictionThreshold;
		//空中制動
		float m_airSpeedRate;
		//ジャンプ高度
		float m_jumpHeight;
		//重力
		float m_gravity;
		//移動方向
		Vec3 m_moveVel;
		//CollisionExitの空中判定の閾値
		int m_collideCount, m_collideCountInit;

		//プレイヤーの状態を表す列挙型
		enum Stats {
			stand,		//地上
			air,		//空中
			hit,		//やられ
			fire,		//長押し攻撃
			attack1,	//攻撃1
		};

		//向き(1が右、-1が左)
		int m_face;

		//HP
		int m_HP, m_HP_max;
		
		//プレイヤーの状態
		Stats m_stateType;

		float _delta = App::GetApp()->GetElapsedTime();

	public:
		//構築と破棄

		Player(const shared_ptr<Stage>& StagePtr);

		virtual ~Player() {}
		//アクセサ
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;

		//何かに接触している判定(接地判定に活用？)
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;

		////Aボタン
		void OnPushA();

		//カメラの移動
		void MoveCamera();
		//重力
		void Gravity();

		//四捨五入
		Vec3 RoundOff(Vec3 number, int point);

		//Transform.Scaleのゲッタ
		Vec3 GetScale();

		//移動速度に応じて向きを変える
		void FacingWithVel();

	};

//====================================================================
// class AttackCollision
// プレイヤーの攻撃判定
//====================================================================

	class AttackCollision : public GameObject {
		weak_ptr<GameObject> m_player;
		Vec3 m_distFromPlayer;
		bool m_isMoveHit;

	public:
		//構築と破棄

		AttackCollision(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& player);

		virtual ~AttackCollision() {}
		//アクセサ
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;

		//何かに接触している判定
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;

		void FollowPlayer();

	};

}
//end basecross

