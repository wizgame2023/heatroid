#pragma once
/*!
@file PlayerState.h
@brief �v���C���[�̃X�e�[�g
*/

#pragma once
#include "stdafx.h"
#include "Player.h"
#include <unordered_map>

namespace basecross {

	class Player;
	class PlayerStateBase;

	class PlayerStandState;
	class PlayerAirState;
	class PlayerHitState;
	class PlayerReleaseState;
	class PlayerStartState;
	class PlayerGoalStandbyState;
	class PlayerGoalState;
	class PlayerDiedState;

	/**
	 * @brief �v���C���[�̃X�e�[�g���N���X
	 * �v���C���[�̍s���E��Ԃ��Ǘ�����I�u�W�F�N�g
	 */
	class PlayerStateBase : enable_shared_from_this<PlayerStateBase> {
	protected:
		weak_ptr<Player> m_player;
		KEYBOARD_STATE m_key;
		vector<CONTROLER_STATE> m_pad;

		//���o�A�j������p�̎��Ԍv���ϐ�
		float m_animTime = 0;
	public:
		PlayerStateBase(const shared_ptr<Player>& obj) : m_player(obj) {
		}
		~PlayerStateBase() {}

		virtual void Enter() {}
		virtual void Update(float deltatime) {
			m_key = App::GetApp()->GetInputDevice().GetKeyState();
			m_pad = App::GetApp()->GetInputDevice().GetControlerVec();
		}
		virtual void Exit() {}
	};

	//----------------------------------------------------------------
	// ��������X�e�[�g(PlayerStateBase�p��)
	//----------------------------------------------------------------
	class PlayerStandState : public PlayerStateBase {
	public:
		PlayerStandState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) { }
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	class PlayerAirState : public PlayerStateBase {
	public:
		PlayerAirState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) {}
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	class PlayerHitState : public PlayerStateBase {
	public:
		PlayerHitState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) {}
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	class PlayerReleaseState : public PlayerStateBase {
	public:
		PlayerReleaseState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) {}
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	class PlayerStartState : public PlayerStateBase {
	public:
		PlayerStartState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) {}
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	class PlayerGoalStandbyState : public PlayerStateBase {
	public:
		PlayerGoalStandbyState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) {}
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	class PlayerGoalState : public PlayerStateBase {
	public:
		PlayerGoalState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) {}
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	class PlayerDiedState : public PlayerStateBase {
	public:
		PlayerDiedState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) {}
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};

	/**
	 * @brief �v���C���[�̃X�e�[�g�}�V��
	 * �v���C���[���X�e�[�g�ғ������邽�߂̊Ǘ��I�u�W�F�N�g
	 */
	class PlayerStateMachine : enable_shared_from_this<PlayerStateMachine> {
	public:
		//�X�e�[�g�^�C�v
		enum PlayerStateType {
			player_stand,		//�n��
			player_air,			//��
			player_hit,			//����
			player_release,		//����
			player_start,		//�Q�[���J�n���o
			player_goalstandby,	//�S�[�����o�ҋ@	
			player_goal,		//�X�e�[�W�N���A
			player_died			//���S
		};

		void ChangeState(PlayerStateType type);
		void Update(float deltatime);

		PlayerStateType GetStateType() {
			return _stateType;
		}
	private:
		//���݂̃X�e�[�g�^�C�v
		PlayerStateType _stateType;
		//�X�e�[�g�ꗗ
		unordered_map<PlayerStateType, shared_ptr<PlayerStateBase>> _stateList;

		void AddState(PlayerStateType type, const shared_ptr<PlayerStateBase> state) {
			_stateList[type] = state;
		}
	public:
		//�R���X�g���N�^
		PlayerStateMachine(shared_ptr<Player> obj) {

			//����
			AddState(player_stand, shared_ptr<PlayerStandState>(new PlayerStandState(obj)));
			//��
			AddState(player_air, shared_ptr<PlayerAirState>(new PlayerAirState(obj)));
			//��炢
			AddState(player_hit, shared_ptr<PlayerHitState>(new PlayerHitState(obj)));
			//����
			AddState(player_release, shared_ptr<PlayerReleaseState>(new PlayerReleaseState(obj)));
			//�X�^�[�g
			AddState(player_start, shared_ptr<PlayerStartState>(new PlayerStartState(obj)));
			//���S
			AddState(player_died, shared_ptr<PlayerDiedState>(new PlayerDiedState(obj)));
			//�S�[�����o�ҋ@��
			AddState(player_goalstandby, shared_ptr<PlayerGoalStandbyState>(new PlayerGoalStandbyState(obj)));
			//�S�[��
			AddState(player_goal, shared_ptr<PlayerGoalState>(new PlayerGoalState(obj)));

			//�����X�e�[�g
			ChangeState(player_start);
		}
		~PlayerStateMachine() {}
	};

}