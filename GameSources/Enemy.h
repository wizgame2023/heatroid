/*!
@file Enemy.h
@brief “G‚È‚Ç
’S“–FˆíŒ©
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject
	{
		float m_hp;
		float m_maxHp;
		float m_speed;
		float m_jumpPower;
		float m_jumpSpeed;
		float m_time;
		int m_dic;           //Œü‚¢‚Ä‚¢‚é•ûŒü ¶-1,‰E1
		bool m_bulletFlag;

		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;

		shared_ptr<Transform> m_trans;
		shared_ptr<Player> m_player;
		shared_ptr<Transform> m_playerTrans;

	public:
		// \’z‚Æ”jŠü
		Enemy(const shared_ptr<Stage>& stage,const shared_ptr<Player>& player);
		Enemy(const shared_ptr<Stage>& stage, 
			const Vec3& position, 
			const Vec3& rotatoin, 
			const Vec3& scale);
		virtual ~Enemy(){}
		virtual void OnCreate() override; // ‰Šú‰»
		virtual void OnUpdate() override; // XV

		void OnCollisionEnter(shared_ptr<GameObject>& other);
		void ReceiveDamage(float damage);
		void ThisDestroy();
		void EnemyJump();

		int GetDic();
		Vec3 GetPos();
	};


	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;
	//--------------------------------------------------------------------------------------
	class EnemyBullet : public GameObject {
	private:
		
		float m_speed;   //’e‚Ì‘¬‚³
		int m_dic;       //’e‚ÌŒü‚« ¶-1,‰E1
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;

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
}