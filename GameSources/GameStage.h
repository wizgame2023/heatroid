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
		vector<shared_ptr<Transform>> m_PlayerObject;
		void CreateViewLight(); //�r���[�̍쐬
		void CreateGameBox();//GameStage�̐���
		void CreatePlayer(); //Player�̐���
		void CreateFixedBox();
		void CreateGimmick();
		void CreateEnemy();
		void InstanceStage();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage(){}
		virtual ~GameStage() {}
		
		virtual void OnCreate()override; //������
		virtual void OnUpdate()override; //������
		void GetRay(Vec3& Near, Vec3& Far);

	};

}
//end basecross

