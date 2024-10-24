/*!
@file GameStageTsuboi.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------

	//�r���[�ƃ��C�g�̍쐬
	void GameStageTsuboi::CreateViewLight() {
		// �J�����̐ݒ�
		auto camera = ObjectFactory::Create<Camera>();
		camera->SetEye(Vec3(0.0f, .2f, 3.0f));
		camera->SetAt(Vec3(0.0f, .2f, 0.0f));

		// �r���[�ɃJ������ݒ�
		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		//�}���`���C�g�̍쐬
		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting(); //�f�t�H���g�̃��C�e�B���O���w��
	}

	void GameStageTsuboi::CreateGameBox() {
		//�z��̏�����
		vector< vector<Vec3> > vec = {
			{
				Vec3(3.0f, .1f, .3f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -.5f, 0.0f)
			},
			{
				Vec3(1.0f, .1f, .3f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(.8f, 0.0f, 0.0f)
			},
			{
				Vec3(.1f, 1.0f, .3f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-.80f, 0.0f, 0.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}


	void GameStageTsuboi::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			auto ptrPlayer = AddGameObject<Player>();
			SetSharedGameObject(L"Player", ptrPlayer);

			CreateGameBox();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
