/*!
@file GameStageTsuboi.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStageTsuboi : public Stage {
		CsvFile m_GameStage1;
		void CreateViewLight(); //�r���[�̍쐬
		void CreateGameBox();//GameStage�̐���
		void CreatePlayer();
		void CreateEnemy();

	public:
		//�\�z�Ɣj��
		GameStageTsuboi() :Stage(){}
		virtual ~GameStageTsuboi() {}
		
		virtual void OnCreate() override; //������
	};

}
//end basecross

