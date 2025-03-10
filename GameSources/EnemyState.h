/*!
@file Enemy.h
@brief ìGÇ»Ç«
íSìñÅFàÌå©
*/

#pragma once
#include "stdafx.h"
#include "Enemy.h"

namespace basecross {
	class EnemyState {
	protected:
		weak_ptr<Enemy> m_enemy;
	public:
		EnemyState(shared_ptr<Enemy>& enemy):
			m_enemy(enemy)
		{
		}
		virtual ~EnemyState(){}

		virtual void Enter(){}
		virtual void Execute(){}
		virtual void Exit(){}

	};
	class ChaseState :public EnemyState {
	public:
		ChaseState(shared_ptr<Enemy>& enemy):
			EnemyState(enemy)
		{
		}
		void Enter() override;
		void Execute()override;
	};
	class OverHeatState : public EnemyState {
	public:
		OverHeatState(shared_ptr<Enemy>& enemy):
			EnemyState(enemy)
		{
		}
		virtual void Enter() override;
		virtual void Execute()override;
		virtual void Exit() override;
	};
	class ThrowAwayState : public EnemyState {
	public:
		ThrowAwayState(shared_ptr<Enemy>& enemy) :
			EnemyState(enemy)
		{
		}
		virtual void Enter() override;
		virtual void Execute() override;
		virtual void Exit() override;
	};
	class MoveBulletState : public EnemyState {
		MoveBulletState(shared_ptr<Enemy>& enemy) :
			EnemyState(enemy)
		{
		}
		virtual void Enter() override;
		virtual void Execute() override;
	};
	class SlideState : public EnemyState {
		SlideState(shared_ptr<Enemy>& enemy) :
			EnemyState(enemy)
		{
		}
		virtual void Enter() override;
		virtual void Execute() override;
	};
	

	//class EnemyStateMachine {
	//protected:
	//	weak_ptr<Enemy> m_owner;
	//	weak_ptr<EnemyState> m_currentState;
	//	weak_ptr<EnemyState> m_previousState;
	//	weak_ptr<EnemyState> m_nextState;
	//public:
	//	EnemyStateMachine(const shared_ptr<Enemy>& owner):
	//		m_owner(owner)
	//	{}
	//	virtual ~EnemyStateMachine(){}
	//	virtual void OnUpdate() {};
	//	void SetCurrentState(const shared_ptr<EnemyState>& state);
	//	void SetPreviousState(const shared_ptr<EnemyState>& state);

	//	void ChangeState(const shared_ptr<EnemyState>& newState);

	//};
}
