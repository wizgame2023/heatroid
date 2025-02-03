/*!
@file Emenyobj.cpp
@brief 敵のオブジェクト
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	EnemyChase::EnemyChase(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		const State& defaultState,
		const State& deathState,
		const shared_ptr<Player>& player
	) :
		Enemy(stage, position, rotation, scale, defaultState, deathState, player),
		m_defaultState(defaultState),
		m_attackState(plunge),
		m_plungeTime(0.0f),
		m_maxPlungeTime(3.0f),
		m_plungeRargeTime(7.0f),
		m_maxPlungeRargeTime(m_plungeRargeTime)
	{}

	EnemyChase::EnemyChase(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		const State& defaultState,
		const State& attackState,
		const State& deathState,
		const shared_ptr<Player>& player
	):
		Enemy(stage,position,rotation,scale,defaultState,deathState,player),
		m_defaultState(defaultState),
		m_attackState(attackState),
		m_plungeTime(0.0f),
		m_maxPlungeTime(3.0f),
		m_plungeRargeTime(7.0f),
		m_maxPlungeRargeTime(m_plungeRargeTime)
	{}

	void EnemyChase::OnCreate() {
		Enemy::OnCreate();
		//SetState(plunge);
	}
	void EnemyChase::OnUpdate() {
		Enemy::OnUpdate();
		float elapsed = App::GetApp()->GetApp()->GetElapsedTime();
		m_plungeTime -= elapsed;

		if (!GetOverHeat() && m_stateType != wait && m_stateType != throwAway) {
			if (m_plungeTime >= 0.0f) {
				EnemyAnime(L"spare");
				SetState(m_attackState);
			}
			else if (m_plungeTime < 0.0f) {
				SetState(m_defaultState);
				m_plungeRargeTime -= elapsed;
			}
			if (m_plungeRargeTime <= 0.0f) {
				m_plungeTime = m_maxPlungeTime;
				m_plungeRargeTime = m_maxPlungeRargeTime;
			}

		}
		//Debug();
	}
	void EnemyChase::Debug() {
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss(L"");
		wss << L"state : "
			<< m_stateType
			<<L"\n"
			<<m_plungeFlag
			<< endl;
		scene->SetDebugString(wss.str());
	}
	EnemyBulletChase::EnemyBulletChase(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotatoin,
		const Vec3& scale,
		const State& state,
		const State& overHeatState,
		const shared_ptr<Player>& player
	):
		Enemy(stage, position, rotatoin, scale, state, overHeatState,player)
	{}
	void EnemyBulletChase::OnCreate() {
		m_meshName = L"ENEMYTOBU";
		Enemy::OnCreate();

	}

}