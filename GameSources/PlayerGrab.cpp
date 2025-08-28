/*!
@file PlayerGrab.cpp
@brief プレイヤーの掴み判定
*/

#include "stdafx.h"
#include "Project.h"
#include "PlayerGrab.h"

namespace basecross {

	//====================================================================
	// class PlayerGrab
	// プレイヤーの掴み判定
	//====================================================================
	void PlayerGrab::OnCreate() {
		auto trans = GetComponent<Transform>();
		trans->SetScale(m_scale);
		trans->SetRotation(0.0f, 0.0f, 0.0f);

		m_collPtr = AddComponent<CollisionSphere>();
		m_collPtr->SetAfterCollision(AfterCollision::None);
		m_collPtr->SetDrawActive(false);//debug
		m_collPtr->AddExcludeCollisionTag(L"Player");
		m_collPtr->AddExcludeCollisionTag(L"Attack");
		m_collPtr->AddExcludeCollisionTag(L"EnemyFloor");

		AddTag(L"PlayerGrab");
	}

	void PlayerGrab::OnUpdate() {
		auto plPtr = m_player.lock();

		//debug
		if (false) {
			bool update = m_collPtr->GetUpdateActive();
			m_collPtr->SetDrawActive(update);
		}

		//判定を出さないときはヒット判定も消す
		if (m_isHit && GetComponent<CollisionSphere>()->GetUpdateActive() == false) {
			ClearTarget();
		}

		if (m_target) {
			m_target->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<PlayerGrab>()));
			Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);
			m_target->GetComponent<Transform>()->SetPosition(pos);
			if (!m_target->GetOverHeat()) {
				ClearTarget();
			}
		}

		if (plPtr) {
			auto trans = GetComponent<Transform>();
			auto plPos = plPtr->GetComponent<Transform>()->GetPosition();
			//差分座標の回転
			auto fwd = plPtr->GetComponent<Transform>()->GetForward();
			auto plRot = atan2f(fwd.z, fwd.x);
			Vec3 distPlus;
			distPlus.x = (cosf(plRot) * m_dist.x) - (sinf(plRot) * m_dist.z);
			distPlus.y = m_dist.y;
			distPlus.z = (cosf(plRot) * m_dist.z) + (sinf(plRot) * m_dist.x);
			trans->SetPosition(plPos + distPlus);

			//プレイヤーの向きに回転を合わせる
			Vec3 r = Vec3(0);
			r.y = -plRot;
			trans->SetRotation(r);
		}
	}

	void PlayerGrab::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Enemy") && !m_target) {
			auto enemy = dynamic_pointer_cast<Enemy>(Other);
			if (enemy->GetOverHeat() == true && !m_isHit) {
				m_target = enemy;
				m_isHit = true;
				return;
			}
		}
	}

	void PlayerGrab::ClearTarget() {
		m_isHit = false;
		m_target->GetComponent<Transform>()->ClearParent();

		//最後にターゲットを解放
		m_target = nullptr;
	}

	void PlayerGrab::ThrowTarget(float charge) {
		m_isHit = false;
		if (m_target == nullptr) {
			return;
		}
		m_target->SetThorwLenght(charge);
		m_target->SetThrowFlag(true);
		m_target->GetComponent<Transform>()->ClearParent();

		//最後にターゲットを解放
		m_target = nullptr;
	}
}