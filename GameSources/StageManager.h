/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class StageManager : public  GameObject
	{
	public:
		enum CameraSelect {
			openingCamera,
			myCamera,
		};

		enum GameStatus {
			// �^�C�g���V�[���\����
			TITLE,
			// �X�e�[�W�Z���N�g�V�[���\����
			SELECT,
			// �Q�[�����J�n���Ă���I������܂�
			GAME_PLAYING,

			TEST_PLAY
		};
		//�G�t�F�N�g�̃C���^�[�t�F�C�X
		shared_ptr<EfkInterface> m_EfkInterface;

		CameraSelect m_CameraSelect;
		shared_ptr<SingleView> m_OpeningCameraView;
		shared_ptr<SingleView> m_MyCameraView;
		int m_nowGameStatus;
		wstring m_StageName;
		CsvFile m_GameStage;
		//�X�v���C�g
		shared_ptr<Transform> m_PlayerObject;
		shared_ptr<GameObject> m_SpriteDraw;
		shared_ptr<GameObject> m_TextDraw;
		shared_ptr<GameObject> m_StageUI;
		shared_ptr<GameObject> m_nextStageUI;
		shared_ptr<GameObject> m_clearSelectStage;
		shared_ptr<GameObject> m_retryStageUI;
		shared_ptr<GameObject> m_overSelectStage;
		shared_ptr<GameObject> m_SelectCharge;
		shared_ptr<GameObject> m_TitleCharge;
		shared_ptr<GameObject> m_BGfade;

		float m_totalTime = 0.0f;
		int m_select = 0;

		bool m_Goaltrue = false;
		bool m_Diedtrue = false;
		bool m_pause = false;
		bool m_Flag = true;
		int m_PushState = 0;

		void CreateViewLight(); //�r���[�̍쐬
		void CreatePlayer(); //Player�̐���
		void CreateFixedBox();
		void CreateGimmick();
		void CreateEnemy();
		void CreateSprite();
		void GoalJudge();
		void GameOverJudge();
		void SetPushState(const int PushState);
		int GetPushState();
		void SetGameStageSelect(const wstring& m_csvFail);
		void MoveSprite(const shared_ptr<GameObject> nextStageUI, const shared_ptr<GameObject> SelectStage);
		void SelectMoveSprite(const shared_ptr<GameObject> nextStageUI, const shared_ptr<GameObject> SelectStage);
		wstring GetGameStageSelect();
		int GetNowGameStatus();
		void SetNowGameStatus(int afterGameStatus);
		int GetNowCameraStatus();
		void ToMainCamera();
		void ToOpeningCamera();

		void PlaySE(wstring path, float loopcnt, float volume);
		
		//�\�z�Ɣj��
		StageManager(const shared_ptr<Stage>& stage) :
			GameObject(stage)
		{
		}
		virtual ~StageManager() {}

		virtual void OnCreate()override; //������
		virtual void OnUpdate()override; //������
		virtual void OnDraw()override; //������

		shared_ptr<EfkInterface> GetEfkInterface() const {
			return m_EfkInterface;
		}
	};
}