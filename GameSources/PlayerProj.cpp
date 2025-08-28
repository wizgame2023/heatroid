/*!
@file PlayerProj.cpp
@brief プレイヤーの飛び道具
*/

#include "stdafx.h"
#include "Project.h"
#include "PlayerGrab.h"

namespace basecross {

	//====================================================================
	// class PlayerProj
	// プレイヤーの飛び道具
	//====================================================================

	PlayerProj::PlayerProj(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_speedAdd(24.0f),
		m_speedBase(24.0f),
		m_rangeMax(.4f),
		m_stopped(false),
		m_lifeEnded(false),
		m_speed(0),
		m_power(0)
	{
	}

	void PlayerProj::OnCreate() {

		//ステージマネージャ
		m_stageMgr = GetStage()->GetSharedGameObject<StageGenerator>(L"StageManager");

		//エフェクト読み込み
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring ProjEffectStr = DataDir + L"Effects\\playerproj.efk";
		wstring EndEffectStr = DataDir + L"Effects\\playerprojend.efk";
		auto ShEfkInterface = m_stageMgr.lock()->GetEfkInterface();
		m_EfkProj = ObjectFactory::Create<EfkEffect>(ShEfkInterface, ProjEffectStr);
		m_EfkProjEnd = ObjectFactory::Create<EfkEffect>(ShEfkInterface, EndEffectStr);

		auto trans = GetComponent<Transform>();
		trans->SetScale(Vec3(8.0f));
		trans->SetRotation(Vec3(0));
		trans->SetPosition(Vec3(0));

		auto coll = AddComponent<TriggerSphere>();
		coll->SetDrawActive(false);//debug
		coll->SetAfterCollision(AfterCollision::None);
		coll->AddExcludeCollisionTag(L"Player");
		coll->AddExcludeCollisionTag(L"Attack");

		//描画コンポーネントの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetBlendState(BlendState::Additive);
		ptrDraw->SetDrawActive(false);//debug

		Mat4x4 meshMat;
		meshMat.affineTransformation(
			Vec3(2.0f / trans->GetScale().x, 2.0f / trans->GetScale().y, 2.0f / trans->GetScale().z), //(.1f, .1f, .1f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);
		ptrDraw->SetMeshToTransformMatrix(meshMat);

		ptrDraw->SetTextureResource(L"FIRE");

		AddTag(L"Attack");

	}

	void PlayerProj::OnUpdate() {
		//ポーズ解除後に一瞬動くのを防止するため
		if (!m_used) {
			GetComponent<TriggerSphere>()->SetUpdateActive(false);
			SetUpdateActive(false);
			return;
		}

		auto trans = GetComponent<Transform>();

		auto delta = App::GetApp()->GetElapsedTime();
		m_playTime += delta;

		if (m_EfkPlay[0]) {
			m_EfkPlay[0]->SetLocation(trans->GetPosition());
		}

		if (!m_stopped) {
			trans->SetPosition(trans->GetPosition() + (m_angle * m_speed * delta));
		}

		m_range -= delta;

		if (m_range <= 0 && !m_lifeEnded) {
			m_lifeEnded = true;
			m_playTime = 0;
			auto fwd = -1 * trans->GetForward();
			auto face = atan2f(fwd.z, fwd.x);

			if (m_EfkPlay[0]) {
				m_EfkPlay[0]->StopEffect();
			}
			m_EfkPlay[1] = ObjectFactory::Create<EfkPlay>(m_EfkProjEnd, trans->GetPosition(), 0.0f);
			m_EfkPlay[1]->SetRotation(Vec3(0, 1, 0), -face);
			m_EfkPlay[1]->SetScale(Vec3(.8f));
		}

		if (m_lifeEnded) {
			m_EfkPlay[1]->SetLocation(trans->GetPosition());
			if (m_range <= -.3f) {
				m_EfkPlay[1]->StopEffect();
				m_used = false;
				SetUpdateActive(false);
			}
		}
	}

	//壁に当たったら止まる
	void PlayerProj::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FixedBox")) {
			m_stopped = true;
		}
	}

}