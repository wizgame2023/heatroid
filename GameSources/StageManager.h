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
		CsvFile m_GameStage;
		shared_ptr<Transform> m_PlayerObject;
		shared_ptr<GameObject> m_SpriteDraw;
		shared_ptr<GameObject> m_TextDraw;
		bool m_Goaltrue = false;
		bool m_Diedtrue = false;
		bool m_Flag = true;
		void CreateViewLight(); //�r���[�̍쐬
		void CreatePlayer(Vec3& pos , Vec3& rot, Vec3& scale); //Player�̐���
		void CreateFixedBox();
		void CreateGimmick();
		void CreateEnemy();
		void CreateSprite();
		void GoalJudge();
		void GameOverJudge();
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