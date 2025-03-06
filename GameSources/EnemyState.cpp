/*!
@file Emenyobj.cpp
@brief 敵のオブジェクト
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void ChaseState::Enter() {
		m_enemy->SetGrav(Vec3(0.0f, m_enemy->m_gravity, 0.0f));
		m_enemy->m_speed = m_enemy->m_maxSpeed;
	}
	void ChaseState::Execute() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto pos = m_enemy->GetPos();

		m_enemy->PlayerDic();
		if (!m_enemy->m_floorCol->GetPlayerFlag()) {
			m_enemy->EnemyAngle();
		}
		if (m_enemy->m_direc.length() <= m_enemy->m_trackingRange * 2) {
			m_enemy->EnemyAnime(L"walk");
			pos += m_enemy->m_speed * m_enemy->m_direcNorm * elapsed;
		}

		if (m_enemy->m_heat >= m_enemy->m_maxHeat) {
			m_enemy->ChangeState<OverHeatState>();
		}
		m_enemy->m_trans->SetPosition(pos);
		m_enemy->m_draw->UpdateAnimation(elapsed);
	}

	void OverHeatState::Enter() {
		m_enemy->SetGrav(Vec3(0.0f, m_enemy->m_gravity, 0.0f));
		m_enemy->PlaySE(L"OverHeatSE", 5.0f);
		m_enemy->EffectPlay(m_enemy->m_heatEffect, m_enemy->GetPos(), 3);
	}
	void OverHeatState::Execute() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto pos = m_enemy->GetPos();

		m_enemy->EnemyAnime(L"wait");

		m_enemy->m_playerFlag = false;
		if (m_enemy->m_heat > 0.0f) {
			m_enemy->m_heat -= elapsed * 2.5f * 10;
		}
		if (m_enemy->m_heat <= 0.0f) {
			//m_enemy->EnemyAnime(L"stand");
			if (m_enemy->m_draw->IsTargetAnimeEnd()) {
				m_enemy->ChangeState<ChaseState>();

			}
		}
		m_enemy->m_draw->UpdateAnimation(elapsed);

	}
	void OverHeatState::Exit() {
		m_enemy->EnemyAnime(L"stand");
		m_enemy->EffectPlay(m_enemy->m_eyeEffect, m_enemy->GetEyePos(Vec3(2.0f, 0.5f, 0.5f)), 1, Vec3(0.5f));
		m_enemy->EffectPlay(m_enemy->m_eyeEffect, m_enemy->GetEyePos(Vec3(2.0f, 0.5f, -0.5f)), 2, Vec3(0.5f));
		m_enemy->PlaySE(L"EnemyRevival", 2.0f);

	}

	//void EnemyStateMachine::ChangeState(const shared_ptr<EnemyState>& newState) {
	//	m_previousState = m_currentState;
	//	auto currentState = m_currentState.lock();
	//	auto owner = m_owner.lock();

	//	if (currentState && owner) {
	//		currentState->Exit();
	//	}
	//	m_currentState = newState;
	//	currentState = m_currentState.lock();
	//	if (currentState && owner) {
	//		currentState->Enter();
	//	}
	//}

	//void EnemyStateMachine::SetCurrentState(const shared_ptr<EnemyState>& state) {
	//	m_currentState = state;
	//}
	//void EnemyStateMachine::SetPreviousState(const shared_ptr<EnemyState>& state) {
	//	m_previousState = state;
	//}

}