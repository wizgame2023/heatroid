/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Speed(10.0f),
		m_jumpHeight(20.0f),
		m_stat(stand)
	{}
	
	Vec2 Player::GetInputState() const {
		Vec2 ret;
		//�R���g���[���̎擾
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		ret.x = 0.0f;
		ret.y = 0.0f;
		WORD wButtons = 0;
		if (cntlVec[0].bConnected) {
			ret.x = cntlVec[0].fThumbLX;
			ret.y = cntlVec[0].fThumbLY;
		}
		//�L�[�{�[�h�̎擾(�L�[�{�[�h�D��)
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPushKeyTbl['W']) ret.y = 1.0f;
		if (KeyState.m_bPushKeyTbl['A']) ret.x = -1.0f;
		if (KeyState.m_bPushKeyTbl['S']) ret.y = -1.0f;
		if (KeyState.m_bPushKeyTbl['D']) ret.x = 1.0f;
		return ret;
	}


	Vec3 Player::GetMoveVector() const {
		Vec3 angle(0, 0, 0);
		//���͂̎擾
		float moveX = GetInputState().x;
		float moveZ = GetInputState().y;

		if (moveX * moveZ != 0) {
			float moveLength = 0;	//���������̃X�s�[�h
			auto ptrTransform = GetComponent<Transform>();
			auto ptrCamera = OnGetDrawCamera();
			//�i�s�����̌������v�Z
			auto front = ptrTransform->GetPosition() - ptrCamera->GetEye();
			front.y = 0;
			front.normalize();
			//�i�s������������̊p�x���Z�o
			float frontAngle = atan2(front.z, front.x);
			//�R���g���[���̌����v�Z
			Vec2 moveVec(moveX, moveZ);
			float moveSize = moveVec.length();
			//�R���g���[���̌�������p�x���v�Z
			float cntlAngle = atan2(-moveX, moveZ);
			//�g�[�^���̊p�x���Z�o
			float totalAngle = frontAngle + cntlAngle;
			//�p�x����x�N�g�����쐬
			angle = Vec3(cos(totalAngle), 0, sin(totalAngle));
			//���K������
			angle.normalize();
			//�ړ��T�C�Y��ݒ�B
			angle *= moveSize;
			//Y���͕ω������Ȃ�
			angle.y = 0;
		}
		return angle;
	}

	void Player::MovePlayer() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		auto angle = GetMoveVector();
		if (angle.length() > 0.0f) {
			auto pos = GetComponent<Transform>()->GetPosition();
			pos += angle * elapsedTime * m_Speed;
			GetComponent<Transform>()->SetPosition(pos.x, pos.y, 0);
		}
		//��]�̌v�Z
		if (angle.length() > 0.0f) {
			auto utilPtr = GetBehavior<UtilBehavior>();
			utilPtr->RotToHead(angle, 1.0f);
		}
	}

	void Player::OnCreate() {

		//�����ʒu�Ȃǂ̐ݒ�
		auto ptr = AddComponent<Transform>();
		ptr->SetScale(1.00f, 1.00f, 1.00f);	//���a25�Z���`�̋���
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(Vec3(0, 0.125f, 0));

		//CollisionSphere�Փ˔����t����
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(true);

		//�e�p�t�H�[�}���X�𓾂�
		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

		//�d�͂�����
		auto ptrGra = AddComponent<Gravity>();

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto shadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		shadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//�`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetFogEnabled(true);
	}

	void Player::OnUpdate() {
		//�R���g���[���`�F�b�N���ē��͂�����΃R�}���h�Ăяo��
		m_InputHandler.PushHandle(GetThis<Player>());
		auto grav = GetComponent<Gravity>();
		if (m_stat == stand)
		{
			grav->SetGravity(Vec3(0, -30.0f, 0));
		}

		MovePlayer();
	}

	void Player::OnPushA()
	{
		auto grav = GetComponent<Gravity>();
		if (m_stat == stand) {
			grav->StartJump(Vec3(0, m_jumpHeight, 0)); m_stat = air;
		}
	}

	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FixedBox")) {
			m_stat = stand;
		}
	}

	void LandingCollider::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptr = AddComponent<Transform>();
		ptr->SetScale(.25f, .25f, .25f);	//���a25�Z���`�̋���
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(Vec3(0, 0.125f, 0));

		//CollisionSphere�Փ˔����t����
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(true);

	}


}
//end basecross

