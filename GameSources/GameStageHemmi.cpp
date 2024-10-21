/*!
@file GameStageHemmi.cpp
@brief �e�X�g�Q�[���X�e�[�W����
�S���F�팩
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------

	//�r���[�ƃ��C�g�̍쐬
	void GameStageHemmi::CreateViewLight() {
		// �J�����̐ݒ�
		auto camera = ObjectFactory::Create<Camera>();
		camera->SetEye(Vec3(0.0f, 2.0f, 15.0f));
		camera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		// �r���[�ɃJ������ݒ�
		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		//�}���`���C�g�̍쐬
		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting(); //�f�t�H���g�̃��C�e�B���O���w��
	}

	void GameStageHemmi::CreateGameBox() {
		//�z��̏�����
		vector< vector<Vec3> > vec = {
			{
				Vec3(30.0f, 1.0f, 3.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -4.0f, 0.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}
	void GameStageHemmi::CreateEnemy() {
		vector<vector<Vec3>> vec = {
			{
				Vec3(5.0f,5.0f,0.0f),
				Vec3(0.0f,0.0f,0.0f),
				Vec3(1.0f,1.0f,1.0f)
			}
		};

		for (auto v : vec) {
			AddGameObject<Enemy>(v[0], v[1], v[2]);
		}
	}


	void GameStageHemmi::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			auto player = AddGameObject<Player>();
			CreateGameBox();
			AddGameObject<Enemy>(player);
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
