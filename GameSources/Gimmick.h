/*!
@file Gimmick.h
@brief Gimmickなど
鎌田
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class GimmickObject : public GameObject
	{
	public:
		// エフェクト関連のメンバ変数
		shared_ptr<EfkEffect> m_EfkEffect;           // エフェクトオブジェクト（単発エフェクト用）
		shared_ptr<EfkEffect> m_EfkEffectLoop;       // ループエフェクトオブジェクト（繰り返しエフェクト用）

		// エフェクト再生オブジェクト
		shared_ptr<EfkPlay> m_EfkPlay;               // エフェクトを制御および再生するためのオブジェクト

		// コンストラクタ
		GimmickObject(const shared_ptr<Stage>& StagePtr);

		// デストラクタ
		virtual ~GimmickObject();

		// 初期化処理（オブジェクトの作成時に呼び出される）
		virtual void OnCreate() override;

		// サウンドエフェクトを再生するメソッド
		// 引数:
		// - path: サウンドファイルのパス
		// - loopcnt: ループ回数
		// - volume: 再生ボリューム
		void PlaySE(wstring path, float loopcnt, float volume);

		// テクスチャマッピング用のタイルボックスを生成するメソッド
		// 引数:
		// - Scale: ボックスのスケール
		// - UPic: U軸のピクチャ数
		// - VPic: V軸のピクチャ数
		// - m_Texname: テクスチャ名
		void TilingBox(const Vec3& Scale, const float& UPic, const float& VPic, const wstring& m_Texname);
	};

	//--------------------------------------------------------------------------------------
	// タイリングする固定のボックス
	//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GimmickObject {
		Vec3 m_Scale;         // ボックスのスケール（サイズ）を保持するベクトル
		Vec3 m_Rotation;      // ボックスの回転情報を保持するベクトル
		Vec3 m_Position;      // ボックスの位置を保持するベクトル
		float m_UPic;         // テクスチャのU軸方向のピクチャ数
		float m_VPic;         // テクスチャのV軸方向のピクチャ数
		wstring m_Texname;    // テクスチャの名前

	public:
		// コンストラクタ：ドアオブジェクトの初期化
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const float& UPic,
			const float& VPic,
			const wstring& m_Texname
		);

		// デストラクタ（オブジェクト破棄時に呼び出される）
		virtual ~TilingFixedBox();

		// 初期化処理
		virtual void OnCreate() override;
	};

	class GimmickButton;
	class GimmickDoor : public GimmickObject {
		Vec3 m_Position;       // ドアの位置を保持するベクトル
		Vec3 m_Rotation;       // ドアの回転情報を保持するベクトル
		Vec3 m_Scale;          // ドアのスケール（サイズ）を保持するベクトル
		bool m_open;           // ドアが開いているかを表すフラグ
		bool m_open2;          // 追加ドアが開いているかを表すフラグ（複数ドアに対応）
		float m_UPic;          // テクスチャのU軸方向のピクチャサイズ
		float m_VPic;          // テクスチャのV軸方向のピクチャサイズ
		shared_ptr<GimmickButton> GDoor; // 対応するギミックボタンの共有ポインタ
		int kazu = 1;          // 一部の状態を表す変数（用途に応じて定義）
		bool m_flag;           // 状態を管理するフラグ

	public:
		float m_OpenSwitch;    // ドアの開閉状態を管理するスイッチ
		wstring m_Texname;     // ドアのテクスチャ名
		int State;             // ドアの現在の状態
		int m_number;          // ドアの番号

		// コンストラクタ：ドアオブジェクトの初期化
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

		// デストラクタ：リソース解放
		virtual ~GimmickDoor();

		// 初期化処理（オブジェクト作成時に呼び出される）
		virtual void OnCreate() override;

		// 更新処理（毎フレーム呼び出される）
		virtual void OnUpdate() override;

		// ドアを開くメソッド
		void OpenDoor();

		// ドアの状態を取得するメソッド
		int GetState();

		// ドアの番号を取得するメソッド
		int GetNumber();
	};

	class GimmickUp : public GimmickObject
	{
		Vec3 m_Position;      // ギミックの位置情報
		Vec3 m_Rotation;      // ギミックの回転情報
		Vec3 m_Scale;         // ギミックのスケール情報
		bool m_open;          // ギミックが開いているかの状態
		bool m_open2;         // 補助ギミックが開いているかの状態
		float m_UPic;         // テクスチャのU軸のスケール
		float m_VPic;         // テクスチャのV軸のスケール
		shared_ptr<GimmickButton> GDoor; // 関連するギミックボタン
		int kazu = 1;         // 特定の状態を表す変数（例: 数量や段階）
		int ido = 0;          // 移動量または状態変化に関連する変数
		bool m_flag;          // ギミックの状態を管理するフラグ
		float m_Max;          // 移動可能な最大範囲または値
		float m_OpenSwitch;   // ギミックの開閉スイッチ
		wstring m_Texname;    // ギミックに使用されるテクスチャの名前
		int m_number;         // ギミックの識別番号

	public:
		GimmickUp(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scales,
			float UPic,
			float VPic,
			float m_OpenSwitch,
			int number,
			const wstring& m_Texname,
			float m_Max);
		virtual ~GimmickUp();

		virtual void OnCreate() override;       // 初期化処理
		virtual void OnUpdate() override;       // 更新処理
		void UpDown();                          // 上下動作を制御
	};

	class GimmickButton : public GimmickObject
	{
		shared_ptr<EfkEffect> m_EfkEffect;      // 単発で再生されるエフェクトオブジェクト
		shared_ptr<EfkEffect> m_EfkEffectLoop;  // ループ再生されるエフェクトオブジェクト
		shared_ptr<EfkPlay> m_EfkPlay;          // エフェクトを制御および再生するためのオブジェクト
		Vec3 m_Position;                        // ボタンの位置を保持（設置されている座標）
		Vec3 m_Rotation;                        // ボタンの回転情報を保持
		Vec3 m_Scale;                           // ボタンのサイズ（スケール情報）を保持
		int m_switch;                           // ボタンに対応するスイッチの識別番号
		int m_number;                           // ボタン自体の識別番号
		wstring m_Texname;                      // ボタンに使用されるテクスチャの名前
		bool m_open;                            // ボタンが押されているかの状態を管理
		bool m_flag = false;                    // ボタンの内部状態を示すフラグ
		float time = 0;                         // タイマーを保持（エフェクトや状態遷移に使用）

	public:
		GimmickButton(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			int m_switch,
			int number,
			const wstring& m_Texname);
		virtual ~GimmickButton();

		virtual void OnCreate() override;       // 初期化処理：オブジェクト生成時に呼び出される
		virtual void OnUpdate() override;       // 更新処理：毎フレームの状態更新
		void EfectPlay();                       // 単発エフェクトを再生する
		void EfectLoopPlay();                   // ループエフェクトを再生する
		bool GetButton();                       // ボタンが押されているかを判定する
		void SetButton(const bool& open);       // ボタンの押下状態を設定する
		int GetSwitch();                        // ボタンに対応するスイッチの識別番号を取得する
	};

	class Elevator : public GimmickObject
	{
		// メンバ変数
		Vec3 m_Position;      // エレベーターの位置（座標）
		Vec3 m_Rotation;      // エレベーターの回転情報
		Vec3 m_Scale;         // エレベーターのスケール（サイズ）
		bool m_open;          // ドアが開いているかのフラグ
		bool m_goaltrue;      // ゴールに到達したかのフラグ
		wstring m_Texname;    // テクスチャ名
		float _delta;         // 経過時間
		float m_animTime;     // アニメーションの経過時間

	public:
		// コンストラクタとデストラクタ
		Elevator(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale,
			const wstring& m_Texname);
		virtual ~Elevator();

		// 基本メソッド
		virtual void OnCreate() override;       // 初期化処理
		virtual void OnUpdate() override;       // 毎フレームの更新処理

		// アニメーション関連
		void AddAnim();                         // アニメーションを追加
		const void SetAnim(wstring animname, float time = 0.0f); // アニメーションを設定

		// ドアの開閉状態を設定するメソッド
		void OpenJudge(const bool& open)
		{
			m_open = open; // ドアの開閉状態を更新
		}
	};

	class DoorGimmickNum : public GimmickObject
	{
		// メンバ変数
		Vec3 m_Position;       // ドアギミックの位置情報
		Vec3 m_UV;             // ドアギミックのUV座標情報
		Vec3 m_Scale;          // ドアギミックのスケール（サイズ）
		wstring m_Texname;     // テクスチャ名
		float m_number;        // 表示する数値

	public:
		vector<VertexPositionColorTexture> m_vertices;   // 頂点情報（位置、色、テクスチャ）
		vector<uint16_t> m_indices;                      // インデックス情報
		shared_ptr<MeshResource> m_squareMesh;           // 四角形のメッシュリソース
		wstring color;                                   // 色情報
		int m_colorSwitch;                               // 色切り替えフラグ

		// コンストラクタとデストラクタ
		DoorGimmickNum(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& UV,
			const Vec3& scale,
			const float& number,
			const wstring& color);
		~DoorGimmickNum() {};

		// 基本メソッド
		virtual void OnCreate() override;       // 初期化処理
		virtual void OnUpdate() override;       // 毎フレームの更新処理

		// 数値を変更するメソッド
		void ChangeNum();

		// ドアの開閉状態を設定するメソッド
		void OpenJudge(const bool& open);       // ドアの状態を更新
	};
}