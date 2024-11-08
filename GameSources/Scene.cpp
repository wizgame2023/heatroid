
/*!
@file Scene.cpp
@brief �V�[������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	void Scene::CreateResources() {

		auto& app = App::GetApp();

		// media�p�X���擾
		auto path = app->GetDataDirWString();
		// textures�p�X���擾
		auto texPath = path + L"Textures/";
		// Models�p�X
		auto mdlPath = path + L"Models/";

		// �v���C���[
		auto modelMesh = MeshResource::CreateBoneModelMesh(mdlPath, L"HeatRoid.bmf");
		app->RegisterResource(L"PLAYER", modelMesh);

		//�G�̃��f��
		auto enemyAruki = MeshResource::CreateBoneModelMesh(mdlPath, L"ZAKO_ARUKU.bmf");
		app->RegisterResource(L"ENEMYARUKU", enemyAruki);

		// �΁A���e�N�X�`��
		app->RegisterTexture(L"FIRE", texPath + L"fire_kari.png");

	}

	void Scene::OnCreate(){
		try {
			// �w�i�F��ݒ�
			SetClearColor(Col4(0.0f, 0.11328125f, 0.2578125, 1.0f)); // �~�b�h�i�C�g�u���[
			
			//���\�[�X�쐬
			CreateResources();

			//�������g�ɃC�x���g�𑗂�
			//����ɂ��e�X�e�[�W��I�u�W�F�N�g��Create���ɃV�[���ɃA�N�Z�X�ł���
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//�Q�[���X�e�[�W�̐ݒ�
			ResetActiveStage<GameStage>();
		}
		else if (event->m_MsgStr == L"ToGameStageHemmi") {
			//�Q�[���X�e�[�W�̐ݒ�
			ResetActiveStage<GameStageHemmi>();
		}
		else if (event->m_MsgStr == L"ToGameStageTsuboi") {
			//�Q�[���X�e�[�W�̐ݒ�
			ResetActiveStage<GameStageTsuboi>();
		}
	}



}
//end basecross
