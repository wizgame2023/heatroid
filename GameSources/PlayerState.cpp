/*!
@file PlayerState.cpp
@brief �v���C���[�̃X�e�[�g�}�V���E�X�e�[�g����
*/

#pragma once
#include "stdafx.h"
#include "Player.h"
#include "PlayerState.h"

namespace basecross {

	/// <summary>
	/// �X�e�[�g�ؑ�
	/// </summary>
	void PlayerStateMachine::ChangeState(PlayerStateType type) {
		if (_stateList[_stateType] != nullptr) {
			_stateList[_stateType]->Exit();
		}
		_stateType = type;
		_stateList[_stateType]->Enter();
	}

	void PlayerStateMachine::Update(float deltatime) {
		_stateList[_stateType]->Update(deltatime);
	}

	/// <summary>
	/// �����X�e�[�g
	/// </summary>
	void PlayerStandState::Enter() {

	}
	void PlayerStandState::Update(float deltatime) {
		PlayerStateBase::Update(deltatime);

		//���S�X�e�[�g��
		if (m_player.lock()->GetHP() <= 0) {
			m_player.lock()->ChangeState(PlayerStateMachine::player_died);
		}
		//�v���C���[�̈ړ�
		m_player.lock()->MovePlayer();
		m_player.lock()->Friction();
		m_player.lock()->Gravity();

		//A�{�^���ŃW�����v
		if ((m_pad[0].wPressedButtons & XINPUT_GAMEPAD_A) || m_key.m_bPressedKeyTbl[VK_SPACE] == true)
		{
			m_player.lock()->Jump();
		}

		//B�{�^���Ŏˏo
		if ((m_pad[0].wReleasedButtons & XINPUT_GAMEPAD_B) || m_key.m_bUpKeyTbl[VK_LCONTROL] == true)
		{
			m_player.lock()->Projectile();
		}

		//B�{�^���Ń`���[�W
		m_player.lock()->Charging((m_pad[0].wButtons & XINPUT_GAMEPAD_B) || m_key.m_bPushKeyTbl[VK_LCONTROL] == true);

		//R�{�^���œG������
		m_player.lock()->GrabEnemy();

		//�󒆂�
		if (m_player.lock()->IsAir()) {
			m_player.lock()->ChangeState(PlayerStateMachine::player_air);
		}

	}
	void PlayerStandState::Exit() {

	}

	//��
	void PlayerAirState::Enter() {

	}
	void PlayerAirState::Update(float deltatime) {
		PlayerStateBase::Update(deltatime);

		//���S�X�e�[�g��
		if (m_player.lock()->GetHP() <= 0) {
			m_player.lock()->ChangeState(PlayerStateMachine::player_died);
		}

		//�v���C���[�̈ړ�
		m_player.lock()->MovePlayer();
		m_player.lock()->Gravity();

	}
	void PlayerAirState::Exit() {

	}

	/// <summary>
	/// ��炢
	/// </summary>
	void PlayerHitState::Enter() {
		//�A�j���[�V�����Đ�
		m_player.lock()->SetAnim(L"GetHit_Stand");
	}
	void PlayerHitState::Update(float deltatime) {
		if (m_player.lock()->GetHP() <= 0) 
		{
			m_player.lock()->ChangeState(PlayerStateMachine::player_died);
		}

		//�v���C���[�̈ړ�
		m_player.lock()->Friction();
		m_player.lock()->Gravity();

		if (m_player.lock()->GetDrawPtr()->GetCurrentAnimation() == L"GetHit_Stand" && 
			m_player.lock()->GetDrawPtr()->GetCurrentAnimationTime() >= .33f) 
		{		
			//��or�n��
			bool air = m_player.lock()->IsAir();
			m_player.lock()->ChangeState(air ? PlayerStateMachine::player_air : PlayerStateMachine::player_stand);
		}
	}
	void PlayerHitState::Exit() {

	}

	//����
	void PlayerReleaseState::Enter() {
		m_player.lock()->SetAnim(L"Release");
	}
	void PlayerReleaseState::Update(float deltatime) {
		//�v���C���[�̈ړ�
		m_player.lock()->Gravity();
		m_player.lock()->Friction();

		//�d���I���
		if (m_player.lock()->GetDrawPtr()->GetCurrentAnimation() == L"Release" &&
			m_player.lock()->GetDrawPtr()->GetCurrentAnimationTime() >= 8.0f / 30.0f)
		{
			m_player.lock()->ChangeState(PlayerStateMachine::player_stand);
		}
	}
	void PlayerReleaseState::Exit() {

	}

	//����
	void PlayerStartState::Enter() {
		m_animTime = 0;
		m_player.lock()->SetAnim(L"Idle");
	}
	void PlayerStartState::Update(float deltatime) {
		//���v���ԃJ�E���g
		m_animTime += deltatime;

		if (m_animTime > 1.0f && m_animTime <= 3.0f) {
			//�A�j���[�V�������O�i
			m_player.lock()->SetAnim(L"Walk");
			Vec3 fwd = m_player.lock()->ForwardConvert(Vec3(1, 0, 0));
			m_player.lock()->AddMoveVel(fwd * .0035, true);
		}
		else if (m_animTime > 3.0f) {
			//�~�܂�
			m_player.lock()->SetAnim(L"Idle");

		}
		m_player.lock()->FrictionMovie();
		m_player.lock()->SpeedLimit();

		//�X�e�[�W�}�l�[�W���擾�A�J�������ɖ߂����瑀��\��
		if (m_player.lock()->GetStageGen()->m_CameraSelect != StageGenerator::CameraSelect::OPENINGCAMERA) {
			m_player.lock()->ChangeState(PlayerStateMachine::player_stand);
		}
	
	}
	void PlayerStartState::Exit() {

	}

	//�S�[���ҋ@
	void PlayerGoalStandbyState::Enter() {

	}
	void PlayerGoalStandbyState::Update(float deltatime) {
		//����
		m_player.lock()->GoalStandbyBehavior();

	}
	void PlayerGoalStandbyState::Exit() {

	}

	/// <summary>
	/// �S�[��
	/// </summary>
	void PlayerGoalState::Enter() {
	}
	void PlayerGoalState::Update(float deltatime) {
		//����
		m_player.lock()->GoalBehavior();
	}
	void PlayerGoalState::Exit() {

	}

	/// <summary>
	/// ���S
	/// </summary>
	void PlayerDiedState::Enter() {
	}
	void PlayerDiedState::Update(float deltatime) {
		if (m_player.lock()->IsAir()) {
			m_player.lock()->SetAnim(L"GetHit_Air");
			m_player.lock()->Gravity();
		}
		else {
			m_player.lock()->Friction();
			m_player.lock()->Died();
		}

	}
	void PlayerDiedState::Exit() {

	}

}