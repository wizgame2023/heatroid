/*!
@file Enemy.h
@brief 敵など
担当：逸見
*/

#pragma once
#include "stdafx.h"
#include "Enemy.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class EnemyState	//敵のデフォルトステート
	//--------------------------------------------------------------------------------------
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

	//--------------------------------------------------------------------------------------
	//	class ChaseState : public EnemyState	//プレイヤーを追いかけるステート
	//--------------------------------------------------------------------------------------
	class ChaseState :public EnemyState {
	private:
		float m_plungeTime;
	public:
		ChaseState(shared_ptr<Enemy>& enemy):
			EnemyState(enemy),
			m_plungeTime(7.0f)
		{
		}
		void Enter() override;
		void Execute()override;
	};

	//--------------------------------------------------------------------------------------
	//	class OverHeatState : public EnemyState	　//オーバーヒートステート
	//--------------------------------------------------------------------------------------
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

	//--------------------------------------------------------------------------------------
	//	class ThrowAwayState : public EnemyState	//オーバーヒート中に投げるステート
	//--------------------------------------------------------------------------------------
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

	//--------------------------------------------------------------------------------------
	//	class MoveBulletState : public EnemyState	//追いかけながら弾を撃ってくるステート
	//--------------------------------------------------------------------------------------
	class MoveBulletState : public EnemyState {
	public:
		MoveBulletState(shared_ptr<Enemy>& enemy) :
			EnemyState(enemy)
		{
		}
		virtual void Enter() override;
		virtual void Execute() override;
	};

	//--------------------------------------------------------------------------------------
	//	class SlideState : public EnemyState	　//左右移動しながら弾を撃ってくるステート
	//--------------------------------------------------------------------------------------
	class SlideState : public EnemyState {
	public:
		SlideState(shared_ptr<Enemy>& enemy) :
			EnemyState(enemy)
		{
		}
		virtual void Enter() override;
		virtual void Execute() override;
	};

	//--------------------------------------------------------------------------------------
	//	class PlungeState : public EnemyState	　//プレイヤーに向かって突っ込むステート
	//--------------------------------------------------------------------------------------
	class PlungeState : public EnemyState {
	private:
		float m_time;
		float m_spareTime;
	public:
		PlungeState(shared_ptr<Enemy>& enemy) :
			EnemyState(enemy),
			m_time(3.0f),
			m_spareTime(1.0f)
		{
		}
		virtual void Enter() override;
		virtual void Execute() override;
	};

	//--------------------------------------------------------------------------------------
	//	class ParabolaBulletState : public EnemyState	//動かず放物線上に弾を撃ってくるステート
	//--------------------------------------------------------------------------------------
	class ParabolaBulletState : public EnemyState {
	public:
		ParabolaBulletState(shared_ptr<Enemy>& enemy) :
			EnemyState(enemy)
		{
		}
		virtual void Enter() override;
		virtual void Execute() override;
	};
}
