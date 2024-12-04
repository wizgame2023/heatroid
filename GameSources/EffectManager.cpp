/*!
@file EffectManager.cpp
@brief �G�t�F�N�g�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"
#include "EffectManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	Effekseer�G�t�F�N�g�̃G�t�F�N�g
	//--------------------------------------------------------------------------------------
	EfkEffect::EfkEffect(const shared_ptr<EfkInterface>& iface, const wstring& filename) :
		m_FileName(filename),
		m_EfkInterface(iface),
		m_Effect(nullptr)
	{
		try {
			if (m_FileName == L"") {
				throw BaseException(
					L"�G�t�F�N�g�t�@�C�������󔒂ł��B",
					L"if (m_FileName == L\"\")",
					L"EfkEffect::EfkEffect()"
				);
			}
			auto m_Efk = m_EfkInterface.lock();
			m_manager = m_Efk->m_Manager;
			// �G�t�F�N�g�̓Ǎ�
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
	///	Effekseer�G�t�F�N�g��Play�I�u�W�F�N�g
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
	///	Effekseer�G�t�F�N�g�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	EfkInterface::EfkInterface() :
		ObjectInterface(),
		m_Manager(nullptr),
		m_renderer(nullptr)
	{}
	EfkInterface::~EfkInterface() {
		// ��ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j��
		m_Manager.Reset();
		// ���ɕ`��p�C���X�^���X��j��
		m_renderer.Reset();
	}

	void EfkInterface::OnCreate() {
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		// �`��p�C���X�^���X�̐���
		m_renderer = EffekseerRendererDX11::Renderer::Create(pDx11Device, pID3D11DeviceContext, 2000);
		// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
		m_Manager = Effekseer::Manager::Create(2000);

		// �`��p�C���X�^���X����`��@�\��ݒ�
		m_Manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
		m_Manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
		m_Manager->SetRingRenderer(m_renderer->CreateRingRenderer());
		m_Manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
		m_Manager->SetModelRenderer(m_renderer->CreateModelRenderer());

		// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
		// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
		m_Manager->SetTextureLoader(m_renderer->CreateTextureLoader());
		m_Manager->SetModelLoader(m_renderer->CreateModelLoader());
	}

	void  EfkInterface::OnUpdate() {
		// �G�t�F�N�g�̍X�V�������s��
		m_Manager->Update();
	}

	void EfkInterface::OnDraw() {
		// �G�t�F�N�g�̕`��J�n�������s���B
		m_renderer->BeginRendering();

		// �G�t�F�N�g�̕`����s���B
		m_Manager->Draw();

		// �G�t�F�N�g�̕`��I���������s���B
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
