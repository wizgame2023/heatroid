#pragma once
/*!
@file PlayerState.h
@brief プレイヤーのステート
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
	 * @brief プレイヤーのステート基底クラス
	 * プレイヤーの行動・状態を管理するオブジェクト
	 */
	class PlayerStateBase : enable_shared_from_this<PlayerStateBase> {
	protected:
		weak_ptr<Player> m_player;
		KEYBOARD_STATE m_key;
		vector<CONTROLER_STATE> m_pad;

		//演出アニメ制御用の時間計測変数
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
	// ここからステート(PlayerStateBase継承)
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
	 * @brief プレイヤーのステートマシン
	 * プレイヤーをステート稼働させるための管理オブジェクト
	 */
	class PlayerStateMachine : enable_shared_from_this<PlayerStateMachine> {
	public:
		//ステートタイプ
		enum PlayerStateType {
			player_stand,		//地上
			player_air,			//空中
			player_hit,			//やられ
			player_release,		//発射
			player_start,		//ゲーム開始演出
			player_goalstandby,	//ゴール演出待機	
			player_goal,		//ステージクリア
			player_died			//死亡
		};

		void ChangeState(PlayerStateType type);
		void Update(float deltatime);

		PlayerStateType GetStateType() const {
			return _stateType;
		}
	private:
		//現在のステートタイプ
		PlayerStateType _stateType;
		//ステート一覧
		unordered_map<PlayerStateType, unique_ptr<PlayerStateBase>> _stateList;

		void AddState(PlayerStateType type, unique_ptr<PlayerStateBase> state) {
			_stateList[type] = move(state);
		}
	public:
		//コンストラクタ
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

			//初期ステート
			ChangeState(player_start);
		}
		~PlayerStateMachine() {}
	};

}