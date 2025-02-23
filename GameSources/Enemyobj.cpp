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

	ChasingEnemy::ChasingEnemy(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		const State& defaultState,
		const State& overheatState,
		const shared_ptr<Player>& player
	):
		Enemy(stage, position, rotation, scale, defaultState, overheatState, player),
		m_defaultState(rightMove),
		m_overheatState(stay)
	{}
	void ChasingEnemy::OnUpdate() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto pos = m_trans->GetPosition();

		pos = Grav();

		switch (m_stateType)
		{
		case basecross::Enemy::stay:
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			EnemyAnime(L"wait");
			break;
		case basecross::Enemy::wait:
			EnemyAnime(L"stand");
			if (!m_overHeatSE) {
				//起動時のエフェクト,SE
				EffectPlay(m_eyeEffect, GetEyePos(Vec3(2.0f, 0.5f, 0.5f)), 1, Vec3(0.5f));
				EffectPlay(m_eyeEffect, GetEyePos(Vec3(2.0f, 0.5f, -0.5f)), 2, Vec3(0.5f));
				PlaySE(L"EnemyRevival", 2.0f);
				m_overHeatSE = true;
			}
			m_spareTime -= elapsed;
			if (m_spareTime <= 0.0f) {

				m_stateType = m_fastState;
			}
			break;
		case basecross::Enemy::rightMove:
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			m_speed = m_maxSpeed;
			PlayerDic();
			if (!m_floorCol->GetPlayerFlag()) {
				EnemyAngle();
			}
			if (m_direc.length() <= m_trackingRange * 2) {
				EnemyAnime(L"walk");
				pos += m_speed * m_direcNorm * elapsed;
			}
			break;
			//投げる
		case throwAway:
			SetGrav(Vec3(0.0f, m_gravity, 0.0f));
			PlayerDic();
			pos.y += 0.25f * m_throwLength;
			pos -= m_speed * m_direcNorm * elapsed * m_throwLength * 20.0f;
			m_throwTime -= elapsed;
			if (m_throwTime < 0.0f) {

				EffectPlay(m_burstEffect, GetEyePos(Vec3(0, 0, 0)), 4, Vec3(0.5f));
				PlaySE(L"EnemyBurst", 2.0f);
				SetState(m_overHeatState);
				m_throwTime = 0.5f;
			}
			AroundOverHeat();
			break;
		default:
			break;
		}
		m_trans->SetPosition(pos);

		if (GetOverHeat()) {
			Grab();
		}
		OverHeat();

		//アニメーションの実装
		m_draw->UpdateAnimation(elapsed);
	}
}