/*!
@file Gimmick.cpp
@brief Gimmick�Ȃ�
�S���F���c
*/

#include "stdafx.h"
#include "Project.h"
#include "FixedBox.h"
namespace basecross {

	bool GimmickButton::m_open = false;

	GimmickButton::GimmickButton(const shared_ptr<Stage>& StagePtr,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		float UPic,
		float VPic
	) :
		GameObject(StagePtr),
		m_position(position),
		m_rotation(rotation),
		m_scale(scale),
		m_UPic(UPic),
		m_VPic(VPic)
	{	}
	GimmickButton::~GimmickButton() {}
	//������
	void GimmickButton::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_scale);
		PtrTrans->SetRotation(m_rotation);
		PtrTrans->SetPosition(m_position);
		auto Coll = AddComponent<CollisionObb>();
		AddTag(L"GimmickButton");
		Coll->SetFixed(true);
		//�`��R���|�[�l���g�̒ǉ�
		auto PtrDraw = AddComponent<PNTStaticInstanceDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		wstring Datadir;
		App::GetApp()->GetDataDirectory(Datadir);

		CsvFile m_GameStage1;
		vector<wstring> LineVec;
		m_GameStage1.GetSelect(LineVec, 0);
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 160; j++)
			{
				switch (LineVec[i][j])
				{
				case 4:
					float x = -25.0f + j + 25.0f;
					float z = +25.0f + (-i) - 25.0f;
					// �C���X�^���X�p�̍s����쐬����
					Mat4x4 matrix;
					matrix.translation(Vec3(x, 0.0f, z));

					PtrDraw->AddMatrix(matrix); // �u���b�N��\���������������s���ǉ����܂��B���̍s�񂪎����ʒu�E�����E�傫���Ŏw�肵�����b�V�����`�悳��܂�


					break;
				}
			}
		}

	}

	void GimmickButton::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if ((Other->FindTag(L"Player")))
		{
			m_open = true;
		}
	}

	void GimmickButton::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
		//if ((Other->FindTag(L"Player")) || Other->FindTag(L"Enemy"))
		//{
		//	m_open = false;
		//}
	}

	GimmickDoor::GimmickDoor(const shared_ptr<Stage>& StagePtr,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale,
		float UPic,
		float VPic
	) :
		GameObject(StagePtr),
		m_position(position),
		m_rotation(rotation),
		m_scale(scale),
		m_UPic(UPic),
		m_VPic(VPic)
	{
	}
	GimmickDoor::~GimmickDoor() {}

	//������
	void GimmickDoor::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_scale);
		PtrTrans->SetRotation(m_rotation);
		PtrTrans->SetPosition(m_position);
		auto Coll = AddComponent<CollisionObb>();
		AddTag(L"GimmickDoor");
		Coll->SetFixed(true);
		//�`��R���|�[�l���g�̒ǉ�
		auto PtrDraw = AddComponent<PNTStaticInstanceDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		wstring Datadir;
		App::GetApp()->GetDataDirectory(Datadir);

		CsvFile m_GameStage1;
		vector<wstring> LineVec;
		m_GameStage1.GetSelect(LineVec, 0);
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 160; j++)
			{
				switch (LineVec[i][j])
				{
				case 3:
					float x = -25.0f + j + 25.0f;
					float z = +25.0f + (-i) - 25.0f;
					// �C���X�^���X�p�̍s����쐬����
					Mat4x4 matrix;
					matrix.translation(Vec3(x, 0.0f, z));

					PtrDraw->AddMatrix(matrix); // �u���b�N��\���������������s���ǉ����܂��B���̍s�񂪎����ʒu�E�����E�傫���Ŏw�肵�����b�V�����`�悳��܂�


					break;
				}
			}
		}

	}

	void GimmickDoor::OnUpdate()
	{
		m_open = GimmickButton::GetButton();
		auto Coll = GetComponent<CollisionObb>();
		Coll->SetSleepActive(true);
		auto ptrTransform = GetComponent<Transform>();
		Vec3 pos = ptrTransform->GetPosition();

		if (m_open)
		{
			for (int i = 0; i < 1; i++)
			{
				ptrTransform->SetPosition(Vec3(pos.x, pos.y += 0.01f, pos.z));
			}
		}
		else
		{
			ptrTransform->SetPosition(m_position);
		}
	}

}