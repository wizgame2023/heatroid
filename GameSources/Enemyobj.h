/*!
@file Emenyobj.h
@brief 敵のオブジェクト
担当：逸見
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class EnemyObj : public Enemy {
	public:
		EnemyObj(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const State& state,
			const State& deathState,
			const shared_ptr<Player>& player);
		virtual ~EnemyObj() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	private :
		float m_plungeTime;
		float m_maxPlungeTime;
		float m_plungeRargeTime;
		float m_maxPlungeRargeTime;
	};
}