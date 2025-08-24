#pragma once
/*!
@file PlayerGrab.h
@brief プレイヤーの掴み判定
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//====================================================================
	// class PlayerGrab
	// プレイヤーの掴み判定
	//====================================================================
	class Enemy;
	class PlayerGrab : public GameObject {
		//プレイヤーとの相対位置・判定の大きさ
		Vec3 m_dist, m_scale;
		//プレイヤーに追従させるためのポインタ
		weak_ptr<Player> m_player;
		//触れた敵のポインタを保管しておく
		shared_ptr<Enemy> m_target;
		//敵に当たっているかどうか
		bool m_isHit;

		shared_ptr<CollisionSphere> m_collPtr;
	public:
		PlayerGrab(const shared_ptr<Stage>& StagePtr, const shared_ptr<Player>& player) :
			GameObject(StagePtr),
			m_player(player),
			m_dist(Vec3(-5.0f, 1, 0)),
			m_scale(Vec3(8.0f)),
			m_isHit(false)
		{
		};

		~PlayerGrab() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void SetCollActive(bool update) {
			GetComponent<CollisionSphere>()->SetUpdateActive(update);
		}

		bool IsHit() {
			return m_isHit;
		}

		//掴み状態を解除
		void ClearTarget();

		//敵を投げる処理
		void ThrowTarget(float charge);

		//何かに接触している判定
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

	};
}