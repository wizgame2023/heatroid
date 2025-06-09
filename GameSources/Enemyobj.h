/*!
@file Emenyobj.h
@brief �G�̃I�u�W�F�N�g
�S���F�팩
*/

#pragma once
#include "stdafx.h"
#include "Enemy.h"
#include "EnemyState.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;  //�ǂ������ēːi����G
	//--------------------------------------------------------------------------------------
	class ChasingEnemy : public Enemy {
	private:
		State m_defaultState;
		State m_overheatState;

	public:
		ChasingEnemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const shared_ptr<Player>& player);
		virtual ~ChasingEnemy() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBulletEnemy : public Enemy;  //���E�ɒǂ������Ēe�������Ă���G
	//--------------------------------------------------------------------------------------
	class MoveBulletEnemy : public Enemy {
	private:
	public:
		MoveBulletEnemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const shared_ptr<Player>& player);
		virtual ~MoveBulletEnemy() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class ParabolaBulletEnemy : public Enemy;  //�������ɒe���΂��Ă���G
	//--------------------------------------------------------------------------------------
	class ParabolaBulletEnemy : public Enemy {
	public:
		ParabolaBulletEnemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const shared_ptr<Player>& player);
		virtual ~ParabolaBulletEnemy() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class SlideEnemy : public Enemy;  //���E�ړ����Ȃ���e�������Ă���G
	//--------------------------------------------------------------------------------------
	class SlideEnemy : public Enemy {
	public:
		SlideEnemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const shared_ptr<Player>& player);
		virtual ~SlideEnemy() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}