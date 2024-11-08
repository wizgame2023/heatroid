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
		//ジャンプ高度
		float m_jumpHeight;
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
			hit,		//やられ
			attack1,	//攻撃1
		};

		//プレイヤーの状態
		Stats m_stateType;

		//移動時の物理学的な計算を行うか否か
		bool m_doPhysicalProcess;
		//火炎放射攻撃中
		bool m_isFiring;
		//火炎放射の間隔
		float m_fireCount;
		//無敵時間
		int m_invincibleTime, m_invincibleTimeMax;

		//HP
		int m_HP, m_HP_max;
		
		float _delta = App::GetApp()->GetElapsedTime();

		unique_ptr<StateMachine<Player>> m_stateMachine;


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
		////Bボタン
		void Firing(bool fire);

		//カメラの移動
		void MoveCamera();
		//重力
		void Gravity();

		//四捨五入
		Vec3 RoundOff(Vec3 number, int point);

		//アニメーションの登録
		void RegisterAnim();

		//Transform.Scaleのゲッタ
		const Vec3 GetScale() {
			return GetComponent<Transform>()->GetScale();
		}

		//描画コンポーネントのゲッタ
		const shared_ptr<PNTBoneModelDraw> GetDrawPtr() {
			return GetComponent<PNTBoneModelDraw>();
		}

		//アニメーションを変更する(既にそのアニメを再生中なら何もしない)
		const void SetAnim(wstring animname, float time = 0.0f) {
			auto draw = GetComponent<PNTBoneModelDraw>();
			if (draw->GetCurrentAnimation() != animname)
				draw->ChangeCurrentAnimation(animname, time);
		}

		void SwitchFireAnim(const float time);

		const wstring AddFire() {
			if (m_isFiring) return L"Fire_";
			else return L"";
		}
	};

//====================================================================
// class AttackCollision
// プレイヤーの攻撃判定
//====================================================================

	class AttackCollision : public GameObject {
		weak_ptr<GameObject> m_player;
		//プレイヤーからどれくらいの位置に置くか
		Vec3 m_distFromPlayer;
		//攻撃が当たったか否か
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

	//====================================================================
	// class FireProjectile
	// プレイヤーの火炎放射
	//====================================================================

	class FireProjectile : public GameObject {
		//どれくらいの位置からスタートするか
		Vec3 m_dist;
		//速度と方向
		float m_speed;
		Vec3 m_angle;
		//射程
		float m_range = 0, m_rangeMax;

	public:
		//構築と破棄

		FireProjectile(const shared_ptr<Stage>& StagePtr,
			const Vec3 dist, const Vec3 angle);

		virtual ~FireProjectile() {}
		//アクセサ
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;

		//何かに接触している判定
	};

	//プレイヤーのステート構成？
	//PlayerStateMovie			演出中などで操作不能状態
	//PlaywrStateCtrl			プレイヤー移動中
	//PlayerStateAttack(1～5)	攻撃

	//====================================================================
	// class PlayerStateCtrl
	// プレイヤーの移動操作中ステート
	//====================================================================
	class PlayerStateCtrl : public ObjState<Player> {
	protected:
		PlayerStateCtrl() {};
		~PlayerStateCtrl() {};
	public:
		static shared_ptr<PlayerStateCtrl> Instance();
		virtual void Enter(const shared_ptr<Player>& Obj) override;
		virtual void Execute(const shared_ptr<Player>& Obj) override;
		virtual void Exit(const shared_ptr<Player>& Obj) override;
	};

	//------------------------------------------------------------------
	class SpriteDebug : public GameObject {
		shared_ptr<PCTSpriteDraw> m_DrawComp;		
		vector<VertexPositionColorTexture> m_Vertices;
	public:
		SpriteDebug(const shared_ptr<Stage>& stage) :
			GameObject(stage)
		{}

		~SpriteDebug() {}

		virtual void SpriteDebug::OnCreate() override {
			Col4 color(0, 0, 0, 1);
			const float windowWidth = 1280.0f;

			m_Vertices = {
				{Vec3(-windowWidth * 0.5f, 400, 0.0f), color, Vec2(0, 0)},
				{Vec3(-windowWidth * 0.3f, 400, 0.0f), color, Vec2(1, 0)},
				{Vec3(-windowWidth * 0.5f, 320, 0.0f), color, Vec2(0, 1)},
				{Vec3(-windowWidth * 0.3f, 320, 0.0f), color, Vec2(1, 1)},
			};
			vector<uint16_t> indices = {
				0, 1, 2,
				2, 1, 3,
			};
			m_DrawComp = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
			m_DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
			m_DrawComp->SetTextureResource(L"FIRE");
			SetAlphaActive(true);
		}
	};

}
//end basecross

