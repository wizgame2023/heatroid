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
		//スピード(最高速)
		float m_speed;
		//加速度
		float m_accel;
		//摩擦係数(地上)
		float m_friction;
		float m_frictionThreshold;
		//ジャンプ高度
		float m_jumpHeight;
		//重力
		float m_gravity;
		//移動方向
		Vec3 m_moveVel;
		//CollisionExitの空中判定の閾値
		int m_collideCount, m_collideCountInit;

		enum Stats {
			stand,		//地上
			air,		//空中
			hit			//やられ
		};

		//HP
		int m_HP, m_HP_max;

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

		////Aボタン
		void OnPushA();

		void MoveCamera();
		Vec3 GetScale();
		Vec3 RoundOff(Vec3 number, int point);
		void Gravity();
	};

}
//end basecross

