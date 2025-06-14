/*!
@file EffectManager.h
@brief Effect�Ȃ�
*/

#pragma once
#include "stdafx.h"


#include <Effekseer.h>
#include <EffekseerRendererDX11.h>


#pragma comment(lib, "Effekseer.lib" )
#pragma comment(lib, "EffekseerRendererDX11.lib" )


namespace basecross {
	//--------------------------------------------------------------------------------------
	///	@class	EffectManeger
	///	@brief	Effekseer���C�u�������g�p�����G�t�F�N�g�̍Đ��A�Ǘ��A�`����s���N���X
	//--------------------------------------------------------------------------------------
	class EffectManeger : public MultiParticle {

	/// @brief �ǂݍ��񂾃G�t�F�N�g���\�[�X���Ǘ�����}�b�v (�L�[: wstring, �l: EffectRef)
	map<wstring, Effekseer::EffectRef> m_ResMap;

	/// @brief �Đ����̃G�t�F�N�g�n���h��
	Effekseer::Handle handle;

	/// @brief Effekseer�̊Ǘ��}�l�[�W���[
	Effekseer::ManagerRef m_Manager;

	/// @brief Effekseer�̕`�惌���_���[ (DirectX11�p)
	EffekseerRendererDX11::RendererRef m_renderer;

	/// @brief �o�ߎ���
	float m_TotalTime;

	/// @brief �P��̃G�t�F�N�g���\�[�X�ւ̎Q�� 
	Effekseer::EffectRef m_Effect;
	public:

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	stage	�Q�[���̃X�e�[�W�I�u�W�F�N�g�ȂǁA�������ɕK�v�ȏ������I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		EffectManeger(const shared_ptr<Stage>& stage);

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		@details	Effekseer�֘A�̃��\�[�X��������܂��B
		*/
		//--------------------------------------------------------------------------------------
		virtual ~EffectManeger();

		//--------------------------------------------------------------------------------------
		/*!
		@brief	����������
		@details	Effekseer�̃}�l�[�W���[�ƃ����_���[�𐶐����A��{�I�Ȑݒ���s���܂��B
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V����
		@details	���t���[���Ăяo����AEffekseer�}�l�[�W���[���X�V���ăG�t�F�N�g�̃A�j���[�V������i�߂܂��B
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�揈���B
		@details	���t���[���Ăяo����A�Ǘ����Ă���G�t�F�N�g��`�悵�܂��B
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I������
		@details	�I�u�W�F�N�g�̔j�����ɁA�m�ۂ������\�[�X��������܂��B
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDestroy()override;

		/*!
		@brief	bsm::Mat4x4 ���� Effekseer::Matrix44 �֍s���ϊ����܂��B
		@param[in]	src		�ϊ����̍s�� (bsm::Mat4x4)
		@param[out]	dest	�ϊ���̍s�� (Effekseer::Matrix44)
		*/
		void Mat4x4ToMatrix44(const bsm::Mat4x4& src, Effekseer::Matrix44& dest);

		/*!
		@brief	�r���[�s��ƃv���W�F�N�V�����s��������_���[�ɐݒ肵�܂��B
		@param[in]	view	�r���[�s��
		@param[in]	proj	�v���W�F�N�V�����s��
		*/
		void SetViewProj(const bsm::Mat4x4& view, const bsm::Mat4x4& proj);

		/*!
		@brief	�w�肵���G�t�F�N�g�̍Đ����x��ݒ肵�܂��B
		@param[in]	handle	�Ώۂ̃G�t�F�N�g�n���h��
		@param[in]	speed	�Đ����x (1.0f���W����)
		*/
		void SetEffectSpeed(Effekseer::Handle& handle, const float& speed);

		/*!
		@brief	�}�l�[�W���[���Ǘ�����S�ẴG�t�F�N�g�̍X�V���ꎞ��~/�ĊJ���܂��B
		@param[in]	pause	true�ňꎞ��~�Afalse�ōĊJ
		*/
		void SetEffectPause(const bool& pause);

		/*!
		@brief	�w�肵���L�[�̃G�t�F�N�g���Đ����܂��B
		@param[out]	handle	�Đ����ꂽ�G�t�F�N�g�̃n���h�������̈����Ɋi�[����܂��B
		@param[in]	Key		���\�[�X�}�b�v�ɓo�^�����G�t�F�N�g�̃L�[
		@param[in]	Emitter	�G�t�F�N�g�̔������W
		@param[in]	freme	�G�t�F�N�g�̊J�n�t���[�� (0�t���[���ڂ���Đ�����ꍇ��0.0f)
		*/
		void PlayEffect(Effekseer::Handle& handle, const wstring& Key, const bsm::Vec3& Emitter, const float freme);

		/*!
		@brief	Effekseer�̃}�l�[�W���[�ƃ����_���[�𐶐����A�����ݒ���s���܂��B
		*/
		void CreateEffectInterface();

		/*!
		@brief	�G�t�F�N�g�t�@�C��(.efkefc)��ǂݍ��݁A���\�[�X�}�b�v�ɓo�^���܂��B
		@param[in]	Key			�o�^����L�[
		@param[in]	FileName	�G�t�F�N�g�t�@�C���̃p�X
		*/
		void RegisterResource(const wstring& Key, const  wstring& FileName);

		/*!
		@brief	�o�^�ς݂̃G�t�F�N�g���\�[�X���擾���܂��B
		@param[in]	Key		�擾�������G�t�F�N�g�̃L�[
		@return	Effekseer::EffectRef	�G�t�F�N�g���\�[�X�ւ̎Q�ƁB������Ȃ��ꍇ��nullptr�B
		*/
		Effekseer::EffectRef GetEffectResource(const wstring& Key) const;

		/*!
		@brief	�G�t�F�N�g�̍��W�𑊑ΓI�Ɉړ������܂��B
		@param[in]	handle		�Ώۂ̃G�t�F�N�g�n���h��
		@param[in]	Location	���݂̍��W�ɉ��Z����ړ���
		*/
		void AddLocation(Effekseer::Handle& handle, const bsm::Vec3& Location);

		/*!
		@brief	�G�t�F�N�g�̍��W���΍��W�Őݒ肵�܂��B
		@param[in]	handle		�Ώۂ̃G�t�F�N�g�n���h��
		@param[in]	Location	�ݒ肷����W
		*/
		void SetLocation(Effekseer::Handle& handle, const bsm::Vec3& Location);

		/*!
		@brief	�G�t�F�N�g�̉�]�����Ɗp�x�Őݒ肵�܂��B
		@param[in]	handle		�Ώۂ̃G�t�F�N�g�n���h��
		@param[in]	Rotation	��]��
		@param[in]	angle		��]�p�x (���W�A��)
		*/
		void SetRotation(Effekseer::Handle& handle, const bsm::Vec3& Rotation, const float angle);

		/*!
		@brief	�G�t�F�N�g�̊g��k������ݒ肵�܂��B
		@param[in]	handle	�Ώۂ̃G�t�F�N�g�n���h��
		@param[in]	Scale	XYZ�e���̊g�嗦
		*/
		void SetScale(Effekseer::Handle& handle, const bsm::Vec3& Scale);

		/*!
		@brief	�G�t�F�N�g�S�̂̐F��ݒ肵�܂��B
		@param[in]	handle	�Ώۂ̃G�t�F�N�g�n���h��
		@param[in]	color	�ݒ肷��F (RGBA)
		*/
		void SetAllColor(Effekseer::Handle& handle, const bsm::Col4 color);

		/*!
		@brief	�w�肵���G�t�F�N�g�̋����j�������܂��B
		@param[in]	handle	��~����G�t�F�N�g�̃n���h��
		*/
		void StopEffect(Effekseer::Handle& handle);

		/*!
		@brief	�G�t�F�N�g�̕`�惌�C���[��ݒ肵�܂��B
		@details	���C���[���Ƃɕ`��^�C�~���O�𐧌䂵�����ꍇ�ȂǂɎg�p���܂��B
		@param[in]	handle	�Ώۂ̃G�t�F�N�g�n���h��
		@param[in]	layer	�ݒ肷�郌�C���[�ԍ�
		*/
		void SetLayer(Effekseer::Handle& handle, int32_t layer);

		/*!
		@brief	�G�t�F�N�g�̕`�惌�C���[���擾���܂��B
		@param[in]	handle	�Ώۂ̃G�t�F�N�g�n���h��
		@return	���C���[�ԍ�
		*/
		int32_t GetLayer(Effekseer::Handle& handle)
		{
			return m_Manager->GetLayer(handle);
		}
	};
}
//end basecross
