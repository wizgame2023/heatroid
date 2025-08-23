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
	//����
	class PlayerStandState : public PlayerStateBase {
	public:
		PlayerStandState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) { }
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	//��
	class PlayerAirState : public PlayerStateBase {
	public:
		PlayerAirState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) {}
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	//��炢
	class PlayerHitState : public PlayerStateBase {
	public:
		PlayerHitState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) {}
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	//��ѓ����
	class PlayerReleaseState : public PlayerStateBase {
	public:
		PlayerReleaseState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) {}
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	//�X�e�[�W�J�n���o
	class PlayerStartState : public PlayerStateBase {
	public:
		PlayerStartState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) {}
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	//�N���A���o�O
	class PlayerGoalStandbyState : public PlayerStateBase {
	public:
		PlayerGoalStandbyState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) {}
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	//�N���A���o
	class PlayerGoalState : public PlayerStateBase {
	public:
		PlayerGoalState(const shared_ptr<Player>& obj) : PlayerStateBase(obj) {}
		virtual void Enter() override;
		virtual void Update(float deltatime) override;
		virtual void Exit() override;
	};
	//���S
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

		PlayerStateType GetStateType() const {
			return _stateType;
		}
	private:
		//���݂̃X�e�[�g�^�C�v
		PlayerStateType _stateType;
		//�X�e�[�g�ꗗ
		unordered_map<PlayerStateType, unique_ptr<PlayerStateBase>> _stateList;

		void AddState(PlayerStateType type, const unique_ptr<PlayerStateBase> state) {
			_stateList[type] = state;
		}
	public:
		//�R���X�g���N�^
		PlayerStateMachine(shared_ptr<Player> obj) {

			//����
			AddState(player_stand, unique_ptr<PlayerStandState>(new PlayerStandState(obj)));
			//��
			AddState(player_air, unique_ptr<PlayerAirState>(new PlayerAirState(obj)));
			//��炢
			AddState(player_hit, unique_ptr<PlayerHitState>(new PlayerHitState(obj)));
			//����
			AddState(player_release, unique_ptr<PlayerReleaseState>(new PlayerReleaseState(obj)));
			//�X�^�[�g
			AddState(player_start, unique_ptr<PlayerStartState>(new PlayerStartState(obj)));
			//���S
			AddState(player_died, unique_ptr<PlayerDiedState>(new PlayerDiedState(obj)));
			//�S�[�����o�ҋ@��
			AddState(player_goalstandby, unique_ptr<PlayerGoalStandbyState>(new PlayerGoalStandbyState(obj)));
			//�S�[��
			AddState(player_goal, unique_ptr<PlayerGoalState>(new PlayerGoalState(obj)));

			//�����X�e�[�g
			ChangeState(player_start);
		}
		~PlayerStateMachine() {}
	};

}