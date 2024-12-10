/*!
@file Emenyobj.h
@brief 敵のオブジェクト
担当：逸見
*/

#pragma once
#include "stdafx.h"

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
	private :
		float m_plungeTime;
		float m_maxPlungeTime;
		float m_plungeRargeTime;
		float m_maxPlungeRargeTime;
	};

	class EnemyBulletChase :public Enemy {
	public:
		EnemyBulletChase(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const State& state,
			const State& overHeatState,
			const shared_ptr<Player>& player);
		virtual ~EnemyBulletChase() {};
		virtual void OnCreate();
	};
}