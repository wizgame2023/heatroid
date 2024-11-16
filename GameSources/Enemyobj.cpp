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
		const State& state,
		const State& deathState,
		const shared_ptr<Player>& player
	):
		Enemy(stage,position,rotation,scale,state,deathState,player),
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

		if (!GetOverHeat()) {
			if (m_plungeTime >= 0.0f) {
				SetState(plunge);
			}
			else if (m_plungeTime < 0.0f) {
				SetState(rightMove);
				m_plungeRargeTime -= elapsed;
				//SetPlungeFlag(false);
			}
			if (m_plungeRargeTime <= 0.0f) {
				m_plungeTime = m_maxPlungeTime;
				m_plungeRargeTime = m_maxPlungeRargeTime;
			}

		}
		//m_trans->SetPosition(m_pos);

		//OverHeat();
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
}