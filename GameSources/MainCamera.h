/*!
* 鎌田大輝
@file MainCamera.h
@brief カメラなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class CameraCollision :public GameObject
	{
		Vec3 GetPos;
		Vec3 TargetPos;
		bool m_Hit;
		float m_ArmLen;
		float m_ToTargetLerp;	//目標を追いかける際の補間値
		bsm::Vec3 m_TargetToAt;	//目標から視点を調整する位置ベクトル
		//回転スピード
		float m_RotSpeed;
		//左右スティック変更のモード
		bool m_LRBaseMode;
		float m_RadY;
		float m_RadXZ;


	public:
		CameraCollision(const shared_ptr<Stage>& StagePtr);
		virtual ~CameraCollision() {}
		//初期化
		virtual void OnCreate();
		//更新
		virtual void OnUpdate();
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
		void UpdateArmLengh();
		bool GetLRBaseMode() const;
		bool IsLRBaseMode() const;

	};

	class MainCamera : public Camera {
	public:
		weak_ptr<GameObject> m_TargetObject;	//目標となるオブジェクト
		float m_ToTargetLerp;	//目標を追いかける際の補間値
		bsm::Vec3 m_TargetToAt;	//目標から視点を調整する位置ベクトル
		float m_RadY;
		float m_RadXZ;
		//カメラを下げる下限角度
		float m_CameraUnderRot;
		//腕の長さの設定
		float m_ArmLen;
		float m_MaxArm;
		float m_MinArm;
		//回転スピード
		float m_RotSpeed;
		//ズームスピード
		float m_ZoomSpeed;
		//左右スティック変更のモード
		bool m_LRBaseMode;

		bool m_Hit;
		bool m_Pushbool = false;
		POINT m_beforeCursorPos{ 0, 0 };
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		MainCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	ArmLen	最初のArmの長さ
		*/
		//--------------------------------------------------------------------------------------
		MainCamera(float ArmLen);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MainCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの位置を設定する
		@param[in]	Eye	カメラ位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetEye(const bsm::Vec3& Eye)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの位置を設定する
		@param[in]	x	x位置
		@param[in]	y	y位置
		@param[in]	z	z位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetEye(float x, float y, float z)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カメラの目標オブジェクトを得る
		@return	カメラの目標
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetTargetObject() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カメラの目標オブジェクトを設定する
		@param[in]	Obj	カメラの目標オブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetObject(const shared_ptr<GameObject>& Obj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オブジェクトを追いかける場合の補間値を得る
		@return	オブジェクトを追いかける場合の補間値
		*/
		//-------------------------------------------------------------------------------------
		float GetToTargetLerp() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オブジェクトを追いかける場合の補間値を設定する
		@param[in]	f	オブジェクトを追いかける場合の補間値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetToTargetLerp(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	EyeとAtの距離を得る
		@return	EyeとAtの距離
		*/
		//--------------------------------------------------------------------------------------
		float GetArmLengh() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	EyeとAtの距離を更新する（現在のEyeとAtから求める）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateArmLengh();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	EyeとAtの距離の最大値を得る
		@return	EyeとAtの距離の最大値
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxArm() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	EyeとAtの距離の最大値を設定する
		@param[in]	f	EyeとAtの距離の最大値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMaxArm(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	EyeとAtの距離の最小値を得る
		@return	EyeとAtの距離の最小値
		*/
		//--------------------------------------------------------------------------------------
		float GetMinArm() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	EyeとAtの距離の最小値設定する
		@param[in]	f	EyeとAtの距離の最小値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMinArm(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	回転スピードを得る
		@return	回転スピード（0.0f以上）
		*/
		//--------------------------------------------------------------------------------------
		float GetRotSpeed() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	回転スピードを設定する
		@param[in]	f	回転スピード（マイナスを入力してもプラスになる）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetRotSpeed(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ターゲットからAtへの調整ベクトルを得る
		@return	ターゲットからAtへの調整ベクトル
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3 GetTargetToAt() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ターゲットからAtへの調整ベクトルを設定する
		@param[in]	v	ターゲットからAtへの調整ベクトルを
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetToAt(const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Rスティックの左右変更をBaseモードにするかどうかを得る
		@return	Baseモードならtrue（デフォルト）
		*/
		//--------------------------------------------------------------------------------------
		bool GetLRBaseMode() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Rスティックの左右変更をBaseモードにするかどうかを得る
		@return	Baseモードならtrue（デフォルト）
		*/
		//--------------------------------------------------------------------------------------
		bool IsLRBaseMode() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Rスティックの左右変更をBaseモードにするかどうかを設定する
		@param[in]	b	Baseモードならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetAt(const bsm::Vec3& At)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの視点を設定する
		@param[in]	x	x位置
		@param[in]	y	y位置
		@param[in]	z	z位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetAt(float x, float y, float z)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};

	class OpeningCameraman : public GameObject {
		Vec3 m_startPos;
		Vec3 m_endPos;
		Vec3 m_atStartPos;
		Vec3 m_atEndPos;
		Vec3 m_atPos;
		Vec3 m_eyePos;
		float m_totalTime;
		Vec3 m_secondEndPos;
		Vec3 m_secondAtEndPos;

		Vec3 m_tempStartPos;
		Vec3 m_tempEndPos;
		Vec3 m_tempAtStartPos;
		Vec3 m_tempAtEndPos;
		Vec3 m_tempAtPos;
		float m_tempTotalTime;

		//ステートマシーン
		unique_ptr< StateMachine<OpeningCameraman> >  m_StateMachine;
	public:
		//構築と破棄
		OpeningCameraman(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& EndPos,
			const Vec3& AtStartPos, const Vec3& AtEndPos, const Vec3& AtPos, float& TotalTime,
			const Vec3& secondEndPos, const Vec3& secondAtEndPos);
		virtual ~OpeningCameraman();
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;

		//アクセサ
		const unique_ptr<StateMachine<OpeningCameraman>>& GetStateMachine() {
			return m_StateMachine;
		}

		Vec3 GetAtPos() const {
			return m_atPos;
		}

		Vec3 GetEyePos() const {
			return m_eyePos;
		}
		void ToGoalEnterBehavior();
		void ToStartEnterBehavior();
		bool ExcuteBehavior(float totaltime);
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
}