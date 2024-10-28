/*!
@file GameStageHemmi.h
@brief �e�X�g�Q�[���X�e�[�W
�S���F�팩
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStageHemmi : public Stage {
		void CreateViewLight(); //�r���[�̍쐬
		void CreateGameBox();//GameStage�̐���
		void CreateEnemy();
		void CreateBreakWall();
	public:
		//�\�z�Ɣj��
		GameStageHemmi() :Stage(){}
		virtual ~GameStageHemmi() {}
		
		virtual void OnCreate()override; //������
	};

}
//end basecross

