/*!
@file Emenyobj.cpp
@brief 敵のオブジェクト
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void ChaseState::Enter() {
		auto enemy = m_enemy.lock();
		if (!enemy) { return; }
		enemy->SetGrav(Vec3(0.0f, enemy->m_gravity, 0.0f));
		enemy->m_speed = enemy->m_maxSpeed;
	}
	void ChaseState::Execute() {
		auto enemy = m_enemy.lock();
		if (!enemy) { return; }

		auto elapsed = App::GetApp()->GetElapsedTime();
		auto pos = enemy->GetPos();

		enemy->PlayerDic();
		if (!enemy->m_floorCol->GetPlayerFlag()) {
			enemy->EnemyAngle();
		}
		if (enemy->m_direc.length() <= enemy->m_trackingRange * 2) {
			enemy->EnemyAnime(L"walk");
			pos += enemy->m_speed * enemy->m_direcNorm * elapsed;
		}

		if (enemy->m_heat >= enemy->m_maxHeat) {
			enemy->ChangeState<OverHeatState>();
		}
		enemy->m_trans->SetPosition(pos);
		enemy->m_draw->UpdateAnimation(elapsed);
	}

	void OverHeatState::Enter() {
		auto enemy = m_enemy.lock();
		if (!enemy) { return; }
		enemy->SetGrav(Vec3(0.0f, enemy->m_gravity, 0.0f));
		enemy->PlaySE(L"OverHeatSE", 5.0f);
		enemy->EffectPlay(enemy->m_heatEffect, enemy->GetPos(), 3);
	}
	void OverHeatState::Execute() {
		auto enemy = m_enemy.lock();
		if (!enemy) { return; }

		auto elapsed = App::GetApp()->GetElapsedTime();
		auto pos = enemy->GetPos();


		enemy->m_playerFlag = false;
		if (enemy->m_heat > 0.0f) {
			enemy->m_heat -= elapsed * 2.5f * 10;
		}
		if (enemy->m_heat <= 0.0f) {
			enemy->EnemyAnime(L"stand");
			if (enemy->m_draw->IsTargetAnimeEnd()) {
				enemy->EffectStop(3);
				enemy->ChangeState<ChaseState>();

			}
		}
		else {
			enemy->EnemyAnime(L"wait");
		}

		enemy->m_draw->UpdateAnimation(elapsed);

	}
	void OverHeatState::Exit() {
		auto enemy = m_enemy.lock();
		if (!enemy) { return; }

		enemy->EnemyAnime(L"stand");
		enemy->EffectPlay(enemy->m_eyeEffect, enemy->GetEyePos(Vec3(2.0f, 2.5f, 0.5f)), 1, Vec3(0.5f));
		enemy->EffectPlay(enemy->m_eyeEffect, enemy->GetEyePos(Vec3(2.0f, 2.5f, -0.5f)), 2, Vec3(0.5f));
		enemy->PlaySE(L"EnemyRevival", 2.0f);

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