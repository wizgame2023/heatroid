/*!
@file Enemy.h
@brief 敵など
担当：逸見
*/

#pragma once
#include "stdafx.h"
#include "FixedBox.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;
	//--------------------------------------------------------------------------------------
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
			runaway,   //プレイヤーと逆方向に移動
			hitDrop,   //ヒップドロップ
			plunge,    //突っ込み
			attack
		};

	private:
		float m_hp;          //体力
		float m_maxHp;       //最大体力
		float m_speed;       //移動速度
		float m_upSpeed;     //上下の速度
		float m_upHeight;    //上下の高さ
		float m_jumpPower;   //ジャンプ力
		float m_jumpTime;    //ジャンプ速度
		float m_time;
		float m_bulletTime;  //弾の発射間隔
		float m_bulletRange; //弾を発射する距離
		float m_gravity;
		Vec3 m_grav;
		Vec3 m_gravVel;
		float m_test;
		int m_dic;           //向いている方向 左-1,右1
		int m_dic2;

		bool m_bulletFlag;   
		bool m_jumpFlag;
		bool m_flyFlag;
		bool m_floorFlag;
		bool m_hitDropFlag;
		bool m_plungeFlag;
		bool m_plungeColFlag;

		State m_stateType;
		State m_beforState;
		State m_deathState;

		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
		Vec3 m_deathPos;
		Vec3 m_playerScale; //プレイヤーのサイズ
		Vec3 m_floorPos;
		Vec3 m_jumpPos;

		shared_ptr<Transform> m_trans;
		weak_ptr<Player> m_player;
		weak_ptr<Transform> m_playerTrans;
		weak_ptr<FixedBox> m_box;
		shared_ptr<CollisionObb> m_collision;
		weak_ptr<FixedBox> m_fixedBox;

	public:
		// 構築と破棄
		Enemy(const shared_ptr<Stage>& stage, 
			const Vec3& position, 
			const Vec3& rotatoin, 
			const Vec3& scale,
			const State& state,
			const State& deathState,
			const shared_ptr<Player>& player,
			const shared_ptr<FixedBox>& box = nullptr);
		virtual ~Enemy(){}
		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新

		void OnCollisionEnter(shared_ptr<GameObject>& other);
		void OnCollisionExcute(shared_ptr<GameObject>& other);
		void OnCollisionExit(shared_ptr<GameObject>& other);
		void EnemyJump();
		void HipDrop();
		void ThisDestroy();
		void ReceiveDamage(float damage);
		void SetEnemyFlayFlag(bool flag);
		void SetSpeed(float speed);
		void SetUpMove(float speed,float height);
		void SetFlyPower(float power);
		void SetState(State state);
		int GetDic();
		Vec3 GetPos();

		//重力に関する関数
		void Grav();
		void SetGrav(Vec3 grav);
		void AddGrav(Vec3 grav);
		void GravZero();
		void GravVelZero();
		void SetGravVel(Vec3 grav);

	};


	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;
	//--------------------------------------------------------------------------------------
	class EnemyBullet : public GameObject {
	private:
		
		float m_speed;   //弾の速さ
		float m_Range;   //射程
		int m_dic;       //弾の向き 左-1,右1
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
		Vec3 m_enemyPos;
		Vec3 m_fixedPos;

		shared_ptr<Transform> m_trans;
		shared_ptr<Enemy> m_enemy;
		

	public :
		EnemyBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy,const Vec3& fixedPos = Vec3(0.0f));
		virtual ~EnemyBullet(){}
		virtual void OnCreate();
		virtual void OnUpdate();
		void OnCollisionEnter(shared_ptr<GameObject>& other);

		void ThisDestroy();

	};


	class MyGravity : public Component {
	public:
		explicit MyGravity(const shared_ptr<GameObject>& GameObjectPtr,
			const bsm::Vec3& gravity = bsm::Vec3(0, -9.8f, 0));
		MyGravity::~MyGravity() {}

	private:
		Vec3 m_gravity;
		//struct Impl;
		//unique_ptr<Impl> pImpl;
	public:
		Vec3 GetGravity() const;
		void SetGravity(const bsm::Vec3& gravity);
		void SetGravityZero();
		void OnUpdate();
	};

}