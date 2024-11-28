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
		};
		CameraSelect m_CameraSelect;
		shared_ptr<SingleView> m_OpeningCameraView;
		shared_ptr<SingleView> m_MyCameraView;
		int m_nowGameStatus;
		wstring m_StageName;
		CsvFile m_GameStage;
		shared_ptr<Transform> m_PlayerObject;
		shared_ptr<GameObject> m_SpriteDraw;
		shared_ptr<GameObject> m_TextDraw;
		bool m_Goaltrue = false;
		bool m_Diedtrue = false;
		bool m_Flag = true;
		void CreateViewLight(); //�r���[�̍쐬
		void CreatePlayer(); //Player�̐���
		void CreateFixedBox();
		void CreateGimmick();
		void CreateEnemy();
		void CreateSprite();
		void GoalJudge();
		void GameOverJudge();
		void SetGameStageSelect(const wstring& m_csvFail);
		wstring GetGameStageSelect();
		int GetNowGameStatus();
		void SetNowGameStatus(int afterGameStatus);
		int GetNowCameraStatus();
		void SetNowCameraStatus(int afterGameStatus);
		void ToMainCamera();
		void ToOpeningCamera();
		
		//�\�z�Ɣj��
		StageManager(const shared_ptr<Stage>& stage) :
			GameObject(stage)
		{
		}
		virtual ~StageManager() {}

		virtual void OnCreate()override; //������
		virtual void OnUpdate()override; //������

	};
}