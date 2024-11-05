/*!
@file MainCamera.cpp
@brief �J�����Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	Vec3 CameraObject::m_pos = Vec3(0, 5.0f, 5.0f);

	CameraObject::CameraObject(const shared_ptr<Stage>& stage,
		const Vec3& scale
	) :
		GameObject(stage),
		m_scal(scale)
	{}

	//������
	void CameraObject::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetScale(m_scal);
		//OBB�Փ�j�����t����
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(false);
	}

	void CameraObject::OnUpdate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
	}

	void CameraObject::SetPos(Vec3 pos) {
		pos = m_pos;
	}

	MainCamera::MainCamera() :
		Camera(),
		m_ToTargetLerp(1.0f),
		m_TargetToAt(0, 0, 0),
		m_RadY(.5f),
		m_RadXZ(0),
		m_CameraUnderRot(0.1f),
		m_ArmLen(5.0f),
		m_MaxArm(30.0f),
		m_MinArm(2.0f),
		m_RotSpeed(1.0f),
		m_ZoomSpeed(0.1f),
		m_LRBaseMode(true)
	{}

	MainCamera::MainCamera(float ArmLen) :
		Camera(),
		m_ToTargetLerp(1.0f),
		m_TargetToAt(0, 0, 0),
		m_RadY(.5f),
		m_RadXZ(0),
		m_CameraUnderRot(0.1f),
		m_ArmLen(5.0f),
		m_MaxArm(30.0f),
		m_MinArm(2.0f),
		m_RotSpeed(1.0f),
		m_ZoomSpeed(0.1f),
		m_LRBaseMode(true)
	{
		m_ArmLen = ArmLen;
		auto eye = GetEye();
		eye.y = m_ArmLen;
		//SetEye(eye);
	}

	MainCamera::~MainCamera() {}
	//�A�N�Z�T

	void MainCamera::SetEye(const bsm::Vec3& Eye) {
		Camera::SetEye(Eye);
		UpdateArmLengh();
	}
	void MainCamera::SetEye(float x, float y, float z) {
		Camera::SetEye(x, y, z);
		UpdateArmLengh();
	}


	shared_ptr<GameObject> MainCamera::GetTargetObject() const {
		if (!m_TargetObject.expired()) {
			return m_TargetObject.lock();
		}
		return nullptr;
	}

	void MainCamera::SetTargetObject(const shared_ptr<GameObject>& Obj) {
		m_TargetObject = Obj;
	}

	float MainCamera::GetToTargetLerp() const {
		return m_ToTargetLerp;
	}
	void MainCamera::SetToTargetLerp(float f) {
		m_ToTargetLerp = f;
	}

	float MainCamera::GetArmLengh() const {
		return m_ArmLen;
	}

	void MainCamera::UpdateArmLengh() {
		auto vec = GetEye() - GetAt();
		m_ArmLen = bsm::length(vec);
		if (m_ArmLen >= m_MaxArm) {
			//m_MaxArm�ȏ㗣��Ȃ��悤�ɂ���
			m_ArmLen = m_MaxArm;
		}
		if (m_ArmLen <= m_MinArm) {
			//m_MinArm�ȉ��߂Â��Ȃ��悤�ɂ���
			m_ArmLen = m_MinArm;
		}
	}

	float MainCamera::GetMaxArm() const {
		return m_MaxArm;

	}
	void MainCamera::SetMaxArm(float f) {
		m_MaxArm = f;
	}
	float MainCamera::GetMinArm() const {
		return m_MinArm;
	}
	void MainCamera::SetMinArm(float f) {
		m_MinArm = f;
	}

	float MainCamera::GetRotSpeed() const {
		return m_RotSpeed;

	}
	void MainCamera::SetRotSpeed(float f) {
		m_RotSpeed = abs(f);
	}

	bsm::Vec3 MainCamera::GetTargetToAt() const {
		return m_TargetToAt;

	}
	void MainCamera::SetTargetToAt(const bsm::Vec3& v) {
		m_TargetToAt = v;
	}

	bool MainCamera::GetLRBaseMode() const {
		return m_LRBaseMode;

	}
	bool MainCamera::IsLRBaseMode() const {
		return m_LRBaseMode;

	}

	void MainCamera::SetAt(const bsm::Vec3& At) {
		Camera::SetAt(At);
		Vec3 armVec = GetEye() - GetAt();
		armVec.normalize();
		armVec *= m_ArmLen;
		Vec3 newEye = GetAt() + armVec;
		Camera::SetEye(newEye);
	}
	void MainCamera::SetAt(float x, float y, float z) {
		Camera::SetAt(x, y, z);
		Vec3 armVec = GetEye() - GetAt();
		armVec.normalize();
		armVec *= m_ArmLen;
		Vec3 newEye = GetAt() + armVec;
		Camera::SetEye(newEye);

	}
	void MainCamera::OnUpdate() {
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keyData = App::GetApp()->GetInputDevice().GetKeyState();
		//�O��̃^�[������̎���
		float elapsedTime = App::GetApp()->GetElapsedTime();
		Vec3 newEye = GetEye();
		Vec3 newAt = GetAt();
		//�v�Z�Ɏg�����߂̘r�p�x�i�x�N�g���j
		bsm::Vec3 armVec = newEye - newAt;
		//���K�����Ă���
		armVec.normalize();
		float fThumbRY = 0.0f;
		float fThumbRX = 0.0f;
		WORD wButtons = 0;
		if (cntlVec[0].bConnected) {
			fThumbRY = cntlVec[0].fThumbRY;
			fThumbRX = cntlVec[0].fThumbRX;
			wButtons = cntlVec[0].wButtons;
		}
		armVec.y = sin(m_RadY);
		//������Y����]���쐬
		if (fThumbRX != 0 || keyData.m_bPushKeyTbl[VK_LEFT] || keyData.m_bPushKeyTbl[VK_RIGHT]) {
			//��]�X�s�[�h�𔽉f
			if (fThumbRX != 0) {
				if (IsLRBaseMode()) {
					m_RadXZ += -fThumbRX * elapsedTime * m_RotSpeed;
				}
				else {
					m_RadXZ += fThumbRX * elapsedTime * m_RotSpeed;
				}
			}
			else if (keyData.m_bPushKeyTbl[VK_LEFT]) {
				if (IsLRBaseMode()) {
					m_RadXZ += elapsedTime * m_RotSpeed;
				}
				else {
					m_RadXZ -= elapsedTime * m_RotSpeed;
				}
			}
			else if (keyData.m_bPushKeyTbl[VK_RIGHT]) {
				if (IsLRBaseMode()) {
					m_RadXZ -= elapsedTime * m_RotSpeed;
				}
				else {
					m_RadXZ += elapsedTime * m_RotSpeed;
				}

			}
			if (abs(m_RadXZ) >= XM_2PI) {
				//1�T�������0��]�ɂ���
				m_RadXZ = 0;
			}
		}
		//�N�I�[�^�j�I����Y��]�i�܂�XZ�x�N�g���̒l�j���v�Z
		Quat qtXZ;
		qtXZ.rotation(m_RadXZ, bsm::Vec3(0, 1.0f, 0));
		qtXZ.normalize();
		//�ړ���s�̍s��v�Z���邱�ƂŁAXZ�̒l���Z�o
		Mat4x4 Mat;
		Mat.strTransformation(
			bsm::Vec3(1.0f, 1.0f, 1.0f),
			bsm::Vec3(0.0f, 0.0f, -1.0f),
			qtXZ
		);

		Vec3 posXZ = Mat.transInMatrix();
		//XZ�̒l���킩�����̂Řr�p�x�ɑ��
		armVec.x = posXZ.x;
		armVec.z = posXZ.z;
		//�r�p�x�𐳋K��
		armVec.normalize();

		auto ptrTarget = GetTargetObject();
		if (ptrTarget) {
			//�ڎw�������ꏊ
			Vec3 toAt = ptrTarget->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
			toAt += m_TargetToAt;
			newAt = Lerp::CalculateLerp(GetAt(), toAt, 0, 1.0f, 1.0, Lerp::Linear);
		}
		////�ڎw�������ꏊ�ɃA�[���̒l�Ƙr�x�N�g����Eye�𒲐�
		Vec3 toEye = newAt + armVec * m_ArmLen;
		newEye = Lerp::CalculateLerp(GetEye(), toEye, 0, 1.0f, m_ToTargetLerp, Lerp::Linear);

		SetAt(newAt);
		SetEye(newEye);
		//CameraObject::SetPos(newEye);
		UpdateArmLengh();
		Camera::OnUpdate();
	}


}