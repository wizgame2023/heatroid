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

		PlayerStateType GetStateType() {
			return _stateType;
		}
	private:
		//現在のステートタイプ
		PlayerStateType _stateType;
		//ステート一覧
		unordered_map<PlayerStateType, shared_ptr<PlayerStateBase>> _stateList;

		void AddState(PlayerStateType type, const shared_ptr<PlayerStateBase> state) {
			_stateList[type] = state;
		}
	public:
		//コンストラクタ
		PlayerStateMachine(shared_ptr<Player> obj) {

			//立ち
			AddState(player_stand, shared_ptr<PlayerStandState>(new PlayerStandState(obj)));
			//空中
			AddState(player_air, shared_ptr<PlayerAirState>(new PlayerAirState(obj)));
			//喰らい
			AddState(player_hit, shared_ptr<PlayerHitState>(new PlayerHitState(obj)));
			//発射
			AddState(player_release, shared_ptr<PlayerReleaseState>(new PlayerReleaseState(obj)));
			//スタート
			AddState(player_start, shared_ptr<PlayerStartState>(new PlayerStartState(obj)));
			//死亡
			AddState(player_died, shared_ptr<PlayerDiedState>(new PlayerDiedState(obj)));
			//ゴール演出待機中
			AddState(player_goalstandby, shared_ptr<PlayerGoalStandbyState>(new PlayerGoalStandbyState(obj)));
			//ゴール
			AddState(player_goal, shared_ptr<PlayerGoalState>(new PlayerGoalState(obj)));

			//初期ステート
			ChangeState(player_start);
		}
		~PlayerStateMachine() {}
	};

}