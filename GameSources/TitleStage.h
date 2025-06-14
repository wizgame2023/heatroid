/*!
@file TitleStage.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {
    class TitleStage : public Stage
    {
        shared_ptr<SoundItem> m_BGM;                   // バックグラウンドミュージックのサウンドアイテム
        std::shared_ptr<basecross::XAudio2Manager> m_PtrXA = App::GetApp()->GetXAudio2Manager();

        float m_updateTime;                          //　計測時間
        bool m_startFlag = false;                    // タイトルのフラグ
        // ビューの作成
        void CreateViewLight();

    public:
        // コンストラクタ
        TitleStage() : Stage() {}
        virtual ~TitleStage() {}

        // 初期化
        virtual void OnCreate() override;

        // 更新処理
        virtual void OnUpdate() override;

        // BGM破棄処理
        void OnDestroy();

        // ボタンAが押された時の処理
        void OnPushA();

        // タイトルスプライトの描画
        void OnTitleSprite();

        // バックグラウンドミュージックを再生する
        void PlayBGM(const wstring& StageBGM);

        // サウンドエフェクトを再生する
        void PlaySE(wstring path, float loopcnt, float volume);
    };

}