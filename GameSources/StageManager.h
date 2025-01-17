/*!
@file StageManager.h
@brief ステージマネージャークラス
*/

#pragma once
#include "stdafx.h"

namespace basecross {
    class StageManager : public GameObject
    {
    public:
        // カメラの選択
        enum CameraSelect {
            openingCamera,    // オープニングカメラ
	      		endingCamera,     // クリア演出カメラ
            myCamera,         // ゲームプレイカメラ
        };

        // ゲームステータスの選択
        enum GameStatus {
            TITLE,            // タイトルシーン表示時
            SELECT,           // ステージセレクトシーン表示時
            GAME_PLAYING,     // ゲームプレイ中
            TEST_PLAY         // テストプレイ中
        };

        // エフェクトのインターフェイス
        shared_ptr<EfkInterface> m_EfkInterface;

        CameraSelect m_CameraSelect;                 // 現在のカメラを保持する変数
        shared_ptr<SingleView> m_OpeningCameraView;  // オープニングカメラビュー
		    shared_ptr<SingleView> m_EndingCameraView;   // クリア演出カメラビュー
        shared_ptr<SingleView> m_MyCameraView;       // ゲームプレイカメラビュー
        int m_nowGameStatus;                         // 現在のゲームステータス
        wstring m_StageName;                         // ステージの名前
        CsvFile m_GameStage;                         // ゲームステージのCSVファイル

        // スプライト
        shared_ptr<GameObject> m_titleSprite;        // タイトルのスプライト
        shared_ptr<Transform> m_PlayerObject;        // プレイヤーオブジェクトのトランスフォーム
        shared_ptr<GameObject> m_SpriteDraw;         // スプライト描画オブジェクト
        shared_ptr<GameObject> m_TextDraw;           // テキスト描画オブジェクト
        shared_ptr<GameObject> m_StageUI;            // ステージUIオブジェクト
        shared_ptr<GameObject> m_kakaeruUI;            // ステージUIオブジェクト
        shared_ptr<GameObject> m_nextStageUI;        // 次のステージUIオブジェクト
        shared_ptr<GameObject> m_clearSelectStage;   // クリアステージ選択UIオブジェクト
        shared_ptr<GameObject> m_retryStageUI;       // リトライステージUIオブジェクト
        shared_ptr<GameObject> m_overSelectStage;    // ゲームオーバー時のステージ選択UIオブジェクト
        shared_ptr<GameObject> m_SelectCharge;       // 選択画面のチャージオブジェクト
        shared_ptr<GameObject> m_TitleCharge;        // タイトル画面のチャージオブジェクト
        shared_ptr<GameObject> m_BGfade;             // 背景フェードオブジェクト

        float m_totalTime = 0.0f;                    // 合計時間を計測する変数
        int m_select = 0;                            // 選択された項目のインデックス

        bool m_Goaltrue = false;                     // ゴールに達したかのフラグ
        bool m_Diedtrue = false;                     // 死亡したかのフラグ
        bool m_pause = false;                        // ポーズ中かのフラグ
        bool m_Flag = true;                          // フラグ
        bool m_startFlag = false;                    // タイトルのフラグ
        int m_PushState = 0;                         // ボタンの押下状態
        float m_updateTime;                           //　計測時間
        // ビューの作成
        void CreateViewLight();

        // プレイヤーの生成
        void CreatePlayer();

        // 固定ボックスの作成
        void CreateFixedBox();

        // ギミックの作成
        void CreateGimmick();

        // 敵の作成
        void CreateEnemy();

        // スプライトの作成
        void CreateSprite();

        // ゴール判定
        void GoalJudge();

        // ゲームオーバー判定
        void GameOverJudge();

        // ボタンの押下状態を設定する
        void SetPushState(const int PushState);

        // ボタンの押下状態を取得する
        int GetPushState();

        // ゲームステージを設定する
        void SetGameStageSelect(const wstring& m_csvFail);

        // スプライトを移動する
        void MoveSprite(const shared_ptr<GameObject> nextStageUI, const shared_ptr<GameObject> SelectStage);

        // 選択画面のスプライトを移動する
        void SelectMoveSprite(const shared_ptr<GameObject> nextStageUI, const shared_ptr<GameObject> SelectStage);

        // ゲームステージの選択を取得する
        wstring GetGameStageSelect();

        // 現在のゲームステータスを取得する
        int GetNowGameStatus();

        // 現在のゲームステータスを設定する
        void SetNowGameStatus(int afterGameStatus);

        // 現在のカメラステータスを取得する
        int GetNowCameraStatus();

        // メインカメラに切り替える
        void ToMainCamera();

        // オープニングカメラに切り替える
        void ToOpeningCamera();

        // クリア演出カメラに切り替える
		    void ToEndingCamera();
        
        // サウンドエフェクトを再生する
        void PlaySE(wstring path, float loopcnt, float volume);

        void UImake();

        // 構築と破棄
        StageManager(const shared_ptr<Stage>& stage) :
            GameObject(stage)
        {
        }

        virtual ~StageManager() {}

        // 初期化
        virtual void OnCreate() override;

        // 更新処理
        virtual void OnUpdate() override;

        // 描画処理
        virtual void OnDraw() override;

        // エフェクトインターフェイスを取得する
        shared_ptr<EfkInterface> GetEfkInterface() const {
            return m_EfkInterface;
        }
    };
}