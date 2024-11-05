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
		//落下時の終端速度
		float m_fallTerminal;
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
		//攻撃判定出現中
		bool m_isAttacking;
		//火炎放射攻撃中
		bool m_isFiring;
		//火炎放射の間隔
		float m_fireCount;
		//無敵時間
		int m_invincibleTime, m_invincibleTimeMax;

		//向き(1が右、-1が左)
		int m_face;

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

		//移動速度に応じて向きを変える
		void FacingWithVel();
		//入力ベクトルに応じて向きを変える
		void FacingWithInput();

		//Transform.Scaleのゲッタ
		const Vec3 GetScale() {
			return GetComponent<Transform>()->GetScale();
		}

		//描画コンポーネントのゲッタ
		const shared_ptr<PNTBoneModelDraw> GetDrawPtr() {
			return GetComponent<PNTBoneModelDraw>();
		}

		//m_faceのゲッタ
		const int GetFace() {
			return m_face;
		}

		//アニメーションを変更する(既にそのアニメを再生中なら何もしない)
		const void SetAnim(wstring animname) {
			auto draw = GetComponent<PNTBoneModelDraw>();
			if (draw->GetCurrentAnimation() != animname)
				draw->ChangeCurrentAnimation(animname);
		}

		void SwitchFireAnim(float time);

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
		Vec3 m_distAdd;
		//向き
		int m_face;
		//速度
		Vec3 m_speed;
		//攻撃が当たったか否か
		bool m_isMoveHit;
		//射程
		float m_range;

	public:
		//構築と破棄

		FireProjectile(const shared_ptr<Stage>& StagePtr,
			const Vec3 dist, const int face);

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
	// class PlayerStateAttack1
	// プレイヤー攻撃1段目
	//====================================================================
	class PlayerStateAttack1 : public ObjState<Player> {
	protected:
		PlayerStateAttack1() {};
		~PlayerStateAttack1() {};
	public:
		static shared_ptr<PlayerStateAttack1> Instance();
		virtual void Enter(const shared_ptr<Player>& Obj) override;
		virtual void Execute(const shared_ptr<Player>& Obj) override;
		virtual void Exit(const shared_ptr<Player>& Obj) override;
	};

}
//end basecross

