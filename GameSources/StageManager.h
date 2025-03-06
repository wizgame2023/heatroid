/*!
@file StageGenerator.h
@brief ステージジェネレータークラス
*/

#pragma once
#include "stdafx.h"

namespace basecross {
    class StageGenerator : public GameObject
    {
    public:
        // カメラの選択
        enum CameraSelect {
            OPENINGCAMERA,    // オープニングカメラ
            ENDINGCAMERA,     // クリア演出カメラ
            PLAYCAMERA,         // ゲームプレイカメラ
        };

        // ゲームステータスの選択
        enum GameStatus {
            GAME_PLAYING,     // ゲームプレイ中
            TEST_PLAY         // テストプレイ中
        };

        // エフェクトのインターフェイス
        shared_ptr<EfkInterface> m_EfkInterface;
        shared_ptr<SoundItem> m_BGM;                   // バックグラウンドミュージックのサウンドアイテム
        std::shared_ptr<basecross::XAudio2Manager> m_PtrXA = App::GetApp()->GetXAudio2Manager(); // XAudio2マネージャー

        CameraSelect m_CameraSelect;                 // 現在のカメラを保持する変数
        shared_ptr<SingleView> m_OpeningCameraView;  // オープニングカメラビュー
        shared_ptr<SingleView> m_EndingCameraView;   // クリア演出カメラビュー
        shared_ptr<SingleView> m_MyCameraView;       // ゲームプレイカメラビュー
        int m_nowGameStatus;                         // 現在のゲームステータス
        wstring m_StageName;                         // ステージの名前
        CsvFile m_GameStage;                         // ゲームステージのCSVファイル

        // スプライト
        shared_ptr<GameObject> m_titleSprite;        // タイトルのスプライト
        shared_ptr<Transform> m_playerObject;        // プレイヤーオブジェクトのトランスフォーム
        shared_ptr<GameObject> m_spriteDraw;         // スプライト描画オブジェクト
        shared_ptr<GameObject> m_textDraw;           // テキスト描画オブジェクト
        shared_ptr<GameObject> m_stageUI;            // ステージUIオブジェクト
        shared_ptr<GameObject> m_kakaeruUI;          // ステージUIオブジェクト
        shared_ptr<GameObject> m_blowUI;             // ステージUIオブジェクト
        shared_ptr<GameObject> m_nextStageUI;        // 次のステージUIオブジェクト
        shared_ptr<GameObject> m_clearSelectStage;   // クリアステージ選択UIオブジェクト
        shared_ptr<GameObject> m_retryStageUI;       // リトライステージUIオブジェクト
        shared_ptr<GameObject> m_overSelectStage;    // ゲームオーバー時のステージ選択UIオブジェクト
        shared_ptr<GameObject> m_selectCharge;       // 選択画面のチャージオブジェクト
        shared_ptr<GameObject> m_titleCharge;        // タイトル画面のチャージオブジェクト
        shared_ptr<GameObject> m_backgroundFade;     // 背景フェードオブジェクト

        float m_totalTime = 0.0f;                    // 合計時間を計測する変数
        float m_clearTotalTime = 0.0f;               // クリア後の演出用
        int m_select = 0;                            // 選択された項目のインデックス

        bool m_goaltrue = false;                     // ゴールに達したかのフラグ
        bool m_diedtrue = false;                     // 死亡したかのフラグ
        bool m_pause = false;                        // ポーズ中かのフラグ
        bool m_flag = true;                          // フラグ
        bool m_fadeoutFlag = true;                   // フェードアウトフラグ
        int m_pushState = 0;                         // ボタンの押下状態
        float m_updateTime;                          //　計測時間
        bool m_startFlag = false;                    // タイトルのフラグ

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

        // バックグラウンドミュージックを再生する
        void PlayBGM(const wstring& StageBGM);

        void OperationUIMake();

        //プレイヤーから敵に飛ばすレイ
        void EnemyRay();

        // 構築と破棄
        StageGenerator(const shared_ptr<Stage>& stage) :
            GameObject(stage)
        {
        }

        virtual ~StageGenerator() {}

        // 初期化
        virtual void OnCreate() override;

        void EnemyUpdate();

        // エフェクトインターフェイスを取得する
        shared_ptr<EfkInterface> GetEfkInterface() const {
            return m_EfkInterface;
        }
    };
}