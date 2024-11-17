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
		CsvFile m_GameStage1;
		shared_ptr<Transform> m_PlayerObject;
		shared_ptr<GameObject> m_SpriteDraw;
		shared_ptr<GameObject> m_TextDraw;
		bool m_Goaltrue;
		void CreateViewLight(); //�r���[�̍쐬
		void CreateGameBox();//GameStage�̐���
		void CreatePlayer(); //Player�̐���
		void CreateFixedBox();
		void CreateGimmick();
		void CreateEnemy();
		void CreateSprite();
		void GoalJudge();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage(){}
		virtual ~GameStage() {}
		
		virtual void OnCreate()override; //������
		virtual void OnUpdate()override; //������

	};

}
//end basecross

