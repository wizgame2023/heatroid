/*!
@file Enemy.h
@brief 敵など
担当：逸見
*/

#pragma once
#include "stdafx.h"
#include "FixedBox.h"
#include "Gimmick.h"
#include "EnemyState.h"
#include "EnemyImage.h"


namespace basecross {
	class EnemyFloorCol;
	class EnemyBullet;
	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;
	//--------------------------------------------------------------------------------------
	static const int MAX_EFFECT_NUM = 4; //エフェクトの数
	class Enemy : public GameObject
	{
	public:
		enum State {
			stay,      //移動なし
			rightMove, //追従左右移動
			upMove,    //上下移動
			flyMove,   //追従浮遊移動
			fly,       //浮遊
			fixedStay, //プレイヤーに触れた位置で固定
			jump,      //ジャンプ
			plunge,    //突っ込み
			bullet,    //弾を撃ってくる
			bulletMove,//横の動きだけついてきて弾を撃つ
			wait,      //立ち上がる
			slide,     //左右に移動しながら弾を撃つ
			throwAway, //投げられる
		};

		

	public:
		float m_heat;          //オーバーヒート値
		float m_maxHeat;       //最大オーバーヒート
		float m_angle;          //プレイヤーに向く角度
		float m_speed;         //移動速度
		float m_maxSpeed;      //↑の最大値
		float m_upSpeed;       //上下の速度
		float m_upHeight;      //上下の高さ
		float m_jumpPower;     //ジャンプ力
		float m_jumpTime;      //ジャンプ速度
		float m_jumpHight;     //ジャンプする高さ
		float m_bulletTime;    //弾の発射間隔
		float m_maxBulletTime; //↑の最大値
		float m_pBulletTime;
		float m_maxPbulletTime;
		int m_bulletCnt;
		float m_throwLength;   //投げる長さをチャージ量によって変える変数
		float m_bulletRangeTime;//弾の発射クールタイム
		float m_maxBulletRangeTime;//↑の最大値
		float m_trackingRange; //弾を発射する距離
		float m_dropTime;      //ヒットドロップまでの時間
		float m_maxDropTime;   //↑の最大値
		float m_hitDropTime;   //ヒットドロップ中の浮いてる時間
		float m_maxHitDropTime;//↑の最大値
		float m_spareTime;    //突っ込みの予備時間
		float m_maxSpareTime; //↑の最大値
		float m_efcTime;
		float m_rad;
		int m_dicUp;
		Vec3 m_direc;
		Vec3 m_direcNorm;
		Vec3 m_firstDirec;
		Vec3 m_moveRot;
		Vec2 m_bulletDic;
		//テスト用
		float m_test;
		float m_throwTime;

		bool m_bulletFlag;
		bool m_jumpFlag;
		bool m_jumpMoveFlag;
		bool m_flyFlag;
		bool m_floorFlag;
		bool m_hitDropFlag;
		bool m_plungeFlag;
		bool m_pGrabFlag;
		bool m_playerFlag;
		bool m_overHeatSE;
		bool m_plungeSE;
		bool m_throwFlag;
		bool m_overHeatFlag;

		bool m_activeFlag;

		wstring m_meshName;

		//重力
		float m_gravity;
		Vec3 m_grav;
		Vec3 m_gravVel;

		State m_stateType;
		State m_fastState;
		State m_beforeState;
		State m_overHeatState;

		Vec3 m_rot;
		Vec3 m_scal;
		Vec3 m_beforePos;
		Vec3 m_deathPos;
		Vec3 m_playerPos;
		Vec3 m_playerScale; //プレイヤーのサイズ

		Vec3 m_floorPos;
		Vec3 m_jumpPos;

		shared_ptr<Transform> m_trans;
		shared_ptr<PNTBoneModelDraw> m_draw;
		shared_ptr<EnemyFloorCol> m_floorCol;
		weak_ptr<Player> m_player;
		weak_ptr<Transform> m_playerTrans;
		weak_ptr<FixedBox> m_box;
		shared_ptr<CollisionCapsule> m_collision;
		weak_ptr<TilingFixedBox> m_fixedBox;
		weak_ptr<PlayerGrab> m_playerGrab;
		weak_ptr<GaugeSquare> m_gauge;
		weak_ptr<Square> m_gaugeFram;
		//エフェクト
		shared_ptr<EfkEffect> m_heatEffect;
		shared_ptr<EfkEffect> m_eyeEffect;
		shared_ptr<EfkEffect> m_burstEffect;
		shared_ptr<EfkPlay> m_EfkPlayer[MAX_EFFECT_NUM];

		unique_ptr<EnemyState> m_currentState;  //現在のステート
		unique_ptr<EnemyState> m_nextState;     //次のステート
		unique_ptr<EnemyState> m_previousState; //一つ前のステート

	public:
		// 構築と破棄
		Enemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const State& state,
			const State& overHeatState,
			const shared_ptr<Player>& player);
		Enemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const State& state,
			const State& overHeatState,
			const wstring& meshName,
			const shared_ptr<Player>& player);
		Enemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const shared_ptr<Player>& player);


		virtual ~Enemy() {}
		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新

		void OnCollisionEnter(shared_ptr<GameObject>& other);
		void OnCollisionExit(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
	public:
		void EnemyJump();
		void HipDropJump();
		void ThisDestroy();
		void PlayerDic();
		void OneJump(float jumpHight);
		void HitDrop();
		Vec3 Plunge();
		void JumpMove();
		void FindFixed();
		void EnemyAngle();
		void Grab();
		void FallBullet();
		void StraightXBullet();
		void RapidFireBullet(int bulletNum);
		void AroundOverHeat();
		void EnemyAnime(wstring anime);
		void OverHeat();
		void PlaySE(wstring path, float volume = 1.0f, float loopcnt = 0);
		void EffectPlay(const shared_ptr<EfkEffect>& efk,
			const Vec3& pos, const int num, const Vec3& scale = Vec3(1.0f));
		void EffectStop(int num);
		Vec3 GetEyePos(const Vec3& eye);
		void Debug();
	public:
		//get,set
		float GetAngle();
		void SetEnemyFlayFlag(bool flag);
		void SetSpeed(float speed);
		void SetUpMove(float speed, float height);
		void SetFlyPower(float power);
		void SetState(State state);
		Vec3 GetDirec();
		Vec3 GetPos();
		Vec3 GetWorldPos();
		bool GetFloorFlag();
		float GetHeatRatio();
		bool GetOverHeat();
		void SetPlungeFlag(bool flag);
		void SetBulletDirec(Vec2 direc);
		bool GetActiveFlag();
		void SetActiveFlag(bool flag);
		void PlayOverHeat();
		void SetThrowFlag(bool flag);
		void SetThorwLenght(float length);
		template <class NextState>
		void ChangeState() {
			m_currentState->Exit();
			m_previousState = move(m_currentState);
			m_currentState.reset();
			m_currentState = make_unique<NextState>(GetThis<Enemy>());
			m_currentState->Enter();
		}
		template <class NextState>
		void OnlyChangeState() {
			m_currentState->Exit();
			m_currentState.reset();
			m_currentState = make_unique<NextState>(GetThis<Enemy>());
			m_currentState->Enter();
		}
		void RetrunState() {
			m_currentState->Exit();
			m_currentState.reset();
			m_currentState = move(m_previousState);
			if (m_currentState) {
				m_currentState->Enter();
			}
		}


	public:
		//重力に関する関数
		Vec3 Grav();
		void SetGrav(Vec3 grav);
		void AddGrav(Vec3 grav);
		void GravZero();
		void GravVelZero();
		void SetGravVel(Vec3 grav);

	};

	//--------------------------------------------------------------------------------------
	//	class EnemyBullet : public GameObject; //弾
	//--------------------------------------------------------------------------------------
	class EnemyBullet : public GameObject {
	protected:
		float m_colTime;
		bool m_playerColFlag;
		shared_ptr<PNTStaticDraw> m_draw;

	public:
		EnemyBullet(const shared_ptr<Stage>& stage);
		virtual ~EnemyBullet() {};
		virtual void OnCreate();
		virtual void OnUpdate();
		virtual void OnCollisionEnter(shared_ptr<GameObject>& other);
		void SetColor(Col4 color);
		void ThisDestroy();

	};
	//--------------------------------------------------------------------------------------
	//	class StraightBullet : public EnemyBullet; //直線に跳ぶ弾
	//--------------------------------------------------------------------------------------
	class StraightBullet : public EnemyBullet {
	private:
		float m_speed;   //弾の速さ
		float m_range;   //射程
		float m_height;  //弾を発射時の高さ
		Vec3 m_pos;
		Vec3 m_scal;
		Vec3 m_enemyPos;

		shared_ptr<Transform> m_trans;
		weak_ptr<Enemy> m_enemy;
	public:
		StraightBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy);
		StraightBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy, const float height);
		virtual ~StraightBullet() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void Debug();
	};

	//--------------------------------------------------------------------------------------
	//	class ParabolaBullet : public EnemyBullet; //放物線状に跳ぶ弾
	//--------------------------------------------------------------------------------------
	class ParabolaBullet : public EnemyBullet {
	private:

		float m_speed;   //弾の速さ
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
		Vec3 m_enemyPos;
		Vec3 m_velocity;
		float m_test;

		shared_ptr<Transform> m_trans;
		weak_ptr<Enemy> m_enemy;

		Vec3 m_grav;
		Vec3 m_gravVel;


	public:
		ParabolaBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy);
		virtual ~ParabolaBullet() {}
		virtual void OnCreate();
		virtual void OnUpdate();

		void StartVel();
		void Debug();
		void Grav();

	};
	//--------------------------------------------------------------------------------------
	//	class TrackingBullet : public EnemyBullet; //プレイヤーをめがけて跳ぶ弾
	//--------------------------------------------------------------------------------------
	class TrackingBullet : public EnemyBullet {
	private:
		float m_speed;
		float m_range;
		Vec3 m_pos;
		Vec3 m_dic;
		Vec3 m_enemyPos;
		shared_ptr<Transform> m_trans;
		weak_ptr<Enemy> m_enemy;
		weak_ptr<Player> m_player;
	public:
		TrackingBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy, const shared_ptr<Player>& player);
		virtual ~TrackingBullet() {};
		virtual void OnCreate();
		virtual void OnUpdate();
	};

	//--------------------------------------------------------------------------------------
	//	class EnemyFloorCol : public GameObject;
	//--------------------------------------------------------------------------------------
	class EnemyFloorCol : public GameObject {
	private:
		float m_posHight;
		float m_plusScale;
		Vec3 m_pos;
		Vec3 m_enemyPos;
		Vec3 m_enemyScal;

		bool m_playerFlag;
		shared_ptr<Transform> m_trans;
		weak_ptr<Enemy> m_enemy;



	public:
		EnemyFloorCol(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy);
		virtual ~EnemyFloorCol() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& other);
		virtual void OnCollisionExit(shared_ptr<GameObject>& other);
		void ThisDestroy();
		bool GetPlayerFlag();
		void SetPosHight(float hight);
	};

}