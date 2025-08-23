/*!
@file PlayerState.cpp
@brief プレイヤーのステートマシン・ステート実体
*/

#pragma once
#include "stdafx.h"
#include "Player.h"
#include "PlayerState.h"

namespace basecross {

	/// <summary>
	/// ステート切替
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
		//プレイヤーの移動
		m_player.lock()->MovePlayer();
		m_player.lock()->Friction();
		m_player.lock()->Gravity();

		//Aボタンでジャンプ
		if ((m_pad[0].wPressedButtons & XINPUT_GAMEPAD_A) || m_key.m_bPressedKeyTbl[VK_SPACE] == true)
		{
			m_player.lock()->Jump();
		}

		//Bボタンで射出
		if ((m_pad[0].wReleasedButtons & XINPUT_GAMEPAD_B) || m_key.m_bUpKeyTbl[VK_LCONTROL] == true)
		{
			m_player.lock()->Projectile();
		}

		//Bボタンでチャージ
		m_player.lock()->Charging((m_pad[0].wButtons & XINPUT_GAMEPAD_B) || m_key.m_bPushKeyTbl[VK_LCONTROL] == true);

		//Rボタンで敵を持つ
		m_player.lock()->GrabEnemy();

		//空中へ
		if (m_player.lock()->IsAir()) {
			m_player.lock()->ChangeState(PlayerStateMachine::player_air);
		}

	}
	void PlayerStandState::Exit() {

	}

	//空中
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
	/// 喰らい
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

	//発射
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

	//発射
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

		//ステージマネージャ取得、カメラ元に戻ったら操作可能に
		if (m_player.lock()->GetStageGen()->m_CameraSelect != StageGenerator::CameraSelect::OPENINGCAMERA) {
			m_player.lock()->ChangeState(PlayerStateMachine::player_stand);
		}
	
	}
	void PlayerStartState::Exit() {

	}

	//ゴール待機
	void PlayerGoalStandbyState::Enter() {

	}
	void PlayerGoalStandbyState::Update(float deltatime) {
		//����
		m_player.lock()->GoalStandbyBehavior();

	}
	void PlayerGoalStandbyState::Exit() {

	}

	/// <summary>
	/// ゴール
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
	/// 死亡
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