/*!
@file MainCamera.cpp
@brief カメラなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	CameraCollision::CameraCollision(const shared_ptr<Stage>& StagePtr)
		:GameObject(StagePtr),
		GetPos(Vec3(0.0f, 1.0f, 0.0f)), 
		m_TargetToAt(0, 0, 0),
		TargetPos(Vec3(0.0f, 0.0f, 0.0f)),
		m_ArmLen(20.5f),
		m_RadY(.5f),
		m_RadXZ(0),
		m_RotSpeed(-2.0f),
		m_ToTargetLerp(1.0f),
		m_Hit(false),
		m_LRBaseMode(true)
	{}

	void CameraCollision::OnCreate() {
		auto ptrCamera = dynamic_pointer_cast<MainCamera>(OnGetDrawCamera());
		auto pos = ptrCamera->GetEye();
		auto rot = ptrCamera->GetAt();
		auto ptrtrans = GetComponent<Transform>();
		ptrtrans->SetScale(Vec3(0.1, 0.1, 0.1));
		ptrtrans->SetPosition(pos);
		TargetPos = Vec3(0.0f, 0.0f, 0.0f);
		GetPos = Vec3(0.0f, 1.0f, 0.0f);
		//当たり判定
		auto m_camera = AddComponent<CollisionSphere>();
		ptrCamera->SetCameraObject(GetThis<GameObject>());
		m_camera->SetDrawActive(true);
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");

	}

	void CameraCollision::OnUpdate() {
		auto scene = App::GetApp()->GetScene<Scene>();

		wstringstream wss;
		wss << GetPos.x << " : " << GetPos.y << " : " << GetPos.z << " : " << endl;
		scene->SetDebugString(L"Camera\n" + wss.str());

		auto Ptr = GetComponent<Transform>();
		auto ptrCamera = dynamic_pointer_cast<MainCamera>(OnGetDrawCamera());
		auto pos = ptrCamera->GetEye();
		auto rot = ptrCamera->GetAt();

		//Vec3 Near, Far;
		//GetTypeStage<GameStage>()->GetRay(Near, Far);
		auto ptrTarget = ptrCamera->GetTargetObject();
		//auto PsPos = ptrTarget->GetComponent<Transform>()->GetPosition();
		//float t;
		//Vec3 RayPos;
		////現在位置と一番近いレイ上の点を得る
		//HitTest::ClosetPtPointSegment(PsPos, Near, Far, t, RayPos);
		//GetComponent<Transform>()->SetPosition(RayPos);

		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keyData = App::GetApp()->GetInputDevice().GetKeyState();
		//前回のターンからの時間
		float elapsedTime = App::GetApp()->GetElapsedTime();
		Vec3 newEye = ptrCamera->GetEye();
		Vec3 newAt = ptrCamera->GetAt();
		//計算に使うための腕角度（ベクトル）
		bsm::Vec3 armVec = newEye - newAt;
		//正規化しておく
		armVec.normalize();
		float fThumbRX = 0.0f;
		if (cntlVec[0].bConnected) {
			fThumbRX = cntlVec[0].fThumbRX;
		}
		armVec.y = sin(m_RadY);
		//ここでY軸回転を作成
		if (fThumbRX != 0 || keyData.m_bPushKeyTbl[VK_LEFT] || keyData.m_bPushKeyTbl[VK_RIGHT]) {
			//回転スピードを反映
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
				//1週回ったら0回転にする
				m_RadXZ = 0;
			}
		}

		//クオータニオンでY回転（つまりXZベクトルの値）を計算
		Quat qtXZ;
		qtXZ.rotation(m_RadXZ, bsm::Vec3(0, 1.0f, 0));
		qtXZ.normalize();
		//移動先行の行列計算することで、XZの値を算出
		Mat4x4 Mat;
		Mat.strTransformation(
			bsm::Vec3(1.0f, 1.0f, 1.0f),
			bsm::Vec3(0.0f, 0.0f, -1.0f),
			qtXZ
		);

		Vec3 posXZ = Mat.transInMatrix();
		//XZの値がわかったので腕角度に代入
		armVec.x = posXZ.x;
		armVec.z = posXZ.z;
		//腕角度を正規化
		armVec.normalize();

		if (ptrTarget) {
			//目指したい場所
			Vec3 toAt = ptrTarget->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
			toAt += m_TargetToAt;
			TargetPos = Lerp::CalculateLerp(rot, toAt, 0, 1.0f, 1.0, Lerp::Linear);
		}

		////目指したい場所にアームの値と腕ベクトルでEyeを調整
		Vec3 toEye = newAt + armVec * m_ArmLen;
		GetPos = Lerp::CalculateLerp(pos, toEye, 0, 1.0f, m_ToTargetLerp, Lerp::Linear);

		//追尾システム
		GetComponent<Transform>()->SetPosition(GetPos);
		//ptrCamera->SetEye(GetPos);
		//GetComponent<Transform>()->SetRotation(TargetPos);
		UpdateArmLengh();
	}

	void CameraCollision::UpdateArmLengh() {
		auto Ptr = GetComponent<Transform>();
		auto ptrCamera = dynamic_pointer_cast<MainCamera>(OnGetDrawCamera());
		auto Pos = Ptr->GetPosition();
		auto ptrTarget = ptrCamera->GetTargetObject();
		auto Rot = ptrTarget->GetComponent<Transform>()->GetPosition();
		Vec3 vec = Pos - Rot;
		m_ArmLen = bsm::length(vec);
		if (m_ArmLen >= ptrCamera->m_MaxArm) {
			//m_MaxArm以上離れないようにする
			m_ArmLen = ptrCamera->m_MaxArm;
		}
		if (m_ArmLen <= ptrCamera->m_MinArm) {
			//m_MaxArm以上離れないようにする
			m_ArmLen = ptrCamera->m_MinArm;
		}
	}

	bool CameraCollision::GetLRBaseMode() const {
		return m_LRBaseMode;

	}
	bool CameraCollision::IsLRBaseMode() const {
		return m_LRBaseMode;

	}
	void CameraCollision::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		m_Hit = true;
	}
	void CameraCollision::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		m_Hit = true;
		
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
		m_MaxArm(25.0f),
		m_MinArm(0.5f),
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
		m_MaxArm(25.0f),
		m_MinArm(0.5f),
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
	//アクセサ

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
			//m_MaxArm以上離れないようにする
			m_ArmLen = m_MaxArm;
		}
		if (m_ArmLen <= m_MinArm) {
			//m_MinArm以下近づかないようにする
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
		//前回のターンからの時間
		float elapsedTime = App::GetApp()->GetElapsedTime();
		Vec3 newEye = GetEye();
		Vec3 newAt = GetAt();
		//計算に使うための腕角度（ベクトル）
		//bsm::Vec3 armVec = newEye - newAt;
		////正規化しておく
		//armVec.normalize();
		//float fThumbRX = 0.0f;
		//if (cntlVec[0].bConnected) {
		//	fThumbRX = cntlVec[0].fThumbRX;
		//}
		//armVec.y = sin(m_RadY);
		////ここでY軸回転を作成
		//if (fThumbRX != 0 || keyData.m_bPushKeyTbl[VK_LEFT] || keyData.m_bPushKeyTbl[VK_RIGHT]) {
		//	//回転スピードを反映
		//	if (fThumbRX != 0) {
		//		if (IsLRBaseMode()) {
		//			m_RadXZ += -fThumbRX * elapsedTime * m_RotSpeed;
		//		}
		//		else {
		//			m_RadXZ += fThumbRX * elapsedTime * m_RotSpeed;
		//		}
		//	}
		//	else if (keyData.m_bPushKeyTbl[VK_LEFT]) {
		//		if (IsLRBaseMode()) {
		//			m_RadXZ += elapsedTime * m_RotSpeed;
		//		}
		//		else {
		//			m_RadXZ -= elapsedTime * m_RotSpeed;
		//		}
		//	}
		//	else if (keyData.m_bPushKeyTbl[VK_RIGHT]) {
		//		if (IsLRBaseMode()) {
		//			m_RadXZ -= elapsedTime * m_RotSpeed;
		//		}
		//		else {
		//			m_RadXZ += elapsedTime * m_RotSpeed;
		//		}

		//	}
		//	if (abs(m_RadXZ) >= XM_2PI) {
		//		//1週回ったら0回転にする
		//		m_RadXZ = 0;
		//	}
		//}
		////クオータニオンでY回転（つまりXZベクトルの値）を計算
		//Quat qtXZ;
		//qtXZ.rotation(m_RadXZ, bsm::Vec3(0, 1.0f, 0));
		//qtXZ.normalize();
		////移動先行の行列計算することで、XZの値を算出
		//Mat4x4 Mat;
		//Mat.strTransformation(
		//	bsm::Vec3(1.0f, 1.0f, 1.0f),
		//	bsm::Vec3(0.0f, 0.0f, -1.0f),
		//	qtXZ
		//);

		//Vec3 posXZ = Mat.transInMatrix();
		////XZの値がわかったので腕角度に代入
		//armVec.x = posXZ.x;
		//armVec.z = posXZ.z;
		////腕角度を正規化
		//armVec.normalize();

		auto ptrTarget = GetTargetObject();
		if (ptrTarget) {
			//目指したい場所
			Vec3 toAt = ptrTarget->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
			toAt += m_TargetToAt;
			newAt = Lerp::CalculateLerp(GetAt(), toAt, 0, 1.0f, 1.0, Lerp::Linear);
		}
		//////目指したい場所にアームの値と腕ベクトルでEyeを調整
		//Vec3 toEye = newAt + armVec * m_ArmLen;
		//newEye = Lerp::CalculateLerp(GetEye(), toEye, 0, 1.0f, m_ToTargetLerp, Lerp::Linear);

		SetAt(newAt);
		//SetEye(newEye);
		////CameraObject::SetPos(newEye);
		//UpdateArmLengh();
		//Camera::OnUpdate();
	}
}