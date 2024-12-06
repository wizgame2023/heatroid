/*!
@file EffectManager.cpp
@brief エフェクトなど実体
*/

#include "stdafx.h"
#include "Project.h"
namespace basecross {

	//--------------------------------------------------------------------------------------
	///	Effekseerエフェクトのエフェクト
	//--------------------------------------------------------------------------------------
	EfkEffect::EfkEffect(const shared_ptr<EfkInterface>& iface, const wstring& filename) :
		m_FileName(filename),
		m_EfkInterface(iface),
		m_Effect(nullptr)
	{
		try {
			//if (m_FileName == L"") {
			//	throw BaseException(
			//		L"エフェクトファイル名が空白です。",
			//		L"if (m_FileName == L\"\")",
			//		L"EfkEffect::EfkEffect()"
			//	);
			//}
			auto m_Efk = m_EfkInterface.lock();
			m_manager = m_Efk->m_Manager;
			// エフェクトの読込
			m_Effect = ::Effekseer::Effect::Create(m_manager, (const char16_t*)filename.c_str());
		}
		catch (...) {
			throw;
		}
	}
	EfkEffect::~EfkEffect() {
	}

	void EfkEffect::OnCreate() {
	}

	//--------------------------------------------------------------------------------------
	///	EffekseerエフェクトのPlayオブジェクト
	//--------------------------------------------------------------------------------------
	EfkPlay::EfkPlay(const shared_ptr<EfkEffect>& effect, const bsm::Vec3& Emitter) :
		m_handle(-1)
	{
		try {
			m_Manager = effect->m_manager;
			m_handle = m_Manager->Play(effect->m_Effect, Emitter.x, Emitter.y, Emitter.z);
		}
		catch (...) {
			throw;
		}
	}

	EfkPlay::~EfkPlay() {
		StopEffect();
	}

	void EfkPlay::AddLocation(const bsm::Vec3& Location) {
		if (m_handle != -1) {
			m_Manager->AddLocation(m_handle, ::Effekseer::Vector3D(Location.x, Location.y, Location.z));
		}
	}

	void EfkPlay::SetRotation(const bsm::Vec3& Location, const float angle)
	{
		m_Manager->SetRotation(m_handle, ::Effekseer::Vector3D(Location.x, Location.y, Location.z), angle);
	}

	void EfkPlay::SetScale(const bsm::Vec3& Scale)
	{
		m_Manager->SetScale(m_handle, Scale.x, Scale.y, Scale.z);
	}

	void EfkPlay::StopEffect() {
		if (m_handle != -1) {
			m_Manager->StopEffect(m_handle);
		}
	}

	//--------------------------------------------------------------------------------------
	///	Effekseerエフェクトのインターフェイス
	//--------------------------------------------------------------------------------------
	EfkInterface::EfkInterface() :
		ObjectInterface(),
		m_Manager(nullptr),
		m_renderer(nullptr)
	{}
	EfkInterface::~EfkInterface() {
		// 先にエフェクト管理用インスタンスを破棄
		m_Manager.Reset();
		// 次に描画用インスタンスを破棄
		m_renderer.Reset();
	}

	void EfkInterface::OnCreate() {
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		// 描画用インスタンスの生成
		m_renderer = EffekseerRendererDX11::Renderer::Create(pDx11Device, pID3D11DeviceContext, 2000);
		// エフェクト管理用インスタンスの生成
		m_Manager = Effekseer::Manager::Create(2000);

		// 描画用インスタンスから描画機能を設定
		m_Manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
		m_Manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
		m_Manager->SetRingRenderer(m_renderer->CreateRingRenderer());
		m_Manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
		m_Manager->SetModelRenderer(m_renderer->CreateModelRenderer());

		// 描画用インスタンスからテクスチャの読込機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		m_Manager->SetTextureLoader(m_renderer->CreateTextureLoader());
		m_Manager->SetModelLoader(m_renderer->CreateModelLoader());
	}

	void  EfkInterface::OnUpdate() {
		// エフェクトの更新処理を行う
		m_Manager->Update();
	}

	void EfkInterface::OnDraw() {
		// エフェクトの描画開始処理を行う。
		m_renderer->BeginRendering();

		// エフェクトの描画を行う。
		m_Manager->Draw();

		// エフェクトの描画終了処理を行う。
		m_renderer->EndRendering();

	}

	void Mat4x4ToMatrix44(const bsm::Mat4x4& src, Effekseer::Matrix44& dest) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				dest.Values[i][j] = src(i, j);
			}
		}
	}

	void  EfkInterface::SetViewProj(const bsm::Mat4x4& view, const bsm::Mat4x4& proj) {
		Effekseer::Matrix44 v, p;
		Mat4x4ToMatrix44(view, v);
		Mat4x4ToMatrix44(proj, p);
		m_renderer->SetCameraMatrix(v);
		m_renderer->SetProjectionMatrix(p);
	}
}
//end basecross
