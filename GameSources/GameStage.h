/*!
鎌田大輝
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
    class GameStage : public Stage {
        CsvFile m_GameStage1;                          // ゲームステージのCSVファイル
        shared_ptr<SoundItem> m_BGM;                   // バックグラウンドミュージックのサウンドアイテム
        std::shared_ptr<basecross::XAudio2Manager> m_PtrXA = App::GetApp()->GetXAudio2Manager(); // XAudio2マネージャー
        shared_ptr<GameObject> m_fadeIn;               // フェードインエフェクト用オブジェクト
        shared_ptr<GameObject> m_fadeOut;              // フェードアウトエフェクト用オブジェクト

        shared_ptr<GameObject> m_pauseBackGround;      // ポーズ時の背景オブジェクト
        shared_ptr<GameObject> m_PauseSelect;          // ポーズ選択オブジェクト
        shared_ptr<GameObject> m_PauseTitle;           // ポーズ時のタイトルオブジェクト
        shared_ptr<GameObject> m_PauseBack;            // ポーズ時の背景オブジェクト
        shared_ptr<GameObject> m_selectCharge;         // 選択画面のチャージオブジェクト
        shared_ptr<GameObject> m_titleCharge;          // タイトル画面のチャージオブジェクト
        float totaltime = 0.0f;                        // 合計時間を計測する変数

        // エフェクトのインターフェイス
        shared_ptr<EffectManeger> m_EfkInterface;

    public:
        bool m_pause = false;                          // ポーズ状態かどうかのフラグ
        bool m_soundFlg = false;

        // 構築と破棄
        GameStage() :Stage() {}
        virtual ‾GameStage() {}

        // 初期化
        virtual void OnCreate() override;

        // 更新処理
        virtual void OnUpdate() override;

        // 描画処理
        virtual void OnDraw() override;

        // バックグラウンドミュージックを再生する
        void PlayBGM(const wstring& StageBGM);

        // SEミュージックを再生する
        void  PlaySE(wstring path, float loopcnt, float volume);

        // ゲームステージの破棄処理
        void OnDestroy();

        // ステージの作成
        void CreateStagegenerator();

        // ゲームをポーズする
        void GamePause();

        void SpriteCreate();

        void EnemyUpdateChange();

        void CreateEffect();

        // エフェクトインターフェイスを取得する
        shared_ptr<EffectManeger> GetEfkInterface() const {
            return m_EfkInterface;
        }

    };

}
//end basecross

