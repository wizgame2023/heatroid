/*!
@file Gimmick.h
@brief Gimmickなど
鎌田
*/

#pragma once
#include "stdafx.h"

namespace basecross {
//--------------------------------------------------------------------------------------
//　タイリングする固定のボックス
//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;         // ボックスのスケール（サイズ）を保持するベクトル
		Vec3 m_Rotation;      // ボックスの回転情報を保持するベクトル
		Vec3 m_Position;      // ボックスの位置を保持するベクトル
		float m_UPic;         // テクスチャのU軸方向のピクチャサイズ
		float m_VPic;         // テクスチャのV軸方向のピクチャサイズ
		wstring m_Texname;    // テクスチャの名前
	public:
		// 構築と破棄
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const float& UPic,
			const float& VPic,
			const wstring& m_Texname
		);
		virtual ~TilingFixedBox();

		// 初期化
		virtual void OnCreate() override;
	};

	class GimmickButton;
	class GimmickDoor : public GameObject {
		Vec3 m_Position;                       // ドアの位置を保持するベクトル
		Vec3 m_Rotation;                      // ドアの回転情報を保持するベクトル
		Vec3 m_Scale;                         // ドアのスケール（サイズ）を保持するベクトル
		bool m_open;                          // ドアが開いているかのフラグ
		bool m_open2;                         // もう一つのドア開閉フラグ
		float m_UPic;                         // テクスチャのU軸方向のピクチャサイズ
		float m_VPic;                         // テクスチャのV軸方向のピクチャサイズ
		shared_ptr<GimmickButton> GDoor;      // 関連するギミックボタン
		int kazu = 1;                         // 数量を表す変数
		int m_number;                         // ドアの識別番号
		bool m_Flag;                          // フラグ
	public:
		float m_OpenSwitch;                   // ドアの開閉スイッチ
		wstring m_Texname;                    // テクスチャの名前
		// 構築と破棄
		GimmickDoor(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scales,
			float UPic,
			float VPic,
			float m_OpenSwitch,
			int number,
			const wstring& m_Texname
		);
		virtual ~GimmickDoor();

		// 初期化
		virtual void OnCreate() override;

		// 更新処理
		virtual void OnUpdate() override;

		// ドアを開く
		void OpenDoor();

		// サウンドエフェクトを再生する
		void PlaySE(wstring path, float loopcnt, float volume);
	};

	class GimmickUp : public GameObject {
		Vec3 m_Position;                       // ギミックの位置を保持するベクトル
		Vec3 m_Rotation;                      // ギミックの回転情報を保持するベクトル
		Vec3 m_Scale;                         // ギミックのスケール（サイズ）を保持するベクトル
		bool m_open;                          // ギミックが開いているかのフラグ
		bool m_open2;                         // もう一つのギミック開閉フラグ
		float m_UPic;                         // テクスチャのU軸方向のピクチャサイズ
		float m_VPic;                         // テクスチャのV軸方向のピクチャサイズ
		shared_ptr<GimmickButton> GDoor;      // 関連するギミックボタン
		int kazu = 1;                         // 数量を表す変数
		int ido = 0;                          // 移動量を表す変数
		int m_number;                         // ギミックの識別番号
		bool m_Flag;                          // フラグ
	public:
		float m_Max;                          // ギミックの最大値
		float m_OpenSwitch;                   // ギミックの開閉スイッチ
		wstring m_Texname;                    // テクスチャの名前

		// 構築と破棄
		GimmickUp(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scales,
			float UPic,
			float VPic,
			float m_OpenSwitch,
			int number,
			const wstring& m_Texname,
			float m_Max
		);
		virtual ~GimmickUp();

		// 初期化
		virtual void OnCreate() override;

		// 更新処理
		virtual void OnUpdate() override;

		// ドアを開く
		void OpenDoor();

		// サウンドエフェクトを再生する
		void PlaySE(wstring path, float loopcnt, float volume);
	};

	class GimmickButton : public GameObject
	{
	public:
		// エフェクト
		shared_ptr<EfkEffect> m_EfkEffect;           // エフェクトオブジェクト
		shared_ptr<EfkEffect> m_EfkEffectLoop;       // ループエフェクトオブジェクト
		// エフェクト実行オブジェクト
		shared_ptr<EfkPlay> m_EfkPlay;               // エフェクト実行オブジェクト

		Vec3 m_Position;                             // ギミックボタンの位置を保持するベクトル
		Vec3 m_Rotation;                            // ギミックボタンの回転情報を保持するベクトル
		Vec3 m_Scale;                               // ギミックボタンのスケール（サイズ）を保持するベクトル
		int m_switch;                               // スイッチ識別番号
		int m_number;                               // ボタンの識別番号
		wstring m_Texname;                          // テクスチャの名前

		bool m_open;                                // ボタンが押されているかのフラグ
		bool m_flag = false;                        // フラグ
		float time = 0;                             // 時間を計測する変数

		// 構築と破棄
		GimmickButton(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			int m_switch,
			int number,
			const wstring& m_Texname
		);
		virtual ~GimmickButton();

		// 初期化
		virtual void OnCreate() override;

		// 更新処理
		virtual void OnUpdate() override;

		// サウンドエフェクトを再生する
		void PlaySE(wstring path, float volume, float loopcnt);

		// エフェクトを再生する
		void EfectPlay();

		// ループエフェクトを再生する
		void EfectLoopPlay();

		// 押されているのかの判定
		bool GetButton()
		{
			return m_open;
		}

		// ボタンの状態を設定する
		void SetButton(const bool& open)
		{
			m_open = open;
		}

		// スイッチを判別する
		int GetSwitch()
		{
			return m_switch;
		}
	};

	class Door : public GameObject
	{
		Vec3 m_Position;                       // ドアの位置を保持するベクトル
		Vec3 m_Rotation;                      // ドアの回転情報を保持するベクトル
		Vec3 m_Scale;                         // ドアのスケール（サイズ）を保持するベクトル
		bool m_open;                          // ドアが開いているかのフラグ
		bool m_Goaltrue;                      // ゴールに達したかのフラグ
		wstring m_Texname;                    // テクスチャの名前
		float _delta = App::GetApp()->GetElapsedTime(); // 経過時間を保持する変数
	public:
		// 構築と破棄
		Door(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const wstring& m_Texname
		);
		virtual ~Door();

		// 初期化
		virtual void OnCreate() override;

		// 更新処理
		virtual void OnUpdate() override;

		// アニメーションを追加する
		void AddAnim();

		// アニメーションを変更する(既にそのアニメを再生中なら何もしない)
		const void SetAnim(wstring animname, float time = 0.0f) {
			auto draw = GetComponent<PNTBoneModelDraw>();
			if (draw->GetCurrentAnimation() != animname)
				draw->ChangeCurrentAnimation(animname, time);
		}

		// ボタンの状態を設定する
		void SetButton(const bool& open)
		{
			m_open = open;
		}
	};

}