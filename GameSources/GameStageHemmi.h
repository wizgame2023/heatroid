/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
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

	public:
		//�\�z�Ɣj��
		GameStageHemmi() :Stage(){}
		virtual ~GameStageHemmi() {}
		
		virtual void OnCreate()override; //������
	};

}
//end basecross

