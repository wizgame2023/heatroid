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
		void CreateViewLight(); //�r���[�̍쐬
		void CreateGameBox();//GameStage�̐���

	public:
		//�\�z�Ɣj��
		GameStageTsuboi() :Stage(){}
		virtual ~GameStageTsuboi() {}
		
		virtual void OnCreate() override; //������
	};

}
//end basecross

