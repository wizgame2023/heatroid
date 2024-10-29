/*!
@file Player.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "Collision.h"

namespace basecross {

//====================================================================
// class TriggerColl
// 押し合い処理を行わない球形コライダ
//====================================================================

	class TriggerColl : public Collision {
	public:
		TriggerColl(const shared_ptr<GameObject>& GameObjectPtr);
		~TriggerColl();

		float GetMadeDiameter() const;
		void SetMadeDiameter(float f);
		float GetMadeRadius() const;
		void SetMadeRadius(float f);

		void CollideTest(const shared_ptr<CollisionSphere>& DestColl);
		CalcScaling GetCalcScaling() const;
		void SetCalcScaling(CalcScaling s);
		SPHERE GetSphere() const;
		SPHERE GetBeforeSphere() const;
		virtual bool SimpleCollisionCall(const shared_ptr<Collision>& Src) override;
		virtual bool SimpleCollision(const shared_ptr<CollisionSphere>& DestColl) override;
		virtual bool SimpleCollision(const shared_ptr<CollisionCapsule>& DestColl) override;
		virtual bool SimpleCollision(const shared_ptr<CollisionObb>& DestColl) override;
		virtual bool SimpleCollision(const shared_ptr<CollisionRect>& DestColl) override;
		virtual void CollisionCall(const shared_ptr<Collision>& Src)override;
		virtual void CollisionTest(const shared_ptr<TriggerColl>& DestColl)override;
		virtual void CollisionTest(const shared_ptr<CollisionSphere>& DestColl)override;
		virtual void CollisionTest(const shared_ptr<CollisionCapsule>& DestColl)override;
		virtual void CollisionTest(const shared_ptr<CollisionObb>& DestColl)override;
		virtual void CollisionTest(const shared_ptr<CollisionRect>& DestColl) override;
		virtual bsm::Vec3 GetCenterPosition()const override;
		virtual AABB GetEnclosingAABB()const override;
		virtual AABB GetWrappedAABB()const override;

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;

	private:
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

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

		////Aボタン
		void OnPushA();

		void MoveCamera();
		Vec3 GetScale();
		Vec3 RoundOff(Vec3 number, int point);
		void Gravity();
	};

//====================================================================
// class AttackCollision
// プレイヤーの攻撃判定
//====================================================================

	class AttackCollision : public GameObject {
		weak_ptr<GameObject> m_player;
		Vec3 m_distFromPlayer;

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

		void FollowPlayer();

	};

}
//end basecross

