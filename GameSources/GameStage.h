/*!
���c��P
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
    class GameStage : public Stage {
        CsvFile m_GameStage1;                          // �Q�[���X�e�[�W��CSV�t�@�C��
        shared_ptr<SoundItem> m_BGM;                   // �o�b�N�O���E���h�~���[�W�b�N�̃T�E���h�A�C�e��
        std::shared_ptr<basecross::XAudio2Manager> m_PtrXA = App::GetApp()->GetXAudio2Manager(); // XAudio2�}�l�[�W���[
        shared_ptr<GameObject> m_fadeIn;               // �t�F�[�h�C���G�t�F�N�g�p�I�u�W�F�N�g
        shared_ptr<GameObject> m_fadeOut;              // �t�F�[�h�A�E�g�G�t�F�N�g�p�I�u�W�F�N�g

        shared_ptr<GameObject> m_pauseBackGround;      // �|�[�Y���̔w�i�I�u�W�F�N�g
        shared_ptr<GameObject> m_PauseSelect;          // �|�[�Y�I���I�u�W�F�N�g
        shared_ptr<GameObject> m_PauseTitle;           // �|�[�Y���̃^�C�g���I�u�W�F�N�g
        shared_ptr<GameObject> m_PauseBack;            // �|�[�Y���̔w�i�I�u�W�F�N�g
        shared_ptr<GameObject> m_selectCharge;         // �I����ʂ̃`���[�W�I�u�W�F�N�g
        shared_ptr<GameObject> m_titleCharge;          // �^�C�g����ʂ̃`���[�W�I�u�W�F�N�g
        float totaltime = 0.0f;                        // ���v���Ԃ��v������ϐ�

    public:
        bool m_pause = false;                          // �|�[�Y��Ԃ��ǂ����̃t���O
        bool m_soundFlg = false;

        // �\�z�Ɣj��
        GameStage() :Stage() {}
        virtual ~GameStage() {}

        // ������
        virtual void OnCreate() override;

        // �X�V����
        virtual void OnUpdate() override;

        // �`�揈��
        virtual void OnDraw() override;

        // �o�b�N�O���E���h�~���[�W�b�N���Đ�����
        void PlayBGM(const wstring& StageBGM);

        // SE�~���[�W�b�N���Đ�����
        void  PlaySE(wstring path, float loopcnt, float volume);

        // �Q�[���X�e�[�W�̔j������
        void OnDestroy();

        // �X�e�[�W�̍쐬
        void CreateStagegenerator();

        // �Q�[�����|�[�Y����
        void GamePause();

        void SpriteCreate();

        void EnemyUpdateChange();
    };

}
//end basecross

