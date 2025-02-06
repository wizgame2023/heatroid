/*!
* 鎌田大輝
@file MainCamera.h
@brief カメラなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {



	class CameraCollision : public GameObject
	{
		Vec3 GetPos;                            // カメラの現在位置を保持するベクトル
		Vec3 TargetPos;                         // カメラの目標位置を保持するベクトル
		bool m_Hit;                             // ヒット判定フラグ
		float m_ArmLen;                         // 腕の長さを保持する変数
		float m_ToTargetLerp;                   // 目標を追いかける際の補間値
		bsm::Vec3 m_TargetToAt;                 // 目標から視点を調整する位置ベクトル
		float m_RotSpeed;						// 回転スピード
		bool m_LRBaseMode;						// 左右スティック変更のモード
		float m_RadY;                           // Y軸の回転角度
		float m_RadXZ;                          // XZ平面の回転角度

	public:
		// コンストラクタ
		CameraCollision(const shared_ptr<Stage>& StagePtr);
		virtual ~CameraCollision() {}

		// 初期化
		virtual void OnCreate();

		// 更新処理
		virtual void OnUpdate();

		// 衝突処理の実行
		virtual void OnCollisionExcute(const CollisionPair& Pair) override;

		// 衝突開始時の処理
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

		// 衝突終了時の処理
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;

		// 腕の長さを更新する
		void UpdateArmLengh();

		// 左右スティック変更のモードを取得する
		bool GetLRBaseMode() const;

		// 左右スティック変更のモードかどうかを判定する
		bool IsLRBaseMode() const;
	};

	class MainCamera : public Camera {
	public:
		weak_ptr<GameObject> m_TargetObject;    // 目標となるオブジェクト
		float m_ToTargetLerp;                   // 目標を追いかける際の補間値
		bsm::Vec3 m_TargetToAt;                 // 目標から視点を調整する位置ベクトル
		float m_RadY;                           // Y軸の回転角度
		float m_RadXZ;                          // XZ平面の回転角度
		float m_CameraUnderRot;					// カメラを下げる下限角度
		float m_ArmLen;							// 腕の長さの設定
		float m_MaxArm;                         // 腕の最大長さ
		float m_MinArm;                         // 腕の最小長さ
		float m_RotSpeed;						// 回転スピード
		float m_ZoomSpeed;						// ズームスピード
		bool m_LRBaseMode;						// 左右スティック変更のモード

		bool m_Hit;                             // ヒット判定
		bool m_Pushbool = false;                // ボタン押下の状態
		POINT m_beforeCursorPos{ 0, 0 };        // カーソルの前回位置

		// コンストラクタ
		MainCamera();
		MainCamera(float ArmLen);
		virtual ~MainCamera();

		// カメラの視点を設定する（ベクトル）
		virtual void SetEye(const bsm::Vec3& Eye) override;
		// カメラの視点を設定する（座標）
		virtual void SetEye(float x, float y, float z) override;

		// 目標オブジェクトを取得する
		shared_ptr<GameObject> GetTargetObject() const;

		// 目標オブジェクトを設定する
		void SetTargetObject(const shared_ptr<GameObject>& Obj);

		// 補間値を取得する
		float GetToTargetLerp() const;

		// 補間値を設定する
		void SetToTargetLerp(float f);

		// 腕の長さを取得する
		float GetArmLengh() const;

		// 腕の長さを更新する
		void UpdateArmLengh();

		// 腕の最大長さを取得する
		float GetMaxArm() const;

		// 腕の最大長さを設定する
		void SetMaxArm(float f);

		// 腕の最小長さを取得する
		float GetMinArm() const;

		// 腕の最小長さを設定する
		void SetMinArm(float f);

		// 回転スピードを取得する
		float GetRotSpeed() const;

		// 回転スピードを設定する
		void SetRotSpeed(float f);

		// 目標から視点を調整する位置ベクトルを取得する
		bsm::Vec3 GetTargetToAt() const;

		// 目標から視点を調整する位置ベクトルを設定する
		void SetTargetToAt(const bsm::Vec3& v);

		// 左右スティック変更のモードを取得する
		bool GetLRBaseMode() const;

		// 左右スティック変更のモードかどうかを判定する
		bool IsLRBaseMode() const;

		// 注視点を設定する（ベクトル）
		virtual void SetAt(const bsm::Vec3& At) override;

		// 注視点を設定する（座標）
		virtual void SetAt(float x, float y, float z) override;

		// 更新処理
		virtual void OnUpdate() override;
	};

//--------------------------------------------------------------------------------------
//	class RayMark : public GameObject; //オブジェクトに触れているかどうか
//--------------------------------------------------------------------------------------
	class RayCameraMark : public GameObject {
	private:
		bool m_hitEnemyFlag;
		bool m_hitCameraFlag;
		bool m_activeFlag;
		int m_wallCnt;
		int m_doorCnt;
		Vec3 m_pos;

		vector<bool> m_hitWallFlag;
		vector<bool> m_hitDoorFlag;
		shared_ptr<Transform> m_trans;
		shared_ptr<PNTStaticDraw> m_draw;
		weak_ptr<Player> m_player;
		weak_ptr<Camera> m_camera;


	public:
		RayCameraMark(const shared_ptr<Stage>& stage);
		RayCameraMark(const shared_ptr<Stage>& stage, const shared_ptr <Player>& player, const shared_ptr<Camera>& enemy);
		virtual ~RayCameraMark() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void Debug();
		Vec3 enemyPos(Vec3 pos);
		bool GetActiveFlag();
		Vec3 GetActivePos();

	};


	class OpeningCameraman : public GameObject {
		Vec3 m_startPos;                          // カメラの開始位置を保持するベクトル
		Vec3 m_endPos;                            // カメラの終了位置を保持するベクトル
		Vec3 m_atStartPos;                        // 注視点の開始位置を保持するベクトル
		Vec3 m_atEndPos;                          // 注視点の終了位置を保持するベクトル
		Vec3 m_atPos;                             // 現在の注視点位置を保持するベクトル
		Vec3 m_eyePos;                            // 現在のカメラ位置を保持するベクトル
		float m_totalTime;                        // 合計時間を保持する変数
		Vec3 m_secondEndPos;                      // 二次終了位置を保持するベクトル
		Vec3 m_secondAtEndPos;                    // 二次注視点終了位置を保持するベクトル

		Vec3 m_tempStartPos;                      // 一時的な開始位置を保持するベクトル
		Vec3 m_tempEndPos;                        // 一時的な終了位置を保持するベクトル
		Vec3 m_tempAtStartPos;                    // 一時的な注視点開始位置を保持するベクトル
		Vec3 m_tempAtEndPos;                      // 一時的な注視点終了位置を保持するベクトル
		Vec3 m_tempAtPos;                         // 一時的な注視点位置を保持するベクトル
		float m_tempTotalTime;                    // 一時的な合計時間を保持する変数

		// ステートマシン
		unique_ptr< StateMachine<OpeningCameraman> >  m_StateMachine;
	public:
		// 構築と破棄
		OpeningCameraman(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& EndPos,
			const Vec3& AtStartPos, const Vec3& AtEndPos, const Vec3& AtPos, float& TotalTime,
			const Vec3& secondEndPos, const Vec3& secondAtEndPos);
		virtual ~OpeningCameraman();

		// 初期化
		virtual void OnCreate() override;

		// 操作
		virtual void OnUpdate() override;

		// アクセサ
		const unique_ptr<StateMachine<OpeningCameraman>>& GetStateMachine() {
			return m_StateMachine;
		}

		Vec3 GetAtPos() const {
			return m_atPos;
		}

		Vec3 GetEyePos() const {
			return m_eyePos;
		}

		// ゴールエンタービヘイビア
		void ToGoalEnterBehavior();

		// スタートエンタービヘイビア
		void ToStartEnterBehavior();

		// 行動を実行する
		bool ExcuteBehavior(float totaltime);

		// 終了状態エンタービヘイビア
		void EndStateEnterBehavior();
	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToGoalState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class OpeningCameramanToFirstState : public ObjState<OpeningCameraman>
	{
		OpeningCameramanToFirstState() {}
	public:
		static shared_ptr<OpeningCameramanToFirstState> Instance();
		virtual void Enter(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Execute(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Exit(const shared_ptr<OpeningCameraman>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToStartState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class OpeningCameramanToSecondState : public ObjState<OpeningCameraman>
	{
		OpeningCameramanToSecondState() {}
	public:
		static shared_ptr<OpeningCameramanToSecondState> Instance();
		virtual void Enter(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Execute(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Exit(const shared_ptr<OpeningCameraman>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanEndState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class OpeningCameramanEndState : public ObjState<OpeningCameraman>
	{
		OpeningCameramanEndState() {}
	public:
		static shared_ptr<OpeningCameramanEndState> Instance();
		virtual void Enter(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Execute(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Exit(const shared_ptr<OpeningCameraman>& Obj)override;
	};

	class OpeningCamera : public Camera {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		OpeningCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~OpeningCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};

	//ステージクリア時のカメラ
	class EndingCameraman : public GameObject {
		Vec3 m_startPos;
		Vec3 m_endPos;
		Vec3 m_atStartPos;
		Vec3 m_atEndPos;
		Vec3 m_eyePos;
		Vec3 m_atPos;
		float m_totalTime;

		//ステートマシーン
		unique_ptr< StateMachine<EndingCameraman> >  m_StateMachine;
	public:
		//構築と破棄
		EndingCameraman(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& EndPos,
			const Vec3& AtStartPos, const Vec3& AtEndPos, const float& TotalTime);
		virtual ~EndingCameraman();
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;

		//アクセサ
		const unique_ptr<StateMachine<EndingCameraman>>& GetStateMachine() {
			return m_StateMachine;
		}

		Vec3 GetAtPos() const {
			return m_atPos;
		}

		Vec3 GetEyePos() const {
			return m_eyePos;
		}

		void ExcuteBehavior(float totaltime);
		void BasicStateEnterBehavior();
	};

	//--------------------------------------------------------------------------------------
	//	class EndingCameramanBasicState : public ObjState<EndingCameraman>;
	//--------------------------------------------------------------------------------------
	class EndingCameramanBasicState : public ObjState<EndingCameraman>
	{
		EndingCameramanBasicState() {}
	public:
		static shared_ptr<EndingCameramanBasicState> Instance() {
			static shared_ptr<EndingCameramanBasicState> instance(new EndingCameramanBasicState);
			return instance;
		};
		virtual void Enter(const shared_ptr<EndingCameraman>& Obj)override;
		virtual void Execute(const shared_ptr<EndingCameraman>& Obj)override;
		virtual void Exit(const shared_ptr<EndingCameraman>& Obj)override;
	};

	class EndingCamera : public Camera {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		EndingCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~EndingCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};

}

