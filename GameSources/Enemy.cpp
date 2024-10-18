/*!
@file Enemy.cpp
@brief ìGÇ»Ç«é¿ëÃ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//èâä˙âª
	Enemy::Enemy(const shared_ptr<Stage>& stage):
		GameObject(stage),
		m_pos(0.0f,0.0,0.0f),
		m_rot(0.0f,0.0f,0.0f),
		m_scal(1.0f,1.0f,1.0f)
	{}

	Enemy::Enemy(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotatoin,
		const Vec3& scale
	) :
		GameObject(stage),
		m_pos(position),
		m_rot(rotatoin),
		m_scal(scale)
	{}

	void Enemy::OnCreate() {
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scal);

		//ï`âÊ
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//è’ìÀîªíË
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		ptrColl->SetDrawActive(true);
		//âe
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		//èdóÕ
		auto ptrGra = AddComponent<Gravity>();

		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

	}
}