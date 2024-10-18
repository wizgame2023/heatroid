/*!
@file Enemy.cpp
@brief ìGÇ»Ç«é¿ëÃ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//èâä˙âª
	Enemy::Enemy(const shared_ptr<Stage>& stage):
		GameObject(stage)
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

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFALUT_CUBE");

		auto ptrColl = AddComponent<Collision>();
		ptrColl->SetFixed(true);
		ptrColl->SetDrawActive(true);


	}
}