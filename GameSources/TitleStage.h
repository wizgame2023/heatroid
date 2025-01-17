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
        std::shared_ptr<basecross::XAudio2Manager> m_ptrXA = App::GetApp()->GetXAudio2Manager(); // XAudio2�}�l�[�W���[
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

        // �j������
        void OnDestroy();

        // �{�^��A�������ꂽ���̏���
        void OnPushA();

        // �^�C�g���X�v���C�g�̕`��
        void OnTitleSprite();

        // �o�b�N�O���E���h�~���[�W�b�N���Đ�����
        void PlayBGM(const wstring& StageBGM);

        // �X�e�[�W�}�l�[�W���[�̍쐬
        void CreateStageManager();
    };

}