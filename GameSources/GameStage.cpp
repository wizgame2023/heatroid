/*!
���c��P
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------

	//�r���[�ƃ��C�g�̍쐬
	void GameStage::CreateViewLight() {
		// �J�����̐ݒ�
		auto camera = ObjectFactory::Create<MainCamera>();
		camera->SetEye(Vec3(-50.0f, 3.00f, 0.0f));
		camera->SetAt(Vec3(0.0f, 0.25, 0.0f));
		//camera->SetCameraObject(cameraObject);
		// �r���[�ɃJ������ݒ�
		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		//�}���`���C�g�̍쐬
		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting(); //�f�t�H���g�̃��C�e�B���O���w��
		auto cameraObject = AddGameObject<CameraCollision>();

	}

	void GameStage::CreateGameBox() {
		//�z��̏�����
		vector< vector<Vec3> > vec = {
			{
				Vec3(0.0f, 0.01f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(50.0f, 0.5f, 50.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto ptrPlayer = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->GetComponent<Transform>()->SetPosition(Vec3(85, 5.0f, 0));
		ptrPlayer->GetComponent<Transform>()->SetRotation(Vec3(0, 180, 0));
		ptrPlayer->GetComponent<Transform>()->SetScale(Vec3(3.0f, 3.0f, 3.0f));
		auto playerPos = ptrPlayer->GetComponent<Transform>();
	
	}

	//�{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {
		//CSV�̍s�P�ʂ̔z��
		vector<wstring> LineVec;
		//0�Ԗڂ̃J������L"FixedBox"�ł���s�𔲂��o��
		m_GameStage1.GetSelect(LineVec, 0, L"Floor");
		for (auto& v : LineVec) {
			//�g�[�N���i�J�����j�̔z��
			vector<wstring> Tokens;
			//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
			Util::WStrToTokenVector(Tokens, v, L',');
			//�e�g�[�N���i�J�����j���X�P�[���A��]�A�ʒu�ɓǂݍ���
			Vec3 Scale(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			Vec3 Rot;
			//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);

			//�e�l����������̂ŃI�u�W�F�N�g�쐬
			auto ptrFloor = AddGameObject<TilingFixedBox>(Pos, Rot, Scale, 1.0f, 1.0f, Tokens[10]);
			ptrFloor->AddTag(L"Floor");
		}
		m_GameStage1.GetSelect(LineVec, 0, L"Wall");
		for (auto& v : LineVec) {
			//�g�[�N���i�J�����j�̔z��
			vector<wstring> Tokens;
			//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
			Util::WStrToTokenVector(Tokens, v, L',');
			//�e�g�[�N���i�J�����j���X�P�[���A��]�A�ʒu�ɓǂݍ���
			Vec3 Scale(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			Vec3 Rot;
			//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);

			//�e�l����������̂ŃI�u�W�F�N�g�쐬
			auto PtrWall = AddGameObject<TilingFixedBox>(Pos, Rot, Scale, Scale.x, Scale.y, Tokens[10]);
			PtrWall->AddTag(L"Wall");
		}
		m_GameStage1.GetSelect(LineVec, 0, L"GoalFloor");
		for (auto& v : LineVec) {
			//�g�[�N���i�J�����j�̔z��
			vector<wstring> Tokens;
			//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
			Util::WStrToTokenVector(Tokens, v, L',');
			//�e�g�[�N���i�J�����j���X�P�[���A��]�A�ʒu�ɓǂݍ���
			Vec3 Scale(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			Vec3 Rot;
			//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);

			//�e�l����������̂ŃI�u�W�F�N�g�쐬
			auto ptrFloor = AddGameObject<TilingFixedBox>(Pos, Rot, Scale, 1.0f, 1.0f, Tokens[10]);
			ptrFloor->AddTag(L"Floor");
			ptrFloor->AddTag(L"Goal");
		}

	}

	void GameStage::CreateGimmick()
	{
		CreateSharedObjectGroup(L"Door");
		CreateSharedObjectGroup(L"Switch");
		vector<wstring> LineVec;
		m_GameStage1.GetSelect(LineVec, 0, L"Door");
		for (auto& v : LineVec) {
			//�g�[�N���i�J�����j�̔z��
			vector<wstring> Tokens;
			//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
			Util::WStrToTokenVector(Tokens, v, L',');
			//�e�g�[�N���i�J�����j���X�P�[���A��]�A�ʒu�ɓǂݍ���
			Vec3 Scale(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			Vec3 Rot;
			//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);

			float Switch = (float)_wtof(Tokens[10].c_str());
			int number = (float)_wtof(Tokens[11].c_str());

			//�e�l����������̂ŃI�u�W�F�N�g�쐬
			auto door = AddGameObject<GimmickDoor>(Pos, Rot, Scale, Scale.x, Scale.y, Switch, number, Tokens[12]);
		}

		m_GameStage1.GetSelect(LineVec, 0, L"Switch");
		for (auto& v : LineVec) {
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Scale(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			Vec3 Rot;
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);

			float Button = (float)_wtof(Tokens[10].c_str());
			int number = (float)_wtof(Tokens[11].c_str());

			AddGameObject<GimmickButton>(Pos, Rot, Scale,  Button, number, Tokens[12]);
		}

	}

	void GameStage::CreateEnemy()
	{			
		CreateSharedObjectGroup(L"Enemy");
		vector<wstring> LineVec;
		m_GameStage1.GetSelect(LineVec, 0, L"Enemy");
		for (auto& v : LineVec) {
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Scale(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			Vec3 Rot;
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);
			auto player = GetSharedGameObject<Player>(L"Player");
			auto enemy = AddGameObject<Enemy>(Pos, Rot, Scale, Enemy::rightMove, Enemy::stay, player);
			auto group = GetSharedObjectGroup(L"Enemy");
			group->IntoGroup(enemy);
		}
	}
	void GameStage::OnCreate() {
		try {
			wstring Datadir;
			App::GetApp()->GetDataDirectory(Datadir);
			//CSV�p�X���擾
			m_GameStage1.SetFileName(Datadir+ L"CSV/" + L"GameStage.csv");
			m_GameStage1.ReadCsv();
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			CreateGameBox();
			CreateGimmick();

			CreatePlayer();
			CreateFixedBox();
			CreateEnemy();
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::OnUpdate()
	{
		//auto group = GetSharedObjectGroup(L"Enemy");
		//auto& vec = group->GetGroupVector();
		//for (auto& object : vec)
		//{
		//	auto Enemybj = object.lock();
		//	auto playerSh = GetSharedGameObject<Player>(L"Player");
		//	auto PlayerPos = playerSh->GetComponent<Transform>()->GetWorldPosition();
		//	if (Enemybj != nullptr)
		//	{
		//		if (length(PlayerPos - Enemybj->GetComponent<Transform>()->GetPosition()) < 50.0f) {
		//			Enemybj->SetUpdateActive(true);
		//		}
		//		else{
		//			Enemybj->SetUpdateActive(false);
		//		}
		//	}
		//}
	}

}
//end basecross
