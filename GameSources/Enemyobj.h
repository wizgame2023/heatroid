/*!
@file Emenyobj.h
@brief 敵のオブジェクト
担当：逸見
*/

#pragma once
#include "stdafx.h"
#include "Enemy.h"
#include "EnemyState.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;  //追いかけて突進する敵
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
		virtual ‾ChasingEnemy() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBulletEnemy : public Enemy;  //左右に追いかけて弾を撃ってくる敵
	//--------------------------------------------------------------------------------------
	class MoveBulletEnemy : public Enemy {
	private:
	public:
		MoveBulletEnemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const shared_ptr<Player>& player);
		virtual ‾MoveBulletEnemy() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class ParabolaBulletEnemy : public Enemy;  //放物線に弾を飛ばしてくる敵
	//--------------------------------------------------------------------------------------
	class ParabolaBulletEnemy : public Enemy {
	public:
		ParabolaBulletEnemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const shared_ptr<Player>& player);
		virtual ‾ParabolaBulletEnemy() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class SlideEnemy : public Enemy;  //左右移動しながら弾を撃ってくる敵
	//--------------------------------------------------------------------------------------
	class SlideEnemy : public Enemy {
	public:
		SlideEnemy(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotatoin,
			const Vec3& scale,
			const shared_ptr<Player>& player);
		virtual ‾SlideEnemy() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}