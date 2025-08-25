#pragma once
/*!
@file PlayerProj.h
@brief プレイヤーの掴み判定
*/

#pragma once
#include "stdafx.h"
#include "EffectManager.h"

namespace basecross {
	//====================================================================
	// class PlayerProj
	// プレイヤーの飛び道具
	//====================================================================

	class PlayerProj : public GameObject {
		//ステージマネージャ
		weak_ptr<StageGenerator> m_stageMgr;

		//エフェクト
		shared_ptr<EffectManeger> m_efkInterface;
		Effekseer::Handle m_efkProj;
		Effekseer::Handle m_efkProjEnd;

		//時間計測
		float m_playTime = 0;

		//どれくらいの位置からスタートするか
		Vec3 m_dist;
		//速度と発射する力(0.0f〜1.0f)
		float m_speed, m_power;
		//跳んでいく方向
		Vec3 m_angle;
		//変動する速度、基底速度
		const float m_speedAdd, m_speedBase;
		//射程
		float m_range = 0, m_rangeMax;
		//壁に当たって停止
		bool m_stopped;
		//消去フラグ
		bool m_lifeEnded;

		//使用中フラグ
		bool m_used = false;

	public:
		//構築と破棄

		PlayerProj(const shared_ptr<Stage>& StagePtr);

		virtual ~PlayerProj() {}
		//アクセサ
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;

		//何かに接触している判定
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

		//エフェクト再生
		void EffectStart() {
			auto fwd = -1 * GetComponent<Transform>()->GetForward();
			auto face = atan2f(fwd.z, fwd.x);

			m_efkInterface->PlayEffect(m_efkProj, L"playerproj", GetComponent<Transform>()->GetPosition(), 0.0f);
			m_efkInterface->SetRotation(m_efkProj, Vec3(0, 1, 0), -face);
			m_efkInterface->SetScale(m_efkProj, Vec3(.8f));
		}

		//使用中か否か
		bool GetUsed() {
			return m_used;
		}

		//呼び出す
		void Invoke(const Vec3 dist, const Vec3 angle, const float power) {
			m_used = true;
			GetComponent<TriggerSphere>()->SetUpdateActive(true);
			SetUpdateActive(true);

			m_stopped = false;
			m_lifeEnded = false;
			m_angle = angle;

			GetComponent<Transform>()->SetPosition(dist);

			//速度×倍率+基底速度
			m_speed = (m_speedAdd * power) + m_speedBase;
			//持続時間
			m_range = m_rangeMax;
			EffectStart();
		}

	};
}