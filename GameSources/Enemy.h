/*!
@file Enemy.h
@brief “G‚È‚Ç
’S“–FˆíŒ©
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
		enum State {
			stay,      //ˆÚ“®‚È‚µ
			rightMove, //’Ç]¶‰EˆÚ“®
			upMove,    //ã‰ºˆÚ“®
			flyMove,   //’Ç]•‚—VˆÚ“®
			fly,       //•‚—V
			death,     //
			runaway    //
		};

		float m_hp;          //‘Ì—Í
		float m_maxHp;       //Å‘å‘Ì—Í
		float m_speed;       //ˆÚ“®‘¬“x
		float m_upSpeed;     //ã‰º‚Ì‘¬“x
		float m_upHeight;    //ã‰º‚Ì‚‚³
		float m_jumpPower;   //ƒWƒƒƒ“ƒv—Í
		float m_jumpTime;    //ƒWƒƒƒ“ƒv‘¬“x
		float m_time;
		float m_bulletTime;  //’e‚Ì”­ËŠÔŠu
		float m_bulletRange; //’e‚ğ”­Ë‚·‚é‹——£
		float m_playerScale; //ƒvƒŒƒCƒ„[‚ÌƒTƒCƒY
		int m_dic;           //Œü‚¢‚Ä‚¢‚é•ûŒü ¶-1,‰E1
		bool m_bulletFlag;   
		bool m_jumpFlag;
		bool m_flyFlag;

		State m_stateType;
		State m_beforState;

		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;

		shared_ptr<Transform> m_trans;
		shared_ptr<Player> m_player;
		shared_ptr<Transform> m_playerTrans;
		shared_ptr<FixedBox> m_box;
		weak_ptr<Gravity> m_gravity;

	public:
		// \’z‚Æ”jŠü
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
		virtual void OnCreate() override; // ‰Šú‰»
		virtual void OnUpdate() override; // XV

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
		
		float m_speed;   //’e‚Ì‘¬‚³
		float m_Range;   //Ë’ö
		int m_dic;       //’e‚ÌŒü‚« ¶-1,‰E1
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