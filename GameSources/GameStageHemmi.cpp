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
		auto camera = ObjectFactory::Create<MainCamera>();
		camera->SetEye(Vec3(0.0f, 0.5f, 3.0f));//0.0f, 0.5f, 3.0f
		camera->SetAt(Vec3(0.0f, 0.5f, 0.0f)); //0.0f, 0.5f, 0.0f

		// �r���[�ɃJ������ݒ�
		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		//�}���`���C�g�̍쐬
		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting(); //�f�t�H���g�̃��C�e�B���O���w��

		//�摜�̒ǉ�
		auto& app = App::GetApp();
		auto path = app->GetDataDirWString();
		auto texPath = path + L"Textures/";
		wstring strTexture = texPath + L"White.png";
		app->RegisterTexture(L"White", strTexture);

	}

	void GameStageHemmi::CreateGameBox() {
		//�z��̏�����
		vector< vector<Vec3> > vec = {
			{
				Vec3(0.0f, -0.4f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(12.0f, 0.1f, 12.0f)
			},
			//{
			//	Vec3(0.0f,0.5f,0.0f),
			//	Vec3(0.0f,0.0f,0.0f),
			//	Vec3(0.5f,0.1f,0.1f)
			//},

		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : vec) {
			auto box = AddGameObject<FixedBox>(v[0], v[1], v[2]);
			box->GetComponent<BcPNTStaticDraw>()->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 1.0f));
			//SetSharedGameObject(L"box",box);
		}
	}
	void GameStageHemmi::CreateBreakWall() {
		vector<vector<Vec3>> vec = {
			{
				Vec3(-1.3f,-0.2f,0.0f),
				Vec3(0.0f,0.0f,0.0f),
				Vec3(0.1f,0.3f,0.1f)
			},
			//{
			//	Vec3(1.3f,-0.2f,0.0f),
			//	Vec3(0.0f,0.0f,0.0f),
			//	Vec3(0.1f,0.3f,0.1f)
			//},

		};

		for (auto v : vec) {
			AddGameObject<BreakWall>(v[0], v[1], v[2]);
		}
	}
	void GameStageHemmi::CreateEnemy() {
		auto rad = XMConvertToRadians(30.0f);
		vector<vector<Vec3>> vec = {
			{
				Vec3(0.6f*4,0.5f,0.0f),
				Vec3(0.0f,0.0f,0.0f),
				Vec3(0.1f,0.1f,0.1f),
			},
			//{
			//	Vec3(-0.6f,0.2f,0.0f),
			//	Vec3(0.0f,0.0f,0.0f),
			//	Vec3(0.1f,0.1f,0.1f),
			//}

		};

		auto player = GetSharedGameObject<Player>(L"Player");
		for (auto v : vec) {
			auto enemy = AddGameObject<Enemy>(v[0], v[1], v[2], Enemy::rightMove, Enemy::runaway, player);
			AddGameObject<GaugeSquare>(enemy);
		}
	}


	void GameStageHemmi::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			auto player = AddGameObject<Player>();
			SetSharedGameObject(L"Player", player);
			CreateGameBox();
			//auto box = GetSharedGameObject<FixedBox>(L"box");
			//auto enemy = AddGameObject<Enemy>(player,box);
			//enemy->SetState(Enemy::rightMove);
			CreateEnemy();
			CreateBreakWall();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
