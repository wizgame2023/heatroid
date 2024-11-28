/*!
@file MainCamera.cpp
@brief �J�����Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	CameraCollision::CameraCollision(const shared_ptr<Stage>& StagePtr)
		:GameObject(StagePtr),
		GetPos(Vec3(0.0f, 1.0f, 0.0f)), 
		m_TargetToAt(0, 0, 0),
		TargetPos(Vec3(0.0f, 0.0f, 0.0f)),
		m_ArmLen(15.5f),
		m_RadY(.5f),
		m_RadXZ(1.5f),
		m_RotSpeed(-2.0f),
		m_ToTargetLerp(1.0f),
		m_Hit(false),
		m_LRBaseMode(true)
	{}

	void CameraCollision::OnCreate() {
		auto ptrCamera = dynamic_pointer_cast<MainCamera>(OnGetDrawCamera());
		auto pos = ptrCamera->GetEye();
		auto ptrtrans = GetComponent<Transform>();
		ptrtrans->SetScale(Vec3(0.1f, 0.1f, 0.1f));
		ptrtrans->SetPosition(pos);
		TargetPos = Vec3(0.0f, 0.0f, 0.0f);
		GetPos = Vec3(0.0f, 1.0f, 0.0f);
		//�����蔻��
		auto m_camera = AddComponent<CollisionSphere>();
		ptrCamera->SetCameraObject(GetThis<GameObject>());
		m_camera->SetAfterCollision(AfterCollision::Auto);
	}

	void CameraCollision::OnUpdate() {
		auto scene = App::GetApp()->GetScene<Scene>();

		wstringstream wss;
		wss << GetPos.x << " : " << GetPos.y << " : " << GetPos.z << " : " << endl;
		scene->SetDebugString(L"Camera\n" + wss.str());

		auto Ptr = GetComponent<Transform>();
		auto ptrCamera = dynamic_pointer_cast<MainCamera>(OnGetDrawCamera());
		auto ptrTarget = ptrCamera->GetTargetObject();

		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keyData = App::GetApp()->GetInputDevice().GetKeyState();
		//�O��̃^�[������̎���
		float elapsedTime = App::GetApp()->GetElapsedTime();
		Vec3 newEye = ptrCamera->GetEye();
		Vec3 newAt = ptrCamera->GetAt();
		//�v�Z�Ɏg�����߂̘r�p�x�i�x�N�g���j
		bsm::Vec3 armVec = newEye - newAt;
		//���K�����Ă���
		armVec.normalize();
		float fThumbRX = 0.0f;
		if (cntlVec[0].bConnected) {
			fThumbRX = cntlVec[0].fThumbRX;
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
			bsm::Vec3(0.0f, 0.0f, 1.0f),
			qtXZ
		);

		Vec3 posXZ = Mat.transInMatrix();
		//XZ�̒l���킩�����̂Řr�p�x�ɑ��
		armVec.x = posXZ.x;
		armVec.z = posXZ.z;
		//�r�p�x�𐳋K��
		armVec.normalize();

		if (ptrTarget) {
			//�ڎw�������ꏊ
			Vec3 toAt = ptrTarget->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
			toAt += m_TargetToAt;
			TargetPos = Lerp::CalculateLerp(newAt, toAt, 0, 1.0f, 1.0, Lerp::Linear);
		}
		////�ڎw�������ꏊ�ɃA�[���̒l�Ƙr�x�N�g����Eye�𒲐�

		UpdateArmLengh();
		Vec3 toEye = newAt + armVec * m_ArmLen;
		GetPos = Lerp::CalculateLerp(newEye, toEye, 0, 1.0f, m_ToTargetLerp, Lerp::Linear);

		//�ǔ��V�X�e��
		GetComponent<Transform>()->SetPosition(GetPos);
		GetComponent<Transform>()->SetRotation(TargetPos);
	}

	void CameraCollision::OnCollisionExcute(const CollisionPair& Pair)
	{
		m_Hit == true;
	}

	void CameraCollision::UpdateArmLengh() {
		auto Ptr = GetComponent<Transform>();
		auto Pos = Ptr->GetPosition();
		auto ptrCamera = dynamic_pointer_cast<MainCamera>(OnGetDrawCamera());
		auto ptrTarget = ptrCamera->GetTargetObject();
		Vec3 toAt = ptrTarget->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();

		Vec3 vec =toAt - Pos;
		m_ArmLen = length(vec);

		auto StageMane = GetStage()->GetSharedGameObject<StageManager>(L"StageManager");
		if (m_ArmLen >= ptrCamera->m_MaxArm) {
			//m_MaxArm�ȏ㗣��Ȃ��悤�ɂ���
			m_ArmLen = ptrCamera->m_MaxArm;
		}
		if (m_ArmLen <= ptrCamera->m_MinArm) {
			//m_MinArm�ȏ㗣��Ȃ��悤�ɂ���
			m_ArmLen = ptrCamera->m_MinArm;
		}
	}

	bool CameraCollision::GetLRBaseMode() const {
		return m_LRBaseMode;

	}
	bool CameraCollision::IsLRBaseMode() const {
		return m_LRBaseMode;

	}

	void CameraCollision::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		
	}

	void CameraCollision::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
		m_Hit = false;
	}

	MainCamera::MainCamera() :
		Camera(),
		m_ToTargetLerp(1.0f),
		m_TargetToAt(0, 0, 0),
		m_RadY(.5f),
		m_RadXZ(0),
		m_CameraUnderRot(0.1f),
		m_ArmLen(3.0f),
		m_MaxArm(20.0f),
		m_MinArm(0.1f),
		m_RotSpeed(-2.0f),
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
		m_ArmLen(3.0f),
		m_MaxArm(20.0f),
		m_MinArm(0.1f),
		m_RotSpeed(-2.0f),
		m_ZoomSpeed(0.1f),
		m_LRBaseMode(true)
	{
		m_ArmLen = ArmLen;
		auto eye = GetEye();
		eye.y = m_ArmLen;
		SetEye(eye);
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
		//�O��̃^�[������̎���
		Vec3 newAt = GetAt();
		auto ptrTarget = GetTargetObject();
		if (ptrTarget) {
			//�ڎw�������ꏊ
			Vec3 toAt = ptrTarget->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
			toAt += m_TargetToAt;
			newAt = Lerp::CalculateLerp(GetAt(), toAt, 0, 1.0f, 1.0, Lerp::Linear);
		}
		SetAt(newAt);
	}

	OpeningCameraman::OpeningCameraman(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& EndPos,
		const Vec3& AtStartPos, const Vec3& AtEndPos, const Vec3& AtPos, float& TotalTime,
		const Vec3& secondEndPos, const Vec3& secondAtEndPos) :
		GameObject(StagePtr),
		m_startPos(StartPos),
		m_endPos(EndPos),
		m_atStartPos(AtStartPos),
		m_atEndPos(AtEndPos),
		m_atPos(AtStartPos),
		m_totalTime(TotalTime),
		m_secondEndPos(secondEndPos),
		m_secondAtEndPos(secondAtEndPos)
	{}
	OpeningCameraman::~OpeningCameraman() {}

	//������
	void OpeningCameraman::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptr = GetComponent<Transform>();
		ptr->SetScale(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(m_startPos);
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<OpeningCameraman>(GetThis<OpeningCameraman>()));
		//�ŏ��̃X�e�[�g��OpeningCameramanToGoalState�ɐݒ�
		m_StateMachine->ChangeState(OpeningCameramanToFirstState::Instance());

		//�㔼�p�̈ꎞ�I�Ȋi�[�ꏊ
		m_tempStartPos = m_startPos;
		m_tempEndPos = m_endPos;
		m_tempAtStartPos = m_atStartPos;
		m_tempAtEndPos = m_atEndPos;
		m_tempAtPos = m_atPos;
		m_tempTotalTime = m_totalTime;
	}

	//����
	void OpeningCameraman::OnUpdate() {
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	void OpeningCameraman::ToGoalEnterBehavior() { //�O����
		m_startPos; //�J�����̍ŏ��̈ʒu
		m_endPos; //�J�����̍Ō�̈ʒu
		m_atStartPos; //�ŏ��Ɍ��Ă���p
		m_atEndPos; //�Ō�Ɍ��Ă���p
		m_atPos;//�J�����Ō�̈ʒu
		m_totalTime;
	}

	void OpeningCameraman::ToStartEnterBehavior() { //�㔼��
		m_startPos = m_tempEndPos; //�J�����̍ŏ��̈ʒu
		m_endPos = m_secondEndPos; //�J�����̍Ō�̈ʒu
		m_atStartPos = m_tempAtEndPos; //�ŏ��Ɍ��Ă���p
		m_atEndPos = m_secondAtEndPos; //�Ō�Ɍ��Ă���p
		m_atPos;//�J�����Ō�̈ʒu
		m_totalTime = m_tempTotalTime;
	}

	bool OpeningCameraman::ExcuteBehavior(float totaltime) {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_totalTime += ElapsedTime;
		if (m_totalTime > totaltime) {
			return true;
		}
		Easing<Vec3> easing;
		m_eyePos = easing.EaseInOut(EasingType::Cubic, m_startPos, m_endPos, m_totalTime, totaltime);
		m_atPos = easing.EaseInOut(EasingType::Cubic, m_atStartPos, m_atEndPos, m_totalTime, totaltime);
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_eyePos);
		return false;
	}
	void OpeningCameraman::EndStateEnterBehavior() {
		auto ptrGameStageManegeer = GetStage()->GetSharedGameObject<StageManager>(L"StageManager");
		ptrGameStageManegeer->ToMainCamera();
	}

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToGoalState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	shared_ptr<OpeningCameramanToFirstState> OpeningCameramanToFirstState::Instance() {
		static shared_ptr<OpeningCameramanToFirstState> instance(new OpeningCameramanToFirstState);
		return instance;
	}
	void OpeningCameramanToFirstState::Enter(const shared_ptr<OpeningCameraman>& Obj) {
		Obj->ToGoalEnterBehavior();
	}
	void OpeningCameramanToFirstState::Execute(const shared_ptr<OpeningCameraman>& Obj) {
		if (Obj->ExcuteBehavior(1.0f)) {
			Obj->GetStateMachine()->ChangeState(OpeningCameramanToSecondState::Instance());
		}
	}
	void OpeningCameramanToFirstState::Exit(const shared_ptr<OpeningCameraman>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToStartState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	shared_ptr<OpeningCameramanToSecondState> OpeningCameramanToSecondState::Instance() {
		static shared_ptr<OpeningCameramanToSecondState> instance(new OpeningCameramanToSecondState);
		return instance;
	}
	void OpeningCameramanToSecondState::Enter(const shared_ptr<OpeningCameraman>& Obj) {
		Obj->ToStartEnterBehavior();
	}
	void OpeningCameramanToSecondState::Execute(const shared_ptr<OpeningCameraman>& Obj) {
		if (Obj->ExcuteBehavior(3.0f)) {
			Obj->GetStateMachine()->ChangeState(OpeningCameramanEndState::Instance());
		}
	}
	void OpeningCameramanToSecondState::Exit(const shared_ptr<OpeningCameraman>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanEndState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	shared_ptr<OpeningCameramanEndState> OpeningCameramanEndState::Instance() {
		static shared_ptr<OpeningCameramanEndState> instance(new OpeningCameramanEndState);
		return instance;
	}
	void OpeningCameramanEndState::Enter(const shared_ptr<OpeningCameraman>& Obj) {
		Obj->EndStateEnterBehavior();
	}
	void OpeningCameramanEndState::Execute(const shared_ptr<OpeningCameraman>& Obj) {
	}
	void OpeningCameramanEndState::Exit(const shared_ptr<OpeningCameraman>& Obj) {
	}



	OpeningCamera::OpeningCamera() :
		Camera()
	{}
	OpeningCamera::~OpeningCamera() {}

	void OpeningCamera::OnCreate()
	{
	}

	void OpeningCamera::OnUpdate() {
		Camera::OnUpdate();

		auto ptrOpeningCameraman = dynamic_pointer_cast<OpeningCameraman>(GetCameraObject());
		if (ptrOpeningCameraman) {
			auto pos = ptrOpeningCameraman->GetAtPos();
			auto eye = ptrOpeningCameraman->GetEyePos();
			SetEye(eye);
			SetAt(pos);
		}
	}
}