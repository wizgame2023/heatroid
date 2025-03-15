/*!
@file Emenyobj.h
@brief 敵のオブジェクト
担当：逸見
*/

#pragma once
#include "stdafx.h"
#include "Enemy.h"
#include "EnemyState.h"

namespace basecross {
	class EnemyChase : public Enemy {
		State m_defaultState;
		State m_attackState;
	public:
		EnemyChase(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const State& defaultState,
			const State& deathState,
			const shared_ptr<Player>& player);
		EnemyChase(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const State& defaultState,
			const State& attackState,
			const State& deathState,
			const shared_ptr<Player>& player);
		virtual ~EnemyChase() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void Debug();
	private:
		float m_plungeTime;
		float m_maxPlungeTime;
		float m_plungeRargeTime;
		float m_maxPlungeRargeTime;
	};

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