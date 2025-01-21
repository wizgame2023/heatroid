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
		Vec3 m_Position;                       // �h�A�̈ʒu��ێ�����x�N�g��
		Vec3 m_Rotation;                      // �h�A�̉�]����ێ�����x�N�g��
		Vec3 m_Scale;                         // �h�A�̃X�P�[���i�T�C�Y�j��ێ�����x�N�g��
		bool m_open;                          // �h�A���J���Ă��邩�̃t���O
		bool m_open2;                         // ������̃h�A�J�t���O
		float m_UPic;                         // �e�N�X�`����U�������̃s�N�`���T�C�Y
		float m_VPic;                         // �e�N�X�`����V�������̃s�N�`���T�C�Y
		shared_ptr<GimmickButton> GDoor;      // �֘A����M�~�b�N�{�^��
		int kazu = 1;                         // ���ʂ�\���ϐ�
		bool m_Flag;                          // �t���O
	public:
		float m_OpenSwitch;                   // �h�A�̊J�X�C�b�`
		wstring m_Texname;                    // �e�N�X�`���̖��O
		int State;
		int m_number;                         // �h�A�̎��ʔԍ�

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

		int GetState();

		int GetNumber();
	};

	class GimmickUp : public GameObject {
		Vec3 m_Position;                       // �M�~�b�N�̈ʒu��ێ�����x�N�g��
		Vec3 m_Rotation;                      // �M�~�b�N�̉�]����ێ�����x�N�g��
		Vec3 m_Scale;                         // �M�~�b�N�̃X�P�[���i�T�C�Y�j��ێ�����x�N�g��
		bool m_open;                          // �M�~�b�N���J���Ă��邩�̃t���O
		bool m_open2;                         // ������̃M�~�b�N�J�t���O
		float m_UPic;                         // �e�N�X�`����U�������̃s�N�`���T�C�Y
		float m_VPic;                         // �e�N�X�`����V�������̃s�N�`���T�C�Y
		shared_ptr<GimmickButton> GDoor;      // �֘A����M�~�b�N�{�^��
		int kazu = 1;                         // ���ʂ�\���ϐ�
		int ido = 0;                          // �ړ��ʂ�\���ϐ�
		bool m_Flag;                          // �t���O
	public:
		float m_Max;                          // �M�~�b�N�̍ő�l
		float m_OpenSwitch;                   // �M�~�b�N�̊J�X�C�b�`
		wstring m_Texname;                    // �e�N�X�`���̖��O
		int m_number;                         // �M�~�b�N�̎��ʔԍ�

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
		//ゴール演出用時間計測
		float m_animTime = 0;
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

	class DoorGimmick : public GameObject
	{
		Vec3 m_Position;                       // �h�A�̈ʒu��ێ�����x�N�g��
		Vec3 m_Rotation;                      // �h�A�̉�]����ێ�����x�N�g��
		Vec3 m_Scale;                         // �h�A�̃X�P�[���i�T�C�Y�j��ێ�����x�N�g��
		wstring m_Texname;                    // �e�N�X�`���̖��O
		float m_number;
	public:
		DoorGimmick(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const float& number
		);
		~DoorGimmick() {};
		// ������
		virtual void OnCreate() override;

		// �X�V����
		virtual void OnUpdate() override;
	};
}