/*!
@file StageManager.h
@brief �X�e�[�W�}�l�[�W���[�N���X
*/

#pragma once
#include "stdafx.h"

namespace basecross {
    class StageManager : public GameObject
    {
    public:
        // �J�����̑I��
        enum CameraSelect {
            openingCamera,    // �I�[�v�j���O�J����
            myCamera,         // �Q�[���v���C�J����
        };

        // �Q�[���X�e�[�^�X�̑I��
        enum GameStatus {
            TITLE,            // �^�C�g���V�[���\����
            SELECT,           // �X�e�[�W�Z���N�g�V�[���\����
            GAME_PLAYING,     // �Q�[���v���C��
            TEST_PLAY         // �e�X�g�v���C��
        };

        // �G�t�F�N�g�̃C���^�[�t�F�C�X
        shared_ptr<EfkInterface> m_EfkInterface;

        CameraSelect m_CameraSelect;                 // ���݂̃J������ێ�����ϐ�
        shared_ptr<SingleView> m_OpeningCameraView;  // �I�[�v�j���O�J�����r���[
        shared_ptr<SingleView> m_MyCameraView;       // �Q�[���v���C�J�����r���[
        int m_nowGameStatus;                         // ���݂̃Q�[���X�e�[�^�X
        wstring m_StageName;                         // �X�e�[�W�̖��O
        CsvFile m_GameStage;                         // �Q�[���X�e�[�W��CSV�t�@�C��

        // �X�v���C�g
        shared_ptr<GameObject> m_titleSprite;        // �^�C�g���̃X�v���C�g
        shared_ptr<Transform> m_PlayerObject;        // �v���C���[�I�u�W�F�N�g�̃g�����X�t�H�[��
        shared_ptr<GameObject> m_SpriteDraw;         // �X�v���C�g�`��I�u�W�F�N�g
        shared_ptr<GameObject> m_TextDraw;           // �e�L�X�g�`��I�u�W�F�N�g
        shared_ptr<GameObject> m_StageUI;            // �X�e�[�WUI�I�u�W�F�N�g
        shared_ptr<GameObject> m_kakaeruUI;            // �X�e�[�WUI�I�u�W�F�N�g
        shared_ptr<GameObject> m_nextStageUI;        // ���̃X�e�[�WUI�I�u�W�F�N�g
        shared_ptr<GameObject> m_clearSelectStage;   // �N���A�X�e�[�W�I��UI�I�u�W�F�N�g
        shared_ptr<GameObject> m_retryStageUI;       // ���g���C�X�e�[�WUI�I�u�W�F�N�g
        shared_ptr<GameObject> m_overSelectStage;    // �Q�[���I�[�o�[���̃X�e�[�W�I��UI�I�u�W�F�N�g
        shared_ptr<GameObject> m_SelectCharge;       // �I����ʂ̃`���[�W�I�u�W�F�N�g
        shared_ptr<GameObject> m_TitleCharge;        // �^�C�g����ʂ̃`���[�W�I�u�W�F�N�g
        shared_ptr<GameObject> m_BGfade;             // �w�i�t�F�[�h�I�u�W�F�N�g

        float m_totalTime = 0.0f;                    // ���v���Ԃ��v������ϐ�
        int m_select = 0;                            // �I�����ꂽ���ڂ̃C���f�b�N�X

        bool m_Goaltrue = false;                     // �S�[���ɒB�������̃t���O
        bool m_Diedtrue = false;                     // ���S�������̃t���O
        bool m_pause = false;                        // �|�[�Y�����̃t���O
        bool m_Flag = true;                          // �t���O
        bool m_startFlag = false;                    // �^�C�g���̃t���O
        int m_PushState = 0;                         // �{�^���̉������
        float m_updateTime;                           //�@�v������
        // �r���[�̍쐬
        void CreateViewLight();

        // �v���C���[�̐���
        void CreatePlayer();

        // �Œ�{�b�N�X�̍쐬
        void CreateFixedBox();

        // �M�~�b�N�̍쐬
        void CreateGimmick();

        // �G�̍쐬
        void CreateEnemy();

        // �X�v���C�g�̍쐬
        void CreateSprite();

        // �S�[������
        void GoalJudge();

        // �Q�[���I�[�o�[����
        void GameOverJudge();

        // �{�^���̉�����Ԃ�ݒ肷��
        void SetPushState(const int PushState);

        // �{�^���̉�����Ԃ��擾����
        int GetPushState();

        // �Q�[���X�e�[�W��ݒ肷��
        void SetGameStageSelect(const wstring& m_csvFail);

        // �X�v���C�g���ړ�����
        void MoveSprite(const shared_ptr<GameObject> nextStageUI, const shared_ptr<GameObject> SelectStage);

        // �I����ʂ̃X�v���C�g���ړ�����
        void SelectMoveSprite(const shared_ptr<GameObject> nextStageUI, const shared_ptr<GameObject> SelectStage);

        // �Q�[���X�e�[�W�̑I�����擾����
        wstring GetGameStageSelect();

        // ���݂̃Q�[���X�e�[�^�X���擾����
        int GetNowGameStatus();

        // ���݂̃Q�[���X�e�[�^�X��ݒ肷��
        void SetNowGameStatus(int afterGameStatus);

        // ���݂̃J�����X�e�[�^�X���擾����
        int GetNowCameraStatus();

        // ���C���J�����ɐ؂�ւ���
        void ToMainCamera();

        // �I�[�v�j���O�J�����ɐ؂�ւ���
        void ToOpeningCamera();

        // �T�E���h�G�t�F�N�g���Đ�����
        void PlaySE(wstring path, float loopcnt, float volume);

        void UImake();

        // �\�z�Ɣj��
        StageManager(const shared_ptr<Stage>& stage) :
            GameObject(stage)
        {
        }

        virtual ~StageManager() {}

        // ������
        virtual void OnCreate() override;

        // �X�V����
        virtual void OnUpdate() override;

        // �`�揈��
        virtual void OnDraw() override;

        // �G�t�F�N�g�C���^�[�t�F�C�X���擾����
        shared_ptr<EfkInterface> GetEfkInterface() const {
            return m_EfkInterface;
        }
    };
}