/*!
@file Enemy.h
@brief �G�Ȃ�
�S���F�팩
*/

#pragma once
#include "stdafx.h"
#include "Enemy.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class EnemyState	//�G�̃f�t�H���g�X�e�[�g
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
	//	class ChaseState : public EnemyState	//�v���C���[��ǂ�������X�e�[�g
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
	//	class OverHeatState : public EnemyState	�@//�I�[�o�[�q�[�g�X�e�[�g
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
	//	class ThrowAwayState : public EnemyState	//�I�[�o�[�q�[�g���ɓ�����X�e�[�g
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
	//	class MoveBulletState : public EnemyState	//�ǂ������Ȃ���e�������Ă���X�e�[�g
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
	//	class SlideState : public EnemyState	�@//���E�ړ����Ȃ���e�������Ă���X�e�[�g
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
	//	class PlungeState : public EnemyState	�@//�v���C���[�Ɍ������ē˂����ރX�e�[�g
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
	//	class ParabolaBulletState : public EnemyState	//��������������ɒe�������Ă���X�e�[�g
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
