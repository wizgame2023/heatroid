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
			runaway    //プレイヤーと逆方向に移動
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
		int m_dic;           //向いている方向 左-1,右1
		bool m_bulletFlag;   
		bool m_jumpFlag;
		bool m_flyFlag;

		State m_stateType;
		State m_beforState;
		State m_deathState;

		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
		Vec3 m_deathPos;
		Vec3 m_playerScale; //プレイヤーのサイズ

		shared_ptr<Transform> m_trans;
		shared_ptr<Player> m_player;
		shared_ptr<Transform> m_playerTrans;
		shared_ptr<FixedBox> m_box;
		shared_ptr<Gravity> m_gravity;
		shared_ptr<CollisionObb> m_collision;

	public:
		// 構築と破棄
		Enemy(const shared_ptr<Stage>& stage,
			const shared_ptr<Player>& player,
			const shared_ptr<FixedBox>& box = nullptr);
		Enemy(const shared_ptr<Stage>& stage, 
			const shared_ptr<Player>& player,
			const Vec3& position, 
			const Vec3& rotatoin, 
			const Vec3& scale,
			const shared_ptr<FixedBox>& box = nullptr);
		virtual ~Enemy(){}
		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新

		void OnCollisionEnter(shared_ptr<GameObject>& other);
		void ReceiveDamage(float damage);
		void ThisDestroy();
		void EnemyJump();
		void SetEnemyFlayFlag(bool flag);
		void SetSpeed(float speed);
		void SetUpMove(float speed,float height);
		void SetFlyPower(float power);
		void SetState(State state);

		int GetDic();
		Vec3 GetPos();
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

		shared_ptr<Transform> m_trans;
		shared_ptr<Enemy> m_enemy;

	public :
		EnemyBullet(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy);
		virtual ~EnemyBullet(){}
		virtual void OnCreate();
		virtual void OnUpdate();
		void OnCollisionEnter(shared_ptr<GameObject>& other);

		void ThisDestroy();

	};

	class MyGravity : public Component {
		explicit MyGravity(const shared_ptr<GameObject>& GameObjectPtr,
			const bsm::Vec3& gravity = bsm::Vec3(0, -9.8f, 0));

	};

}