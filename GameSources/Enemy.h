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
		float m_bulletTime;    //弾の発射間隔
		float m_maxBulletTime; //↑の最大値
		float m_parabolaBulletTime;    //放物線に飛ばす弾の間隔
		float m_maxParabolabulletTime; //↑の最大値
		int m_bulletCnt;        //連続で出す弾の数
		float m_throwLength;    //投げる長さをチャージ量によって変える変数
		float m_bulletRangeTime;//弾の発射クールタイム
		float m_maxBulletRangeTime;//↑の最大値
		float m_trackingRange;  //弾を発射する距離
		float m_spareTime;      //突っ込みの予備時間
		float m_maxSpareTime;   //↑の最大値
		float m_rad;            //角度の計算
		float m_throwTime;      //投げれる時間
		Vec3 m_playerDirec;     //プレイヤーへのベクトル
		Vec3 m_playerDirecNorm; //プレイヤーへのベクトルの正規化
		Vec3 m_moveRot;
		Vec2 m_bulletDic;
		//テスト用
		bool m_bulletFlag;      //弾を発射するフラグ
		bool m_objFlag;         //オブジェクトへの衝突フラグ
		bool m_pGrabFlag;       //持たれている状態
		bool m_playerFlag;      //プレイヤーへの衝突フラグ
		bool m_throwFlag;       //投げれる状態
		bool m_overHeatFlag;    //オーバーヒート状態
		bool m_activeFlag;      //アクティブ状態

		wstring m_meshName;     //メッシュのパス

		//重力
		float m_gravity;        //重力の値
		Vec3 m_grav;            //重力の大きさ
		Vec3 m_gravVel;         //重力ベクトル

		Vec3 m_firstRot;        //最初の回転
		Vec3 m_firstScal;       //最初の大きさ
		Vec3 m_beforePos;       //最初の座標
		Vec3 m_playerPos;       //プレイヤーの座標
		Vec3 m_playerScale;     //プレイヤーのサイズ

		shared_ptr<Transform> m_trans;
		shared_ptr<PNTBoneModelDraw> m_draw;
		shared_ptr<CollisionCapsule> m_collision;
		shared_ptr<EnemyFloorCol> m_floorCol;
		//プレイヤーの取得
		weak_ptr<Player> m_player;
		weak_ptr<Transform> m_playerTrans;
		weak_ptr<PlayerGrab> m_playerGrab;
		//オーバーヒートのUI
		weak_ptr<GaugeSquare> m_gauge;
		weak_ptr<Square> m_gaugeFram;
		//エフェクト
		shared_ptr<EffectManeger> m_EfkPlayer;
		Effekseer::Handle m_heatEffect;
		Effekseer::Handle m_eyeEffect;
		Effekseer::Handle m_burstEffect;

		//ステート
		unique_ptr<EnemyState> m_currentState;  //現在のステート
		unique_ptr<EnemyState> m_previousState; //一つ前のステート

	public:
		// 構築と破棄
		Enemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const shared_ptr<Player>& player);


		virtual ~Enemy() {}
		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新

		//衝突判定
		virtual void OnCollisionEnter(shared_ptr<GameObject>& other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& other) override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& other) override;
	public:
		//自分自身の削除
		void ThisDestroy();
		//プレイヤーの方向を計算
		void PlayerDic();
		//プレイヤーの方向を向かせる
		void EnemyAngle();
		//放物線に発射する弾
		void FallBullet();
		//真っ直ぐ発射する弾
		void StraightXBullet();
		//連射する弾
		void RapidFireBullet(int bulletNum);
		//周りもオーバーヒートする
		void AroundOverHeat();
		//アニメーションの再生
		void EnemyAnime(wstring anime);
		//SEの再生
		void PlaySE(wstring path, float volume = 1.0f, float loopcnt = 0);
		//エフェクトの再生
		void EffectPlay(Effekseer::Handle& efk, const wstring name,
			const Vec3& pos, const int num, const Vec3& scale = Vec3(1.0f));
		//エフェクトの止める
		void EffectStop(Effekseer::Handle& efk);
		//敵の目の場所を設定
		Vec3 GetEyePos(const Vec3& eye);
		//デバック用
		void Debug();
	public:
		//get,set
		//角度の取得
		float GetAngle();
		//スピードの設定
		void SetSpeed(float speed);
		Vec3 GetDirec();
		//座標の取得
		Vec3 GetPos();
		//ワールド座標の取得
		Vec3 GetWorldPos();
		//床に触れてるかの状態
		bool GetFloorFlag();
		//オーバーヒート率の取得
		float GetHeatRatio();
		//オーバーヒート状態
		bool GetOverHeat();
		//弾を飛ばす角度の設定
		void SetBulletDirec(Vec2 direc);
		bool GetActiveFlag();
		void SetActiveFlag(bool flag);
		//オーバーヒートにする
		void PlayOverHeat();
		//投げている状態
		void SetThrowFlag(bool flag);
		//投げるときときの強さ
		void SetThorwLenght(float length);

		//ステータスの切り替え
		template <class NextState>
		void ChangeState() {
			m_currentState->Exit();
			m_previousState = move(m_currentState);
			m_currentState.reset();
			m_currentState = make_unique<NextState>(GetThis<Enemy>());
			m_currentState->Enter();
		}
		//ステータスの切り替え　（前のステータスの記憶なし）
		template <class NextState>
		void OnlyChangeState() {
			m_currentState->Exit();
			m_currentState.reset();
			m_currentState = make_unique<NextState>(GetThis<Enemy>());
			m_currentState->Enter();
		}
		//前のステータスと切り替え
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
		//重力の大きさを設定
		void SetGrav(Vec3 grav);
		//重力をなくす
		void GravZero();
		
	};

	//--------------------------------------------------------------------------------------
	//	class EnemyBullet : public GameObject; //弾
	//--------------------------------------------------------------------------------------
	class EnemyBullet : public GameObject {
	protected:
		float m_colTime;      //誤差の時間
		bool m_playerColFlag; //プレイヤーへの衝突フラグ
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

		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
		Vec3 m_enemyPos;
		float m_speed;   //弾の速さ
		Vec3 m_velocity; //飛ばすベクトル

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
	//	class EnemyFloorCol : public GameObject; //敵の頭上のコリジョン
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