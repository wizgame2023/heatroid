/*!
@file Emenyobj.h
@brief “G‚ÌƒIƒuƒWƒFƒNƒg
’S“–FˆíŒ©
*/

#pragma once
#include "stdafx.h"
#include "Enemy.h"
#include "EnemyState.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;  //’Ç‚¢‚©‚¯‚Ä“Ëi‚·‚é“G
	//--------------------------------------------------------------------------------------
	class ChasingEnemy : public Enemy {
	private:
		State m_defaultState;
		State m_overheatState;

	public:
		ChasingEnemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const shared_ptr<Player>& player);
		virtual ~ChasingEnemy() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBulletEnemy : public Enemy;  //¶‰E‚É’Ç‚¢‚©‚¯‚Ä’e‚ğŒ‚‚Á‚Ä‚­‚é“G
	//--------------------------------------------------------------------------------------
	class MoveBulletEnemy : public Enemy {
	private:
	public:
		MoveBulletEnemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const shared_ptr<Player>& player);
		virtual ~MoveBulletEnemy() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class ParabolaBulletEnemy : public Enemy;  //•ú•¨ü‚É’e‚ğ”ò‚Î‚µ‚Ä‚­‚é“G
	//--------------------------------------------------------------------------------------
	class ParabolaBulletEnemy : public Enemy {
	public:
		ParabolaBulletEnemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const shared_ptr<Player>& player);
		virtual ~ParabolaBulletEnemy() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class SlideEnemy : public Enemy;  //¶‰EˆÚ“®‚µ‚È‚ª‚ç’e‚ğŒ‚‚Á‚Ä‚­‚é“G
	//--------------------------------------------------------------------------------------
	class SlideEnemy : public Enemy {
	public:
		SlideEnemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const shared_ptr<Player>& player);
		virtual ~SlideEnemy() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}