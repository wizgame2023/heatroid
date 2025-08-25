/*!
@file Player.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "EffectManager.h"
#include "StageManager.h"
#include "PlayerState.h"
#include "PlayerGrab.h"
#include "PlayerProj.h"
#include "PlayerUI.h"

namespace basecross {

	//====================================================================
	// class Player
	// プレイヤークラス
	//====================================================================
	class PlayerGrab;
	class PlayerProj;
	class Player : public GameObject {
		shared_ptr<PlayerStateMachine> m_state;

		shared_ptr<EffectManeger> m_Effect;
		Effekseer::Handle m_Handle;
		Effekseer::Handle m_EfkMuzzle;
		Effekseer::Handle m_EfkHit;
		Effekseer::Handle m_EfkJump;
		Effekseer::Handle m_EfkLand;

		//飛び道具オブジェクト(前以て3つ呼び出しておく)
		shared_ptr<PlayerProj> m_proj[3];

		//Transformの初期化
		Vec3 m_initPos;
		Vec3 m_initRot;
		Vec3 m_initSca;
		//プレイヤーが使用するコントローラとキーボードの入力
		Vec2 GetInputState() const;
		// コントローラから方向ベクトルを得る
		Vec3 GetMoveVector();
		//文字列の表示
		void ShowDebug();
		//スピード(最高速)
		const float m_speed = 90.0f;
		const float m_airSpeedPerc = .3f;
		//加速度
		const float m_accel = 240.0f;
		//摩擦係数(静/動/完全停止)
		const float m_friction = .75f;
		const float m_frictionDynamic = .5f;
		const float m_frictionThreshold = .5f;
		//ジャンプ高度
		const float m_jumpHeight = 18.0f;
		//操作方向の向き
		float m_moveAngle = 0;
		//重力
		const float m_gravity = -32.0f;
		//落下時の終端速度
		const float m_fallTerminal = -120.0f;
		//飛び道具が出る場所
		const Vec3 m_firePos = Vec3(1.0f, .8f, -.75f);
		//移動方向
		Vec3 m_moveVel = Vec3(0, 0, 0);
		//空中判定
		bool m_isAir = false;
		//CollisionExitの空中判定用カウント
		int m_collideCount;
		const int m_collideCountInit = 15;

		float m_landSEcooltime = 0.0f;

		//演出アニメ制御用の時間計測変数
		float m_animTime = 0;
		//ゴール床
		shared_ptr<GameObject> m_goal = nullptr;
		bool m_goalPosMoved = false;
		//ゴール演出にてゴールから離れる距離
		const float m_distToGoal = 15.0f;
		//ゴール演出で振り向く処理用
		Quat m_goalRotate = Quat(0);
		Vec3 pos1, addpos1;

		//ステージマネージャ
		weak_ptr<StageGenerator> m_stageMgr;

		//掴み判定用ポインタ
		weak_ptr<PlayerGrab> m_pGrab;

		//移動時の物理学的な計算を行うか否か
		bool m_doPhysicalProcess = true;
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
		//歩行音の間隔計測用
		float m_walkSndTime;

		//HP
		int m_HP = 0;
		const int m_HP_max = 4;

		float _delta = App::GetApp()->GetElapsedTime();

		//クォータニオン基準での回転処理 1q:最初の回転 v:回転軸 rad:回転量
		Quat RotateQuat(const Quat q, const Vec3 v, const float rad) const {
			Quat r = Quat(cos(rad / 2), v.x * sin(rad / 2), v.y * sin(rad / 2), v.z * sin(rad / 2));
			Quat r2 = Quat(cos(rad / 2), -v.x * sin(rad / 2), -v.y * sin(rad / 2), -v.z * sin(rad / 2));
			return q * r * r2;
		}

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

			//ゴール演出まで待機
			void GoalStandbyBehavior();
			//ゴール演出
			void GoalBehavior();

			//プレイヤーの移動
			void MovePlayer();
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
			//ジャンプ
			void Jump();
			//飛び道具発射
			void Projectile();
			//攻撃をくらう
			void GetHit(shared_ptr<GameObject>& target);
			//死ぬ
			void Died();

			//ステージ生成オブジェクトを取得
			shared_ptr<StageGenerator> GetStageGen() const {
				return m_stageMgr.lock();
			}

			//ステート変更
			void ChangeState(PlayerStateMachine::PlayerStateType type) const {
				m_state->ChangeState(type);
			}

			//ゴール判定のオブジェクトを取得
			shared_ptr<GameObject> GetGoalObj() const {
				return m_goal;
			}

			//空中にいるか
			bool IsAir() const {
				return m_isAir;
			}

			//速度を設定
			void SetMoveVel(Vec3 vel) {
				m_moveVel = vel;
			}
			//速度を取得
			Vec3 GetMoveVel() const {
				return m_moveVel;
			}
			//速度を加算
			void AddMoveVel(Vec3 vel, bool mulAccel = false) {
				m_moveVel += vel * (mulAccel ? m_accel : 1.0f);
			}

			//Transform.Scaleのゲッタ
			const Vec3 GetScale() {
				return GetComponent<Transform>()->GetScale();
			}

			//HP取得
			const int GetHP() {
				return m_HP;
			}

			//HPを1を最大値とした割合で返す
			const float GetHPRate() {
				if (m_HP < 0) return 0;
				return static_cast<float>(m_HP) / static_cast<float>(m_HP_max);
			}

			//死んだらtrueを返す
			const bool GetDied() {
				return (m_state->GetStateType() == PlayerStateMachine::player_died);
			}

			//ゴールに到達したらtrueを返す
			const bool GetArrivedGoal() {
				return (m_state->GetStateType() == PlayerStateMachine::player_goal);
			}

			//描画コンポーネントのゲッタ
			const shared_ptr<PNTBoneModelDraw> GetDrawPtr() {
				return GetComponent<PNTBoneModelDraw>();
			}

			//敵を持っているか否か(持っていればtrueを返す)
			const bool Player::IsCarryingEnemy();

			//SEの再生
			void Player::PlaySnd(const wstring& sndname, float volume, float loopcount) {
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

			//GetForwardから向きに応じたベクトルを返す
			Vec3 ForwardConvert(Vec3 v) {
				Vec3 ret;

				//プレイヤーと同じ方向を向いた単位ベクトルを作成
				auto trans = GetComponent<Transform>();
				auto fwd = -1 * trans->GetForward();
				auto face = atan2f(fwd.z, fwd.x);

				//渡されたvの分だけずらす
				ret.x = (cosf(face) * v.x) - (sinf(face) * v.z);
				ret.y = v.y;
				ret.z = (cosf(face) * v.z) + (sinf(face) * v.x);

				return ret;
			}

			//アニメーションを変更する(既にそのアニメを再生中なら何もしない)
			const void SetAnim(const wstring& animname, float time = 0.0f) {
				auto draw = GetComponent<PNTBoneModelDraw>();
				if (draw->GetCurrentAnimation() != animname)
					draw->ChangeCurrentAnimation(animname, time);
			}

			//通常時・チャージ中、敵運搬中でアニメーションを切り替える
			void SwitchAnim(const float time, const float condition, const wstring& prefix);

			//長押しチャージ
			void Charging(bool charge) {
				m_isCharging = charge;
				if (charge == false) return;

				//1を超えたら少しずつ減らす
				if (m_isOverCharge) {
					m_chargePerc += m_chargeReduceSpeed * m_chargePerc * _delta;
				}
				//普通は増やしていく
				else {
					m_chargePerc += m_chargeSpeed * _delta;
				}

				//1を超えたフラグ
				if (m_chargePerc > 1.0f) m_isOverCharge = true;
			}

			//チャージ状態をリセットし、発射モーションに移行(chargePercが0になるので、オブジェクト生成後に呼ぶこと)
			void ResetCharge() {
				m_chargePerc = 0;
				m_isOverCharge = false;
				ChangeState(PlayerStateMachine::player_release);
			}

			//状態に応じてアニメーションを変える
			const wstring AddPrefix() {
				//敵運搬中
				if (m_isCarrying) return L"Grab_";
				//チャージ中
				if (m_isCharging) return L"Fire_";
				else return L"";
			}
	};

}
//end basecross

