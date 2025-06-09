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
	EffectManeger::EffectManeger(const shared_ptr<Stage>& stage) :
		MultiParticle(stage),
		m_renderer(nullptr),
		m_Manager(nullptr)
	{
	}
	EffectManeger::~EffectManeger() {
		// ��ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j��
		m_Manager.Reset();
		// ���ɕ`��p�C���X�^���X��j��
		m_renderer.Reset();
	}

	void EffectManeger::OnCreate() {
		CreateEffectInterface();

		auto& app = App::GetApp();
		SetAlphaActive(true);
	}

	void EffectManeger::OnUpdate()
	{
		auto elps = App::GetApp()->GetElapsedTime();
		m_TotalTime += elps;

		// �G�t�F�N�g�̍X�V�������s��
		m_Manager->Update();
		m_renderer->SetTime(elps);

	}

	void EffectManeger::OnDraw()
	{
		auto& camera = GetStage()->GetView()->GetTargetCamera();
		SetViewProj(camera->GetViewMatrix(), camera->GetProjMatrix());
		// �G�t�F�N�g�̕`��J�n�������s���B
		m_renderer->BeginRendering();
		// �G�t�F�N�g�̕`����s���B
		m_Manager->Draw();
		
		// �G�t�F�N�g�̕`��I���������s���B
		m_renderer->EndRendering();
	}

	void EffectManeger::OnDestroy()
	{
		m_Manager->Release();
	}

	void EffectManeger::SetEffectSpeed(Effekseer::Handle& handle, const float& speed)
	{
		m_Manager->SetSpeed(handle, speed);
	}

	void EffectManeger::SetEffectPause( const bool& pause)
	{
		m_Manager->SetPausedToAllEffects(pause);
	}

	void EffectManeger::Mat4x4ToMatrix44(const bsm::Mat4x4& src, Effekseer::Matrix44& dest)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				dest.Values[i][j] = src(i, j);
			}
		}
	}

	void EffectManeger::SetViewProj(const bsm::Mat4x4& view, const bsm::Mat4x4& proj)
	{
		Effekseer::Matrix44 v, p;
		Mat4x4ToMatrix44(view, v);
		Mat4x4ToMatrix44(proj, p);
		m_renderer->SetCameraMatrix(v);
		m_renderer->SetProjectionMatrix(p);
	}

	void EffectManeger::PlayEffect(Effekseer::Handle& handle, const wstring& Key, const bsm::Vec3& Emitter, const float freme)
	{
		int32_t Freme = freme;
		m_Effect = GetEffectResource(Key);
		handle = m_Manager->Play(m_Effect, ::Effekseer::Vector3D(Emitter.x, Emitter.y, Emitter.z), Freme);
	}

	void EffectManeger::CreateEffectInterface()
	{

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		// �`��p�C���X�^���X�̐���
		m_renderer = EffekseerRendererDX11::Renderer::Create(pDx11Device, pID3D11DeviceContext, 8000);
		// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
		m_Manager = Effekseer::Manager::Create(8000);

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
		m_Manager->SetMaterialLoader(m_renderer->CreateMaterialLoader());
		m_Manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
	}

	void EffectManeger::RegisterResource(const wstring& Key, const  wstring& FileName)
	{
		try {
			// �L�[���󕶎���̏ꍇ�͕s���ȌĂяo���Ƃ��ė�O���X���[
			if (Key == L"") {
				throw BaseException(
					L"�L�[����ł��B", // �G���[���b�Z�[�W
					L"if(Key == L\"\")", // �G���[�ӏ�
					L"Effect::RegisterResource()" // �֐���
				);
			}

			// Effekseer���g�p���ăt�@�C������G�t�F�N�g�f�[�^��ǂݍ���
			auto Effect = Effekseer::Effect::Create(m_Manager, (const char16_t*)FileName.c_str());

			//--- �d���`�F�b�N ---
			map<wstring, Effekseer::EffectRef>::iterator it;
			// 1. ���ɓǂݍ��܂ꂽ�G�t�F�N�g�f�[�^�Ɠ������̂ł͂Ȃ����`�F�b�N
			for (it = m_ResMap.begin(); it != m_ResMap.end(); it++) {
				// �����G�t�F�N�g�f�[�^�����������ꍇ
				if (it->second == Effect)
				{
					// �L�[�������Ȃ�A���ɓo�^�ς݂̂��ߐ���I���i��d�o�^�h�~�j
					if (it->first == Key)
					{
						return;
					}
					// �����G�t�F�N�g�f�[�^���u�قȂ�L�[�v�œo�^����悤�Ƃ��Ă��邽�߁A��O���X���[
					wstring keyerr = L"�����G�t�F�N�g���\�[�X���ʂ̃L�[(" + it->first + L")�Ŋ��ɓo�^����Ă��܂��B�L�[: " + Key;
					throw BaseException(
						L"���\�[�X�̏d���o�^�G���[",
						keyerr,
						L"Effect::RegisterResource()"
					);
				}
			}

			// 2. �w�肳�ꂽ�L�[�����Ɏg���Ă��Ȃ����`�F�b�N
			it = m_ResMap.find(Key);
			if (it != m_ResMap.end())
			{
				// �w��̃L�[�����������i�L�[���d�����Ă���j���߁A��O���X���[
				wstring keyerr = L"�w�肳�ꂽ�L�[(" + Key + L")�͊��Ɏg�p����Ă��܂��B";
				throw BaseException(
					L"�L�[�̏d���G���[",
					keyerr,
					L"Effect::RegisterResource()"
				);
			}
			else {
				// �`�F�b�N�����ׂăp�X�����ꍇ�A���\�[�X�}�b�v�ɓo�^����
				m_ResMap[Key] = Effect;
			}
		}
		catch (...) {
			// ���̊֐����Ŕ���������O���A���̂܂܌Ăяo�����ɓ�����
			throw;
		}
	}

	Effekseer::EffectRef EffectManeger::GetEffectResource(const wstring& Key) const
	{
		// �L�[���󕶎���̏ꍇ�͕s���ȌĂяo���Ƃ��ė�O���X���[
		if (Key == L"") {
			throw BaseException(
				L"�L�[����ł��B",
				L"if(Key == L\"\")",
				L"App::GetResource()" // NOTE: EffectManeger::GetEffectResource() ����萳�m��������܂���
			);
		}

		// �}�b�v����L�[�������iconst_iterator���g�p�j
		map<wstring, Effekseer::EffectRef >::const_iterator  it;
		it = m_ResMap.find(Key);

		if (it != m_ResMap.end()) {
			// �w��̃L�[�����������ꍇ�A�Ή�����G�t�F�N�g���\�[�X��Ԃ�
			return  it->second;
		}
		else {
			// �L�[��������Ȃ��������߁A��O���X���[
			wstring keyerr = L"�w�肳�ꂽ�L�[(" + Key + L")�̃��\�[�X��������܂���B";
			throw BaseException(
				L"���\�[�X�������G���[",
				keyerr,
				L"App::GetResource()" // NOTE: EffectManeger::GetEffectResource() ����萳�m��������܂���
			);
		}
	}

	void EffectManeger::AddLocation(Effekseer::Handle& handle, const bsm::Vec3& Location) {
		if (handle != -1) {
			m_Manager->AddLocation(handle, ::Effekseer::Vector3D(Location.x, Location.y, Location.z));
		}
	}

	void EffectManeger::SetRotation(Effekseer::Handle& handle, const bsm::Vec3& Location, const float angle)
	{
		m_Manager->SetRotation(handle, ::Effekseer::Vector3D(Location.x, Location.y, Location.z), angle);
	}

	void EffectManeger::SetLocation(Effekseer::Handle& handle, const bsm::Vec3& Location) {
		m_Manager->SetLocation(handle, Location.x, Location.y, Location.z);
	}
	void EffectManeger::SetScale(Effekseer::Handle& handle, const bsm::Vec3& Scale)
	{
		m_Manager->SetScale(handle, Scale.x, Scale.y, Scale.z);
	}

	void EffectManeger::SetAllColor(Effekseer::Handle& handle, const bsm::Col4 Color)
	{
		auto color = Col4(Color) * 255;
		m_Manager->SetAllColor(handle, ::Effekseer::Color(color.x, color.y, color.z, color.w));
	}

	void EffectManeger::StopEffect(Effekseer::Handle& handle) {
		if (handle != -1) {
			m_Manager->StopEffect(handle);
		}
	}

	void EffectManeger::SetLayer(Effekseer::Handle& handle, int32_t layer)
	{
		m_Manager->SetLayer(handle, layer);
	}

}
//end basecross
