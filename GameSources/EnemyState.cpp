/*!
@file Emenyobj.cpp
@brief 敵のオブジェクト
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class ChaseState : public EnemyState	//プレイヤーを追いかけるステート
	//--------------------------------------------------------------------------------------
	void ChaseState::Enter() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		enemy->SetGrav(Vec3(0.0f, enemy->m_gravity, 0.0f));
		enemy->m_speed = enemy->m_maxSpeed;
		m_plungeTime = 7.0f;
	}
	void ChaseState::Execute() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;

		auto elapsed = App::GetApp()->GetElapsedTime();
		auto pos = enemy->GetPos();

		pos.y = enemy->Grav().y;

		enemy->PlayerDic();
		if (!enemy->m_floorCol->GetPlayerFlag()) {
			enemy->EnemyAngle();
		}
		if (enemy->m_direc.length() <= enemy->m_trackingRange * 2) {
			enemy->EnemyAnime(L"walk");
			pos += enemy->m_speed * enemy->m_direcNorm * elapsed;
		}

		m_plungeTime -= elapsed;
		if (m_plungeTime <= 0.0f) {
			enemy->ChangeState<PlungeState>();
		}

		if (enemy->m_heat >= enemy->m_maxHeat) {
			enemy->ChangeState<OverHeatState>();
		}
		enemy->m_trans->SetPosition(pos);
		enemy->m_draw->UpdateAnimation(elapsed);
	}

	//--------------------------------------------------------------------------------------
	//	class OverHeatState : public EnemyState	　//オーバーヒートステート
	//--------------------------------------------------------------------------------------
	void OverHeatState::Enter() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;

		enemy->SetGrav(Vec3(0.0f, enemy->m_gravity, 0.0f));
		enemy->PlaySE(L"OverHeatSE", 5.0f);
		enemy->EffectPlay(enemy->m_heatEffect, enemy->GetPos(), 3);
		enemy->m_overHeatFlag = true;

		auto gauge = enemy->m_gauge.lock();
		if (!gauge) return;
		gauge->DrawGauge(true);
		auto gaugeFram = enemy->m_gaugeFram.lock();
		if (!gaugeFram) return;
		gaugeFram->DrawGauge(true);
	}
	void OverHeatState::Execute() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;

		auto pad = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keyState = App::GetApp()->GetInputDevice().GetKeyState();
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto elapsed = App::GetApp()->GetElapsedTime();
		auto pos = enemy->GetPos();

		pos.y = enemy->Grav().y;

		enemy->m_playerFlag = false;
		if (enemy->m_heat > 0.0f) {
			enemy->m_heat -= elapsed * 2.5f;
		}
		if (enemy->m_heat <= 0.0f) {
			enemy->EnemyAnime(L"stand");
			if (enemy->m_draw->IsTargetAnimeEnd()) {
				enemy->RetrunState();

			}
		}
		else {
			enemy->EnemyAnime(L"wait");
		}

		if (cntlVec[0].bConnected) {
			if (pad[0].wReleasedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				enemy->m_pGrabFlag = false;
			}
		}
		else if (keyState.m_bUpKeyTbl['Q'] == true) {
			enemy->m_pGrabFlag = false;
		}
		if (enemy->m_pGrabFlag) {
			auto pGrab = enemy->m_playerGrab.lock();
			if (!pGrab) return;
			auto grabTrans = pGrab->GetComponent<Transform>();
			if (enemy->m_EfkPlayer[2]) {
				enemy->m_EfkPlayer[2]->SetLocation(grabTrans->GetPosition());
			}
			if (cntlVec[0].bConnected) {
				if (enemy->m_throwFlag) {
					enemy->m_pGrabFlag = false;
					enemy->OnlyChangeState<ThrowAwayState>();
				}
			}
		}
		else {
			if (enemy->m_EfkPlayer[2]) {
				enemy->m_EfkPlayer[2]->SetLocation(enemy->GetPos());
			}

		}

		enemy->m_trans->SetPosition(pos);
		enemy->m_draw->UpdateAnimation(elapsed);

	}
	void OverHeatState::Exit() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;

		enemy->EnemyAnime(L"stand");
		enemy->EffectPlay(enemy->m_eyeEffect, enemy->GetEyePos(Vec3(2.0f, 2.5f, 0.5f)), 1, Vec3(0.5f));
		enemy->EffectPlay(enemy->m_eyeEffect, enemy->GetEyePos(Vec3(2.0f, 2.5f, -0.5f)), 2, Vec3(0.5f));
		enemy->PlaySE(L"EnemyRevival", 2.0f);
		enemy->EffectStop(3);
		enemy->m_overHeatFlag = false;
		enemy->m_throwFlag = false;

		auto gauge = enemy->m_gauge.lock();
		if (!gauge) return;
		gauge->DrawGauge(false);
		auto gaugeFram = enemy->m_gaugeFram.lock();
		if (!gaugeFram) return;
		gaugeFram->DrawGauge(false);
	}

	//--------------------------------------------------------------------------------------
	//	class ThrowAwayState : public EnemyState	//オーバーヒート中に投げるステート
	//--------------------------------------------------------------------------------------
	void ThrowAwayState::Enter() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;

		enemy->SetGrav(Vec3(0.0f, enemy->m_gravity, 0.0f));
		enemy->PlaySE(L"EnemyBurst", 2.0f);

	}
	void ThrowAwayState::Execute() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;

		auto pos = enemy->GetPos();
		float elapsed = App::GetApp()->GetElapsedTime();

		pos.y = enemy->Grav().y;

		enemy->PlayerDic();
		pos.y += 0.25f * enemy->m_throwLength;
		pos -= enemy->m_speed * enemy->m_direcNorm * elapsed * enemy->m_throwLength * 20.0f;
		enemy->m_throwTime -= elapsed;
		if (enemy->m_throwTime < 0.0f) {

			enemy->m_throwTime = 0.5f;
		}

		enemy->AroundOverHeat();
		if (enemy->m_floorFlag) {
			enemy->OnlyChangeState<OverHeatState>();
		}

		enemy->m_trans->SetPosition(pos);
		enemy->m_draw->UpdateAnimation(elapsed);
	}
	void ThrowAwayState::Exit() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;

		enemy->EffectPlay(enemy->m_burstEffect, enemy->GetEyePos(Vec3(0, 0, 0)), 4, Vec3(0.5f));

	}

	//--------------------------------------------------------------------------------------
	//	class MoveBulletState : public EnemyState	//追いかけながら弾を撃ってくるステート
	//--------------------------------------------------------------------------------------
	void MoveBulletState::Enter() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;

		enemy->SetGrav(Vec3(0.0f, enemy->m_gravity, 0.0f));
		enemy->m_speed = enemy->m_maxSpeed * 3.0f;
	}
	void MoveBulletState::Execute() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		float elapsed = App::GetApp()->GetElapsedTime();
		enemy->PlayerDic();
		auto pos = enemy->GetPos();
		Vec3 forward = enemy->m_trans->GetForward();
		float face = atan2f(forward.z, forward.x);
		Vec3 movePos;
		movePos.z = (cosf(face) * enemy->m_direcNorm.z) + (sinf(face) * enemy->m_direcNorm.x);
		Vec3 movePosNorm = movePos.normalize();
		pos += movePosNorm * elapsed * enemy->m_speed;

		pos.y = enemy->Grav().y;

		if (enemy->m_direc.length() <= enemy->m_trackingRange * 2) {
			enemy->EnemyAnime(L"walk");
			enemy->RapidFireBullet(3);
			//pos += enemy->m_speed * enemy->m_direcNorm * elapsed;
		}
		if (enemy->m_heat >= enemy->m_maxHeat) {
			enemy->ChangeState<OverHeatState>();
		}

		enemy->m_trans->SetPosition(pos);
		enemy->m_draw->UpdateAnimation(elapsed);
	}

	//--------------------------------------------------------------------------------------
	//	class SlideState : public EnemyState	　//左右移動しながら弾を撃ってくるステート
	//--------------------------------------------------------------------------------------
	void SlideState::Enter() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;

		enemy->SetGrav(Vec3(0.0f, enemy->m_gravity, 0.0f));
		enemy->m_speed = enemy->m_maxSpeed;
	}
	void SlideState::Execute() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		float elapsed = App::GetApp()->GetElapsedTime();
		auto shaft = enemy->GetDirec().cross(Vec3(0.0f, 1.0f, 0.0f));

		enemy->PlayerDic();
		enemy->EnemyAngle();
		enemy->m_maxBulletTime = 3.0f;
		enemy->StraightXBullet();
		auto pos = enemy->GetPos();
		enemy->m_rad += 0.05f;
		pos.y = enemy->Grav().y;
		if (XMConvertToRadians(enemy->m_rad) >= 360) {
			enemy->m_rad = 0;
		}

		if (enemy->m_direc.length() <= enemy->m_trackingRange * 2) {
			enemy->EnemyAnime(L"kaihi");
			pos += shaft.normalize() * sinf(enemy->m_rad) * enemy->m_speed * 2.0f * elapsed;
		}
		if (enemy->m_heat >= enemy->m_maxHeat) {
			enemy->ChangeState<OverHeatState>();
		}

		enemy->m_trans->SetPosition(pos);
		enemy->m_draw->UpdateAnimation(elapsed);
	}

	//--------------------------------------------------------------------------------------
	//	class PlungeState : public EnemyState	　//プレイヤーに向かって突っ込むステート
	//--------------------------------------------------------------------------------------
	void PlungeState::Enter() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;

		enemy->SetGrav(Vec3(0.0f, enemy->m_gravity, 0.0f));
		enemy->EnemyAnime(L"spare");
		auto pos = enemy->GetPos();
		enemy->PlayerDic();
	}
	void PlungeState::Execute() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		float elapsed = App::GetApp()->GetElapsedTime();
		auto pos = enemy->GetPos();
		pos.y = enemy->Grav().y;

		enemy->m_spareTime -= elapsed;
		m_spareTime -= elapsed;
		if (m_spareTime <= 0.0f) {
			pos += enemy->m_speed * enemy->m_direcNorm * elapsed * 3.5f;
		}
		m_time -= elapsed;
		if (m_time <= 0.0f) {
			enemy->ChangeState<ChaseState>();
		}
		if (enemy->m_heat >= enemy->m_maxHeat) {
			enemy->ChangeState<OverHeatState>();
		}

		enemy->m_trans->SetPosition(pos);
		enemy->m_draw->UpdateAnimation(elapsed);
	}
	
	//--------------------------------------------------------------------------------------
	//	class ParabolaBulletState : public EnemyState	//動かず放物線上に弾を撃ってくるステート
	//--------------------------------------------------------------------------------------
	void ParabolaBulletState::Enter() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		enemy->SetGrav(Vec3(0.0f, enemy->m_gravity, 0.0f));
	}
	void ParabolaBulletState::Execute() {
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		float elapsed = App::GetApp()->GetElapsedTime();
		auto shaft = enemy->GetDirec().cross(Vec3(0.0f, 1.0f, 0.0f));
		auto pos = enemy->GetPos();
		pos.y = enemy->Grav().y;

		enemy->EnemyAngle();
		if (enemy->m_direc.length() <= enemy->m_trackingRange * 2) {
			enemy->EnemyAnime(L"walk");
			enemy->FallBullet();
			pos += shaft.normalize() * sinf(enemy->m_rad) * enemy->m_speed * 2.0f * elapsed;
		}
		if (enemy->m_heat >= enemy->m_maxHeat) {
			enemy->ChangeState<OverHeatState>();
		}

		enemy->m_trans->SetPosition(pos);
		enemy->m_draw->UpdateAnimation(elapsed);
	}

}