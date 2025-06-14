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
		m_ArmLen(15.5f),
		m_RadY(.5f),
		m_RadXZ(XM_PIDIV2),
		m_RotSpeed(-2.0f),
		m_ToTargetLerp(1.0f),
		m_Hit(false),
		m_LRBaseMode(true)
	{
	}

	void CameraCollision::OnCreate() {
		auto ptrCamera = dynamic_pointer_cast<MainCamera>(OnGetDrawCamera());
		auto pos = ptrCamera->GetEye();
		auto ptrtrans = GetComponent<Transform>();
		ptrtrans->SetScale(Vec3(0.1f, 0.1f, 0.1f));
		ptrtrans->SetPosition(pos);
		TargetPos = Vec3(0.0f, 0.0f, 0.0f);
		GetPos = Vec3(0.0f, 1.0f, 0.0f);
		//当たり判定
		auto ptrTarget = ptrCamera->GetTargetObject();
		auto m_camera = AddComponent<CollisionSphere>();
		auto draw = AddComponent<BcPNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		ptrCamera->SetCameraObject(GetThis<GameObject>());
		m_camera->SetAfterCollision(AfterCollision::Auto);
		m_camera->AddExcludeCollisionGameObject(ptrTarget);

	}

	void CameraCollision::OnUpdate() {
		auto scene = App::GetApp()->GetScene<Scene>();

		//wstringstream wss;
		//wss << GetPos.x << " : " << GetPos.y << " : " << GetPos.z << " : " << endl;
		//scene->SetDebugString(L"Camera\n" + wss.str());
		auto Ptr = GetComponent<Transform>();
		auto ptrCamera = dynamic_pointer_cast<MainCamera>(OnGetDrawCamera());
		if (!ptrCamera) {
			return;
		}
		auto ptrTarget = ptrCamera->GetTargetObject();

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
		//キーボード操作の場合
		if (keyData.m_bPushKeyTbl[VK_LEFT] || keyData.m_bPushKeyTbl[VK_RIGHT]) {
			if (keyData.m_bPushKeyTbl[VK_LEFT]) {
				fThumbRX = (IsLRBaseMode() ? -1.0f : 1.0f);
			}
			if (keyData.m_bPushKeyTbl[VK_RIGHT]) {
				fThumbRX = (IsLRBaseMode() ? 1.0f : -1.0f);
			}
		}

		//プレイヤーの向きにカメラを戻す
		float tempFixedRad = FixCameraDirection(ptrTarget, (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_Y || keyData.m_bPressedKeyTbl['C']));
		if (tempFixedRad != 0)
			fThumbRX = tempFixedRad;

		armVec.y = sin(m_RadY);
		//ここでY軸回転を作成
		if (fThumbRX != 0) {
			//回転スピードを反映
			if (IsLRBaseMode()) {
				m_RadXZ += -fThumbRX * elapsedTime * m_RotSpeed;
			}
			else {
				m_RadXZ += fThumbRX * elapsedTime * m_RotSpeed;
			}

			if (m_RadXZ > XM_2PI) {
				//1週回ったら0回転にする
				m_RadXZ -= XM_2PI;
			}
			else if (m_RadXZ < 0) {
				m_RadXZ += XM_2PI;
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
			bsm::Vec3(0.0f, 0.0f, 1.0f),
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
			TargetPos = Lerp::CalculateLerp(newAt, toAt, 0, 1.0f, 1.0, Lerp::Linear);
		}
		////目指したい場所にアームの値と腕ベクトルでEyeを調整

		UpdateArmLengh();
		Vec3 toEye = newAt + armVec * m_ArmLen;
		GetPos = Lerp::CalculateLerp(newEye, toEye, 0, 1.0f, m_ToTargetLerp, Lerp::Linear);
		//if (Afterpos != Vec3(0))
		//{
		//	GetPos = Afterpos;
		//}
		//追尾システム
		GetComponent<Transform>()->SetPosition(GetPos);
		GetComponent<Transform>()->SetRotation(TargetPos);
	}

	void CameraCollision::OnCollisionExcute(const CollisionPair& Pair)
	{
		m_Hit == true;
	}

	float CameraCollision::FixCameraDirection(shared_ptr<GameObject>& Other, bool Input)
	{
		//カメラの移動開始
		if (Input == true) {
			if (m_isMovingToFixedDir == false) {
				//プレイヤーの向きベクトルを取得
				Vec3 fwd = -1 * Other->GetComponent<Transform>()->GetForward();
				float angle = atan2f(-fwd.z, fwd.x) - XM_PIDIV2;
				m_fixedDirY = (angle - m_RadXZ);

				//180°より大きい角度が入っている場合
				if (m_fixedDirY > XM_PI) {
					m_fixedDirY = -XM_2PI + m_fixedDirY;
				}
				if (m_fixedDirY < -XM_PI) {
					m_fixedDirY = XM_2PI + m_fixedDirY;
				}

				m_isMovingToFixedDir = true;
			}
		}

		//回す角度を返す
		if (m_isMovingToFixedDir == true) {
			float delta = App::GetApp()->GetElapsedTime();
			if (abs(m_fixedDirY) > m_fixSpeed * delta * abs(m_RotSpeed)) {
				float sub = m_fixedDirY > 0 ? m_fixSpeed : -m_fixSpeed;
				m_fixedDirY -= sub * delta * abs(m_RotSpeed);
				return sub;
			}
			else {
				m_isMovingToFixedDir = false;
				return m_fixedDirY;
			}
		}
		else {
			return 0;
		}

	}

	void CameraCollision::UpdateArmLengh() {
		auto Ptr = GetComponent<Transform>();
		auto Pos = Ptr->GetPosition();
		auto ptrCamera = dynamic_pointer_cast<MainCamera>(OnGetDrawCamera());
		auto ptrTarget = ptrCamera->GetTargetObject();
		Vec3 toAt = ptrTarget->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
		auto ray = GetStage()->GetSharedGameObject<RayCameraMark>(L"Ray");
		Vec3 Afterpos = ray->GetActivePos();
		if (Afterpos != Vec3(0))
		{
			Vec3 vec2 = toAt - Afterpos;
			m_ArmLen = length(vec2);
		}
		else {
			Vec3 vec = toAt - Pos;
			m_ArmLen = length(vec);
		}

		if (m_ArmLen >= ptrCamera->m_MaxArm) {
			//m_MaxArm以上離れないようにする
			m_ArmLen = ptrCamera->m_MaxArm;
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
	{
	}

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
		//前回のターンからの時間
		Vec3 newAt = GetAt();
		auto ptrTarget = GetTargetObject();
		if (ptrTarget) {
			//目指したい場所
			Vec3 toAt = ptrTarget->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
			toAt += m_TargetToAt;
			newAt = Lerp::CalculateLerp(GetAt(), toAt, 0, 1.0f, 1.0, Lerp::Linear);
		}
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keyData = App::GetApp()->GetInputDevice().GetKeyState();
		if (cntlVec[0].wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			Vec3 Pos = Vec3(0, 100, 0);
			SetEye(Pos);
		}
		SetAt(newAt);
	}

	//--------------------------------------------------------------------------------------
//	class RayMark : public GameObject; //衝突したときの表示されるオブジェクト
//--------------------------------------------------------------------------------------

	RayCameraMark::RayCameraMark(const shared_ptr<Stage>& stage
	) :
		GameObject(stage)
	{
	}
	RayCameraMark::RayCameraMark(const shared_ptr<Stage>& stage,
		const shared_ptr<Player>& player,
		const shared_ptr<Camera>& camera
	) :
		GameObject(stage),
		m_player(player),
		m_camera(camera),
		m_wallCnt(0),
		m_activeFlag(false)
	{
	}

	void RayCameraMark::OnCreate() {
		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"DEFAULT_SPHERE");
		Col4 color(0.0f, 0.0f, 1.0f, 0.4f);
		m_draw->SetDiffuse(color);
		m_draw->SetEmissive(color);
		m_draw->SetDrawActive(false);
		SetAlphaActive(true);

		m_trans = GetComponent<Transform>();
		m_trans->SetScale(Vec3(1.0f));
	}
	void RayCameraMark::OnUpdate() {
		auto stage = GetStage();

		//必要な変数の宣言
		Vec3 pos = Vec3(0.0f);
		Vec3 rayStart;
		Vec3 rayEnd;
		Vec3 enemyCrossPos;
		Vec3 objCrossPos;
		TRIANGLE triangle;
		size_t triangleIndex;

		//プレイヤー
		auto player = m_player.lock();
		if (!player) return;
		rayStart = player->GetComponent<Transform>()->GetPosition(); //レイを飛ばす始点

		//レイを飛ばす
		//敵
		auto camera = m_camera.lock();
		if (!camera) return;
		auto cameraObj = camera->GetCameraObject();
		auto cameraDraw = cameraObj->GetComponent<BcPNTStaticDraw>();
		rayEnd = camera->GetEye();		             //レイを飛ばす終点
		//当たっているかのフラグ
		m_hitEnemyFlag = cameraDraw->HitTestStaticMeshSegmentTriangles
		(rayStart, rayEnd, enemyCrossPos, triangle, triangleIndex);

		//壁
		auto wallGroup = GetStage()->GetSharedObjectGroup(L"Wall");
		auto& wallVec = wallGroup->GetGroupVector();
		for (auto v : wallVec) {
			auto walls = v.lock();
			auto wallDraw = walls->GetComponent<PNTStaticDraw>();
			//すべての壁をチェック、レイがあったていたらtrue
			m_hitWallFlag.push_back(wallDraw->HitTestStaticMeshSegmentTriangles
			(rayStart, rayEnd, objCrossPos, triangle, triangleIndex));
		}
		for (int i = 0; i < m_hitWallFlag.size(); i++) {
			if (!m_hitWallFlag[i]) {
				m_wallCnt++;
			}
		}

		//ドア
		auto doorGroup = GetStage()->GetSharedObjectGroup(L"Door");
		auto& doorVec = doorGroup->GetGroupVector();
		for (auto v : doorVec) {
			auto doors = v.lock();
			auto doorDraw = doors->GetComponent<PNTStaticDraw>();
			m_hitDoorFlag.push_back(doorDraw->HitTestStaticMeshSegmentTriangles
			(rayStart, rayEnd, objCrossPos, triangle, triangleIndex));
		}
		for (int i = 0; i < doorVec.size(); i++) {
			if (!m_hitDoorFlag[i]) {
				m_doorCnt++;
			}
		}


		//壁かドアにあたっているか
		if (m_wallCnt == m_hitWallFlag.size() && m_doorCnt == m_hitDoorFlag.size()) {
			m_activeFlag = true;
			m_wallCnt = 0;
			m_doorCnt = 0;
			m_hitWallFlag.clear();
			m_hitDoorFlag.clear();
		}
		else {
			m_activeFlag = false;
			m_wallCnt = 0;
			m_doorCnt = 0;
			m_hitWallFlag.clear();
			m_hitDoorFlag.clear();
		}

		//デバック用
		//当たっている場所の描画
		if (m_hitEnemyFlag) {
			pos = enemyCrossPos;
			m_pos = objCrossPos;
		}
		m_trans->SetPosition(pos);
	}

	bool RayCameraMark::GetActiveFlag() {
		return m_activeFlag;
	}

	Vec3 RayCameraMark::GetActivePos()
	{
		return m_pos;
	}

	void RayCameraMark::Debug() {
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss(L"");
		wss << L"Wall : "
			<< endl;
		scene->SetDebugString(wss.str());

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
	{
	}
	OpeningCameraman::~OpeningCameraman() {}

	//初期化
	void OpeningCameraman::OnCreate() {
		//初期位置などの設定
		auto ptr = GetComponent<Transform>();
		ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(m_startPos);
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<OpeningCameraman>(GetThis<OpeningCameraman>()));
		//最初のステートをOpeningCameramanToGoalStateに設定
		m_StateMachine->ChangeState(OpeningCameramanToFirstState::Instance());

		//後半用の一時的な格納場所
		m_tempStartPos = m_startPos;
		m_tempEndPos = m_endPos;
		m_tempAtStartPos = m_atStartPos;
		m_tempAtEndPos = m_atEndPos;
		m_tempAtPos = m_atPos;
		m_tempTotalTime = m_totalTime;
	}

	//操作
	void OpeningCameraman::OnUpdate() {
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	void OpeningCameraman::ToGoalEnterBehavior() { //後半部
		m_startPos; //カメラの最初の位置
		m_endPos; //カメラの最後の位置
		m_atStartPos; //最初に見てる方角
		m_atEndPos; //最後に見てる方角
		m_atPos;//カメラ最後の位置
		m_totalTime;
	}

	void OpeningCameraman::ToStartEnterBehavior() { //前半部
		m_startPos = m_tempEndPos; //カメラの最初の位置
		m_endPos = m_secondEndPos; //カメラの最後の位置
		m_atStartPos = m_tempAtEndPos; //最初に見てる方角
		m_atEndPos = m_secondAtEndPos; //最後に見てる方角
		m_atPos;//カメラ最後の位置
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
		auto ptrGameStageManegeer = GetStage()->GetSharedGameObject<StageGenerator>(L"StageManager");
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
		if (Obj->ExcuteBehavior(4.0f)) {
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
		if (Obj->ExcuteBehavior(2.0f)) {
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
	{
	}
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

	//ステージクリア時のカメラ演出
	EndingCameraman::EndingCameraman(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& EndPos,
		const Vec3& AtStartPos, const Vec3& AtEndPos, const float& TotalTime) :
		GameObject(StagePtr),
		m_startPos(StartPos),
		m_endPos(EndPos),
		m_atStartPos(AtStartPos),
		m_atEndPos(AtEndPos),
		m_totalTime(TotalTime)
	{
	}
	EndingCameraman::~EndingCameraman() {}

	//初期化
	void EndingCameraman::OnCreate() {
		//初期位置などの設定
		auto ptr = GetComponent<Transform>();
		ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(m_startPos);
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<EndingCameraman>(GetThis<EndingCameraman>()));
		//ステート設定
		m_StateMachine->ChangeState(EndingCameramanBasicState::Instance());
	}

	//操作
	void EndingCameraman::OnUpdate() {
		//ステートマシンのUpdateを行う
		m_StateMachine->Update();
	}

	void EndingCameraman::ExcuteBehavior(float totaltime) {
		if (m_totalTime > totaltime) return;
		m_totalTime += App::GetApp()->GetElapsedTime();
		Easing<Vec3> easing;
		m_eyePos = easing.EaseInOut(EasingType::Cubic, m_startPos, m_endPos, m_totalTime, totaltime);
		m_atPos = easing.EaseInOut(EasingType::Cubic, m_atStartPos, m_atEndPos, m_totalTime, totaltime);
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_eyePos);
	}

	void EndingCameraman::BasicStateEnterBehavior() {
		auto ptrStageMgr = GetStage()->GetSharedGameObject<StageGenerator>(L"StageManager");
		//ptrStageMgr->ToMainCamera();
	}

	//--------------------------------------------------------------------------------------
	//	class EndingCameramanBasicState : public ObjState<EndingCameraman>;
	//--------------------------------------------------------------------------------------
	void EndingCameramanBasicState::Enter(const shared_ptr<EndingCameraman>& Obj) {
		Obj->BasicStateEnterBehavior();
	}
	void EndingCameramanBasicState::Execute(const shared_ptr<EndingCameraman>& Obj) {
		Obj->ExcuteBehavior(4.0f);
	}
	void EndingCameramanBasicState::Exit(const shared_ptr<EndingCameraman>& Obj) {
	}

	EndingCamera::EndingCamera() :
		Camera()
	{
	}
	EndingCamera::~EndingCamera() {}

	void EndingCamera::OnUpdate() {
		Camera::OnUpdate();

		auto ptrEndingCameraman = dynamic_pointer_cast<EndingCameraman>(GetCameraObject());
		if (ptrEndingCameraman) {
			auto pos = ptrEndingCameraman->GetAtPos();
			auto eye = ptrEndingCameraman->GetEyePos();
			SetEye(eye);
			SetAt(pos);
		}
	}

}