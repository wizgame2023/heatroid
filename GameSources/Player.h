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
	class PlayerGrab;
	class Player : public GameObject {

		//エフェクト
		shared_ptr<EfkEffect> m_EfkMuzzle;
		shared_ptr<EfkEffect> m_EfkHit;
		//エフェクト実行オブジェクト
		shared_ptr<EfkPlay> m_EfkPlay;

		//Transformの初期化
		Vec3 m_initPos;
		Vec3 m_initRot;
		Vec3 m_initSca;
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
		const float m_speed = 72.0f;
		const float m_airSpeedPerc = .35f;
		//加速度
		const float m_accel = 200.0f;
		//摩擦係数(静/動/完全停止)
		const float m_friction = .75f;
		const float m_frictionDynamic = .5f;
		const float m_frictionThreshold = .5f;
		//ジャンプ高度
		const float m_jumpHeight = 12.0f;
		//操作方向の向き
		float m_moveAngle = 0;
		//重力
		const float m_gravity = -20.0f;
		//落下時の終端速度
		const float m_fallTerminal = -50.0f;
		//飛び道具が出る場所
		const Vec3 m_firePos = Vec3(1.0f, .8f, -.75f);
		//移動方向
		Vec3 m_moveVel = Vec3(0, 0, 0);
		//CollisionExitの空中判定用カウント
		int m_collideCount;
		const int m_collideCountInit = 10;

		float m_landSEcooltime = 0.0f;

		//プレイヤーの状態を表す列挙型
		enum Stats {
			stand,		//地上
			air,		//空中
			hit_stand,	//やられ地上
			hit_air,	//やられ空中
			shot_stand,	//チャージ地上
			shot_air,	//チャージ空中
			release,	//発射
			start,		//ゲーム開始演出
			died,		//死亡
			died_air,		//死亡空中
			goal		//ステージクリア
		};
		//プレイヤーの状態
		Stats m_stateType = start;
		//演出アニメ制御用の時間計測変数
		float m_animTime = 0;

		//ステージマネージャ
		shared_ptr<StageManager> m_stageMgr;

		//掴み判定用ポインタ
		weak_ptr<PlayerGrab> m_pGrab;

		//移動時の物理学的な計算を行うか否か
		bool m_doPhysicalProcess;
		//チャージ中orオーバーチャージ中
		bool m_isCharging = false;
		bool m_isOverCharge = false;
		//タメ
		float m_chargePerc = 0;
		const float m_chargeSpeed = 1.2f;
		const float m_chargeReduceSpeed = -.4f;
		//無敵時間
		float m_invincibleTime = 0;
		const float m_invincibleTimeMax = 1.8f;
		//掴み判定の持続秒数
		float m_grabTime = 0;
		const float m_grabTimeMax = .2f;
		//何かを持っているか否か
		bool m_isCarrying;
		//歩行音用
		float m_walkSndTime;

		//HP
		int m_HP = 0;
		const int m_HP_max = 4;
		
		float _delta = App::GetApp()->GetElapsedTime();


	public:
		//構築と破棄

		Player(const shared_ptr<Stage>& StagePtr,
			const Vec3& pos, const Vec3& rot, const Vec3& sca);

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

		//最高速度
		void SpeedLimit();
		//アニメーション制御
		void Animate();
		//重力
		void Gravity();
		//摩擦
		void Friction();
		//演出中の摩擦
		void FrictionMovie();
		//歩行音
		void WalkSound();

		//四捨五入
		Vec3 RoundOff(Vec3 number, int point);
		//アニメーションの登録
		void RegisterAnim();
		//敵を持つ
		void GrabEnemy();
		//飛び道具発射
		void Projectile();
		//攻撃をくらう/死ぬ
		void GetHit(shared_ptr<GameObject>& target);
		void Died();

		//Transform.Scaleのゲッタ
		const Vec3 GetScale() {
			return GetComponent<Transform>()->GetScale();
		}

		//HPを1を最大値とした割合で返す
		const float GetHPRate() {
			if (m_HP < 0) return 0;
			return static_cast<float>(m_HP) / static_cast<float>(m_HP_max);
		}

		//死んだらtrueを返す
		const bool GetDied() {
			return (m_stateType == died || m_stateType == died_air);
		}

		//ゴールに到達したらtrueを返す
		const bool GetArrivedGoal() {
			return (m_stateType == goal);
		}

		//描画コンポーネントのゲッタ
		const shared_ptr<PNTBoneModelDraw> GetDrawPtr() {
			return GetComponent<PNTBoneModelDraw>();
		}

		//SEの再生
		void Player::PlaySnd(wstring sndname, float volume, float loopcount) {
			auto audio = App::GetApp()->GetXAudio2Manager();
			audio->Start(sndname, loopcount, volume);
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

		void SwitchAnim(const float time, const float condition, const wstring prefix);

		void Charging(bool charge) {
			if (m_isCarrying == true) return;
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

		const wstring AddPrefix() {
			if (m_isCharging) return L"Fire_";
			if (m_isCarrying) return L"Grab_";
			else return L"";
		}
	};

	//====================================================================
	// class PlayerGrab
	// プレイヤーの掴み判定
	//====================================================================
	class Enemy;
	class PlayerGrab : public GameObject {
		//プレイヤーとの相対位置・判定の大きさ
		Vec3 m_dist, m_scale;
		//プレイヤーに追従させるためのポインタ
		weak_ptr<Player> m_player;
		//触れた敵のポインタを保管しておく
		shared_ptr<Enemy> m_target;
		//敵に当たっているかどうか
		bool m_isHit;
	public:
		PlayerGrab(const shared_ptr<Stage>& StagePtr, const shared_ptr<Player>& player) :
			GameObject(StagePtr),
			m_player(player),
			m_dist(Vec3(-5.0f, 1, 0)),
			m_scale(Vec3(8.0f)),
			m_isHit(false)
		{};

		~PlayerGrab() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void SetCollActive(bool update) {
			GetComponent<CollisionSphere>()->SetUpdateActive(update);
		}

		bool IsHit() {
			return m_isHit;
		}

		//何かに接触している判定
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

	};

	//====================================================================
	// class ChargeAura
	// プレイヤーがチャージ中に足元に出る筒
	//====================================================================
	class ChargeAura : public GameObject {
	public:
		ChargeAura(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr)
		{};
		~ChargeAura() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	};
	//====================================================================
	// class FireProjectile
	// プレイヤーの火炎放射
	//====================================================================

	class FireProjectile : public GameObject {
		//ステージマネージャ
		shared_ptr<StageManager> m_stageMgr;

		//エフェクト
		shared_ptr<EfkEffect> m_EfkEffect;
		//エフェクト実行オブジェクト
		shared_ptr<EfkPlay> m_EfkPlay;

		float m_playTime = 0;

		//どれくらいの位置からスタートするか
		Vec3 m_dist;
		//速度と方向
		float m_speed, m_speedBase, m_power;
		Vec3 m_angle;
		//射程
		float m_range = 0, m_rangeMax;
		bool m_stopped;

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
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

	};

	//====================================================================
	// class ChargePtcl
	// チャージ中のパーティクル
	//====================================================================

	class ChargePtcl : public MultiParticle {
	public:
		ChargePtcl(const shared_ptr<Stage>& StagePtr) :
			MultiParticle(StagePtr)
		{}
		virtual ~ChargePtcl() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void Emit(const Vec3& emitPos, const Vec3& randomEmitRange);
	};

	//====================================================================
	// class SpritePlayerUI
	// プレイヤーのゲージ類
	//====================================================================
	class SpriteHealth;
	class SpriteCharge;

	class SpritePlayerUI : public GameObject {
		wstring m_resKey;
		int m_layer;
		weak_ptr<Player> m_player;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		shared_ptr<SpriteHealth> m_health;
		shared_ptr<SpriteCharge> m_charge;
		shared_ptr<SpritePlayerUI> m_frame;

		const float m_width = 600.0f;
		const float m_height = 150.0f;
		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpritePlayerUI(const shared_ptr<Stage>& StagePtr, const shared_ptr<Player>& player, const wstring& resKey, const int& layer = 1):
		GameObject(StagePtr),
		m_player(player),
		m_resKey(resKey),
		m_layer(layer)
		{}

		~SpritePlayerUI() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};


	//====================================================================
	// class PlayerMeterBase
	// プレイヤーのゲージ類の親
	//====================================================================
	class PlayerMeterBase : public GameObject {
	protected:
		weak_ptr<Player> m_player;
		shared_ptr<SpritePlayerUI> m_meter;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		float m_width;
		float m_height;
		float m_bottomSlip;
		Vec3 addPos;
	public:
		PlayerMeterBase(const shared_ptr<Stage>& StagePtr, const shared_ptr<Player>& player, const shared_ptr<SpritePlayerUI>& meter) :
			GameObject(StagePtr),
			m_player(player),
			m_meter(meter)
		{}
		void Init(wstring ResKey);
	};

	//====================================================================
	// class SpriteHealth
	// プレイヤーのライフ
	//====================================================================
	class SpriteHealth : public PlayerMeterBase {

	public:
		SpriteHealth(const shared_ptr<Stage>& StagePtr, const shared_ptr<Player>& player, const shared_ptr<SpritePlayerUI>& meter) :
			PlayerMeterBase(StagePtr, player, meter)
		{}

		~SpriteHealth() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;
	};

	//====================================================================
	// class SpriteCharge
	// プレイヤーの長押しゲージ
	//====================================================================
	class SpriteCharge : public PlayerMeterBase {

	public:
		SpriteCharge(const shared_ptr<Stage>& StagePtr, const shared_ptr<Player>& player, const shared_ptr<SpritePlayerUI>& meter) :
			PlayerMeterBase(StagePtr, player, meter)
		{}

		~SpriteCharge() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;
	};

}
//end basecross

