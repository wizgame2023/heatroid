/*!
@file Emenyobj.cpp
@brief 敵のオブジェクト
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Enemy;

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
		const shared_ptr<Player>& player
	):
		Enemy(stage, position, rotation, scale, player)
	{}
	void ChasingEnemy::OnCreate() {
		Enemy::OnCreate();
		m_currentState = make_unique<ChaseState>(dynamic_pointer_cast<Enemy>(GetThis<ChasingEnemy>()));
		m_currentState->Enter();
	}
	void ChasingEnemy::OnUpdate() {
		if (m_currentState) {
			m_currentState->Execute();
		}
	}

	MoveBulletEnemy::MoveBulletEnemy(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotatoin,
		const Vec3& scale,
		const shared_ptr<Player>& player
	) :
		Enemy(stage,position,rotatoin,scale,player)
	{
	}
	void MoveBulletEnemy::OnCreate() {
		Enemy::OnCreate();
		m_currentState = make_unique<MoveBulletState>(dynamic_pointer_cast<Enemy>(GetThis<MoveBulletEnemy>()));
		m_currentState->Enter();
	}
	void MoveBulletEnemy::OnUpdate() {
		if (m_currentState) {
			m_currentState->Execute();
		}
	}

	ParabolaBulletEnemy::ParabolaBulletEnemy(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotatoin,
		const Vec3& scale,
		const shared_ptr<Player>& player
	) :
		Enemy(stage, position, rotatoin, scale, player)
	{
	}
	void ParabolaBulletEnemy::OnCreate() {
		Enemy::OnCreate();
		m_currentState = make_unique<ParabolaBulletState>(dynamic_pointer_cast<Enemy>(GetThis<ParabolaBulletEnemy>()));
		m_currentState->Enter();
	}
	void ParabolaBulletEnemy::OnUpdate() {
		if (m_currentState) {
			m_currentState->Execute();
		}
	}

	SlideEnemy::SlideEnemy(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotatoin,
		const Vec3& scale,
		const shared_ptr<Player>& player
	) :
		Enemy(stage, position, rotatoin, scale, player)
	{
	}

	void SlideEnemy::OnCreate() {
		Enemy::OnCreate();
		m_currentState = make_unique<SlideState>(dynamic_pointer_cast<Enemy>(GetThis<SlideEnemy>()));
		m_currentState->Enter();


		auto pos = m_beforePos;
		float rad = XMConvertToRadians(180.0f);

		//auto gauge = GetStage()->AddGameObject<GaugeSquare>(4.0f, 2.0f, L"OverHeatGauge",
		//	Col4(1.0f, 0.0f, 0.0f, 1.0f), GetThis<Enemy>());
		//auto gaugeFram = GetStage()->AddGameObject<Square>(4.0f, 2.0f, L"OverHeatFram",
		//	Col4(1.0f, 1.0f, 1.0f, 1.0f), GetThis<Enemy>());

		m_meshName = L"ENEMYYOKO";
		m_scal.y = m_scal.y;
		pos.y = pos.y + m_scal.y;

		Mat4x4 meshMat;
		meshMat.affineTransformation(
			Vec3(1.0f, 1.0f * 2.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, rad, 0.0f),
			Vec3(0.0f, -1.5f, 0.0f)
		);
		m_draw->SetMeshToTransformMatrix(meshMat);
		m_gauge.lock()->SetPosHight(6.0f);
		m_gaugeFram.lock()->SetPosHight(6.0f);
		m_floorCol->SetPosHight(3.0f);

		m_trans->SetScale(m_scal);
		m_trans->SetPosition(pos);
		//描画
		m_draw->SetMeshResource(m_meshName);
		m_draw->SetOwnShadowActive(true);


		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(m_meshName);

		m_draw->AddAnimation(L"stand", 160, 20, false, 30);
		m_draw->AddAnimation(L"wait", 120, 30, false, 30);   //オーバーヒート状態


	}
	void SlideEnemy::OnUpdate() {
		if (m_currentState) {
			m_currentState->Execute();
		}
	}
}

