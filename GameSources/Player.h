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
		Vec3 GetMoveVector();
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
		//摩擦係数(静/動/完全停止)
		float m_friction;
		float m_frictionDynamic;
		float m_frictionThreshold;
		//ジャンプ高度
		float m_jumpHeight;
		//操作方向の向き
		float m_moveAngle;
		//重力
		float m_gravity;
		//落下時の終端速度
		float m_fallTerminal;
		//飛び道具が出る場所
		Vec3 m_firePos;
		//移動方向
		Vec3 m_moveVel;
		//CollisionExitの空中判定の閾値
		int m_collideCount, m_collideCountInit;

		//プレイヤーの状態を表す列挙型
		enum Stats {
			stand,		//地上
			air,		//空中
			hit_stand,	//やられ地上
			hit_air,	//やられ空中
			shot_stand,	//攻撃地上
			shot_air,	//攻撃空中
			died		//死亡
		};
		//プレイヤーの状態
		Stats m_stateType;

		//移動時の物理学的な計算を行うか否か
		bool m_doPhysicalProcess;
		//チャージ中orオーバーチャージ中
		bool m_isCharging, m_isOverCharge;
		//タメ
		float m_chargePerc, m_chargeSpeed, m_chargeReduceSpeed;
		//無敵時間
		float m_invincibleTime, m_invincibleTimeMax;

		//HP
		int m_HP, m_HP_max;
		
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
		//アニメーション制御
		void Animate();
		//重力
		void Gravity();
		//摩擦
		void Friction();

		//四捨五入
		Vec3 RoundOff(Vec3 number, int point);
		//アニメーションの登録
		void RegisterAnim();
		//飛び道具発射
		void Projectile();
		//攻撃をくらう/死ぬ
		void GetHit();
		void Died();

		//Transform.Scaleのゲッタ
		const Vec3 GetScale() {
			return GetComponent<Transform>()->GetScale();
		}

		const float GetHPRate() {
			if (m_HP < 0) return 0;
			return static_cast<float>(m_HP) / static_cast<float>(m_HP_max);
		}

		//描画コンポーネントのゲッタ
		const shared_ptr<PNTBoneModelDraw> GetDrawPtr() {
			return GetComponent<PNTBoneModelDraw>();
		}

		//isChargingフラグの取得
		const bool IsCharging() {
			return m_isCharging;
		}

		//ゲージの溜まり具合のゲッタ
		const float GetChargePerc() {
			return m_chargePerc;
		}

		//アニメーションを変更する(既にそのアニメを再生中なら何もしない)
		const void SetAnim(wstring animname, float time = 0.0f) {
			auto draw = GetComponent<PNTBoneModelDraw>();
			if (draw->GetCurrentAnimation() != animname)
				draw->ChangeCurrentAnimation(animname, time);
		}

		void SwitchFireAnim(const float time);

		void Charging(bool charge) {
			m_isCharging = charge;
			if (charge == false) return;
			if (m_isOverCharge) {
				m_chargePerc += m_chargeReduceSpeed * m_chargePerc * _delta;
			}
			else {
				m_chargePerc += m_chargeSpeed * _delta;
			}

			if (m_chargePerc > 1.0f) m_isOverCharge = true;
		}

		const wstring AddFire() {
			if (m_isCharging) return L"Fire_";
			else return L"";
		}
	};

	//====================================================================
	// class FireProjectile
	// プレイヤーの火炎放射
	//====================================================================

	class FireProjectile : public GameObject {
		//どれくらいの位置からスタートするか
		Vec3 m_dist;
		//速度と方向
		float m_speed, m_power;
		Vec3 m_angle;
		//射程
		float m_range = 0, m_rangeMax;

	public:
		//構築と破棄

		FireProjectile(const shared_ptr<Stage>& StagePtr,
			const Vec3 dist, const Vec3 angle, const float power);

		virtual ~FireProjectile() {}
		//アクセサ
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;

		//何かに接触している判定
	};

	//------------------------------------------------------------------
	class SpriteHealth : public GameObject {
		weak_ptr<Player> m_player;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		const float m_width = 150.0f;
		const float m_height = 15.0f;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpriteHealth(const shared_ptr<Stage>& StagePtr, const shared_ptr<Player>& player) :
			GameObject(StagePtr),
			m_player(player)
		{}

		~SpriteHealth() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;
	};
	//------------------------------------------------------------------
	class SpriteCharge : public GameObject {
		weak_ptr<Player> m_player;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		const float m_width = 150.0f;
		const float m_height = 15.0f;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpriteCharge(const shared_ptr<Stage>& StagePtr, const shared_ptr<Player>& player) :
			GameObject(StagePtr),
			m_player(player)
		{}

		~SpriteCharge() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;
	};

}
//end basecross

