/*!
@file TitleStage.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {
    class TitleStage : public Stage
    {
        shared_ptr<SoundItem> m_BGM;                   // �o�b�N�O���E���h�~���[�W�b�N�̃T�E���h�A�C�e��
        std::shared_ptr<basecross::XAudio2Manager> m_PtrXA = App::GetApp()->GetXAudio2Manager(); // XAudio2�}�l�[�W���[

        float m_updateTime;                          //�@�v������
        bool m_startFlag = false;                    // �^�C�g���̃t���O
        // �r���[�̍쐬
        void CreateViewLight();

    public:
        // �R���X�g���N�^
        TitleStage() : Stage() {}
        virtual ~TitleStage() {}

        // ������
        virtual void OnCreate() override;

        // �X�V����
        virtual void OnUpdate() override;

        // BGM�j������
        void OnDestroy();

        // �{�^��A�������ꂽ���̏���
        void OnPushA();

        // �^�C�g���X�v���C�g�̕`��
        void OnTitleSprite();

        // �o�b�N�O���E���h�~���[�W�b�N���Đ�����
        void PlayBGM(const wstring& StageBGM);

        // �T�E���h�G�t�F�N�g���Đ�����
        void PlaySE(wstring path, float loopcnt, float volume);
    };

}