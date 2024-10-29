/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"
#include "Collision.h"

namespace basecross {

//====================================================================
// class TriggerColl
// 押し合い処理を行わない球形コライダ
//====================================================================

	struct TriggerColl::Impl {
		float m_MadeDiameter;					//作成時の直径
		//配列ボリュームと衝突時に衝突した配列を特定するインデックス
		size_t m_IsHitVolumeIndex;
		CalcScaling m_CalcScaling;
		Impl() :
			m_MadeDiameter(1.0f),
			m_IsHitVolumeIndex(0),
			m_CalcScaling(CalcScaling::YScale)
		{}
		~Impl() {};
	};

	TriggerColl::TriggerColl(const shared_ptr<GameObject>& GameObjectPtr) :
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	TriggerColl::~TriggerColl() {}

	//アクセサ
	float TriggerColl::GetMadeDiameter() const {
		return pImpl->m_MadeDiameter;
	}
	void TriggerColl::SetMadeDiameter(float f) {
		pImpl->m_MadeDiameter = f;
	}
	float TriggerColl::GetMadeRadius() const {
		return pImpl->m_MadeDiameter * 0.5f;
	}
	void TriggerColl::SetMadeRadius(float f) {
		pImpl->m_MadeDiameter = f * 2.0f;
	}

	CalcScaling TriggerColl::GetCalcScaling() const {
		return pImpl->m_CalcScaling;

	}
	void TriggerColl::SetCalcScaling(CalcScaling s) {
		pImpl->m_CalcScaling = s;
	}


	SPHERE TriggerColl::GetSphere() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Mat4x4 MatBase;
		MatBase.scale(bsm::Vec3(pImpl->m_MadeDiameter, pImpl->m_MadeDiameter, pImpl->m_MadeDiameter));
		MatBase *= TransPtr->GetWorldMatrix();
		//このオブジェクトのSPHEREを作成
		SPHERE Ret(MatBase.transInMatrix(), MatBase.scaleInMatrix().x * 0.5f);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::XScale:
			Ret.m_Radius = MatBase.scaleInMatrix().x * 0.5f;
			break;
		case CalcScaling::YScale:
			Ret.m_Radius = MatBase.scaleInMatrix().y * 0.5f;
			break;
		case CalcScaling::ZScale:
			Ret.m_Radius = MatBase.scaleInMatrix().z * 0.5f;
			break;
		default:
			break;
		}
		return Ret;
	}


	SPHERE TriggerColl::GetBeforeSphere() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Mat4x4 MatBase;
		MatBase.scale(bsm::Vec3(pImpl->m_MadeDiameter, pImpl->m_MadeDiameter, pImpl->m_MadeDiameter));
		MatBase *= TransPtr->GetBeforeWorldMatrix();
		//このオブジェクトのSPHEREを作成
		SPHERE Ret(MatBase.transInMatrix(), MatBase.scaleInMatrix().x * 0.5f);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::XScale:
			Ret.m_Radius = MatBase.scaleInMatrix().x * 0.5f;
			break;
		case CalcScaling::YScale:
			Ret.m_Radius = MatBase.scaleInMatrix().y * 0.5f;
			break;
		case CalcScaling::ZScale:
			Ret.m_Radius = MatBase.scaleInMatrix().z * 0.5f;
			break;
		default:
			break;
		}
		return Ret;
	}

	bool TriggerColl::SimpleCollisionCall(const shared_ptr<Collision>& Src) {
		return Src->SimpleCollision(GetThis<CollisionSphere>());
	}
	
	bool TriggerColl::SimpleCollision(const shared_ptr<CollisionSphere>& DestColl) {
		auto SrcSp = GetSphere();
		auto DestSp = DestColl->GetSphere();
		if (!HitTest::AABB_AABB(SrcSp.GetWrappedAABB(), DestSp.GetWrappedAABB())) {
			return false;
		}
		if (HitTest::SPHERE_SPHERE(SrcSp, DestSp)) {
			return true;
		}
		return false;
	}

	bool TriggerColl::SimpleCollision(const shared_ptr<CollisionCapsule>& DestColl) {
		auto SrcSp = GetSphere();
		auto DestCap = DestColl->GetCapsule();
		if (!HitTest::AABB_AABB(SrcSp.GetWrappedAABB(), DestCap.GetWrappedAABB())) {
			return false;
		}
		bsm::Vec3 d;
		if (HitTest::SPHERE_CAPSULE(SrcSp, DestCap, d)) {
			return true;
		}
		return false;

	}

	bool TriggerColl::SimpleCollision(const shared_ptr<CollisionObb>& DestColl) {
		auto SrcSp = GetSphere();
		auto DestObb = DestColl->GetObb();
		if (!HitTest::AABB_AABB(SrcSp.GetWrappedAABB(), DestObb.GetWrappedAABB())) {
			return false;
		}
		bsm::Vec3 d;
		if (HitTest::SPHERE_OBB(SrcSp, DestObb, d)) {
			return true;
		}
		return false;
	}

	bool TriggerColl::SimpleCollision(const shared_ptr<CollisionRect>& DestColl) {
		auto SrcSp = GetSphere();
		auto DestCol = DestColl->GetColRect();
		if (!HitTest::AABB_AABB(SrcSp.GetWrappedAABB(), DestCol.GetWrappedAABB())) {
			return false;
		}
		bsm::Vec3 d;
		if (HitTest::SPHERE_COLRECT(SrcSp, DestCol, d)) {
			return true;
		}
		return false;
	}

	void TriggerColl::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<TriggerColl>());
	}

	void TriggerColl::CollisionTest(const shared_ptr<TriggerColl>& DestColl) {

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//球の場合は、すべて移動以外変化なしとする
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手のCollisionSphere
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		//簡易的な判定
		if (!HitTest::SPHERE_SPHERE(SrcSphere, DestSphere)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereSphere(SrcBeforSphere, SpanVelocity, DestBeforeSphere, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	void TriggerColl::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//球の場合は、すべて移動以外変化なしとする
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手のCollisionSphere
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		//簡易的な判定
		if (!HitTest::SPHERE_SPHERE(SrcSphere, DestSphere)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereSphere(SrcBeforSphere, SpanVelocity, DestBeforeSphere, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	void TriggerColl::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手
		CAPSULE DestCap = DestColl->GetCapsule();
		CAPSULE DestBeforeCap = DestColl->GetBeforeCapsule();
		//簡易的な判定
		bsm::Vec3 ret;
		if (!HitTest::SPHERE_CAPSULE(SrcSphere, DestCap, ret)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(SrcBeforSphere, SpanVelocity, DestBeforeCap, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			GetCollisionManager()->InsertNewPair(pair);
		}
	}


	void TriggerColl::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		//簡易的な判定
		bsm::Vec3 ret;
		if (!HitTest::SPHERE_OBB(SrcSphere, DestObb, ret)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(SrcBeforSphere, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	void TriggerColl::CollisionTest(const shared_ptr<CollisionRect>& DestColl) {

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手
		COLRECT DestRect = DestColl->GetColRect();
		COLRECT DestBeforeRect = DestColl->GetBeforeColRect();
		//簡易的な判定
		bsm::Vec3 ret;
		if (!HitTest::SPHERE_COLRECT(SrcSphere, DestRect, ret)) {
			return;
		}
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereRect(SrcBeforSphere, SpanVelocity, DestBeforeRect, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			GetCollisionManager()->InsertNewPair(pair);
		}
	}


	bsm::Vec3 TriggerColl::GetCenterPosition()const {
		SPHERE SrcSphere = GetSphere();
		return SrcSphere.m_Center;
	}

	AABB TriggerColl::GetEnclosingAABB()const {
		AABB enc = GetBeforeSphere().GetWrappedAABB();
		enc.UnionAABB(GetSphere().GetWrappedAABB());
		return enc;
	}

	AABB TriggerColl::GetWrappedAABB()const {
		return GetSphere().GetWrappedAABB();
	}

	void TriggerColl::OnCreate() {
		SetDrawActive(false);
	}
	void TriggerColl::OnUpdate() {
	}
	void TriggerColl::OnDraw() {
		GenericDraw Draw;
		bsm::Mat4x4 MeshToTransformMatrix;

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Scale = PtrTransform->GetScale();
		bsm::Vec3 CollScale(Scale.x, Scale.x, Scale.x);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::YScale:
			CollScale = bsm::Vec3(Scale.y, Scale.y, Scale.y);
			break;
		case CalcScaling::ZScale:
			CollScale = bsm::Vec3(Scale.z, Scale.z, Scale.z);
			break;
		default:
			break;
		}
		bsm::Vec3 ColcScale(CollScale.x / Scale.x, CollScale.y / Scale.y, CollScale.z / Scale.z);
		bsm::Mat4x4 mat;
		mat.scale(ColcScale);

		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_SPHERE"),mat);
	}

//====================================================================
// class Player
// プレイヤークラス
//====================================================================

	Player::Player(const shared_ptr<Stage>& StagePtr) : 
		GameObject(StagePtr),
		m_speed(.225f),
		m_accel(.1f),
		m_friction(.84f),
		m_frictionThreshold(.001f),
		m_jumpHeight(.4f),
		m_gravity(-.075f),
		m_moveVel(Vec3(0, 0, 0)),
		m_collideCountInit(3),
		m_collideCount(m_collideCountInit),
		m_stateType(air),

		m_HP_max(100)
	{}

	Vec2 Player::GetInputState() const {
		Vec2 ret;
		//コントローラの取得
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		ret.x = 0.0f;
		ret.y = 0.0f;
		WORD wButtons = 0;
		if (cntlVec[0].bConnected) {
			ret.x = cntlVec[0].fThumbLX;
			ret.y = cntlVec[0].fThumbLY;
		}
		//キーボードの取得(キーボード優先)
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPushKeyTbl['A']) { ret.x = -1.0f; }
		if (KeyState.m_bPushKeyTbl['D']) { ret.x = 1.0f; }

		return ret;
	}


	Vec3 Player::GetMoveVector() const {
		Vec3 angle(0, 0, 0);
		//入力の取得
		float moveX = GetInputState().x;
		float moveZ = GetInputState().y;

		if (moveX + moveZ != 0) {
			float moveLength = 0;	//動いた時のスピード
			auto ptrTransform = GetComponent<Transform>();
			auto ptrCamera = OnGetDrawCamera();
			//進行方向の向きを計算
			auto front = ptrTransform->GetPosition() - ptrCamera->GetEye();
			front.y = 0;
			front.normalize();
			//進行方向向きからの角度を算出
			float frontAngle = atan2(front.z, front.x);
			//コントローラの向き計算
			Vec2 moveVec(moveX, moveZ);
			float moveSize = moveVec.length();
			//コントローラの向きから角度を計算
			float cntlAngle = atan2(-moveX, moveZ);
			//トータルの角度を算出
			float totalAngle = frontAngle + cntlAngle;
			//角度からベクトルを作成
			angle = Vec3(cos(totalAngle), 0, sin(totalAngle));
			//正規化する
			angle.normalize();
			//移動サイズを設定。
			angle *= moveSize;
			//Y軸は変化させない
			angle.y = 0;
		}
		return angle;
	}

	void Player::MovePlayer() {
		m_moveVel.x += GetMoveVector().x * m_accel * _delta;
		m_moveVel.y += m_gravity * _delta;

		if (m_stateType == stand) {
			if (GetMoveVector().x == 0) {
				if (abs(m_moveVel.x) <= m_frictionThreshold) m_moveVel.x = 0;
				else m_moveVel.x *= m_friction * _delta;
			}
		}

		if (m_moveVel.x > m_speed) m_moveVel.x = m_speed;
		if (m_moveVel.x < -m_speed) m_moveVel.x = -m_speed;

		auto trans = GetComponent<Transform>();
		trans->SetPosition((m_moveVel * _delta) + trans->GetPosition());

		if (m_stateType == stand && m_moveVel.y < m_gravity) m_moveVel.y = m_gravity;
	}

	void Player::OnCreate() {

		AddTag(L"Player");

		//初期位置などの設定
		auto ptr = AddComponent<Transform>();
		ptr->SetScale(0.10f, 0.10f, 0.10f);
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(Vec3(0, 1.0f, 0));

		//CollisionSphere衝突判定を付ける
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(true);

		//各パフォーマンスを得る
		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネントの設定
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();

		//描画するメッシュを設定
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetFogEnabled(true);

		m_HP = m_HP_max;

	}

	void Player::OnUpdate() {
		//コントローラチェックして入力があればコマンド呼び出し
		m_InputHandler.PushHandle(GetThis<Player>());

		MovePlayer();
		MoveCamera();

		m_collideCount--;
		if (m_stateType == stand && m_collideCount <= 0) m_stateType = air;
	}

	void Player::OnUpdate2() {
		ShowDebug();
	}

	void Player::ShowDebug() {
		wstringstream wss;
		auto pos = RoundOff(GetComponent<Transform>()->GetPosition(), 3);
		auto rot = RoundOff(GetComponent<Transform>()->GetRotation(), 3);

		wss << "stateType : " << m_stateType << endl;
		wss << "pos : " << pos.x << ", " << pos.y << endl;
		wss << "vel : " << m_moveVel.x << ", " << m_moveVel.y << endl;
		wss << "exitcnt : " << m_collideCount << endl;
		wss << "hp : " << m_HP << " / " << m_HP_max << endl;

		auto scene = App::GetApp()->GetScene<Scene>();
		scene->SetDebugString(L"Player\n" + wss.str());
	}

	void Player::OnPushA()
	{
		if (m_stateType != stand) return;	//立ち状態以外ではジャンプしない
		m_moveVel.y = m_jumpHeight;
		m_stateType = air;
	}

	void Player::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		m_collideCount = m_collideCountInit;
		if (m_stateType == air && Other->FindTag(L"FixedBox")) {
			m_stateType = stand;
		}
		//メモ　地形オブジェクトのタグをWallとFloorに分けて接地判定を実装したい

	}

	void Player::MoveCamera()
	{
		auto ptrCamera = dynamic_pointer_cast<Camera>(OnGetDrawCamera());
		auto pos = GetComponent<Transform>()->GetPosition();
		Vec3 Camera = ptrCamera->GetEye();
		float differenceX = pos.x - Camera.x;
		if (differenceX >= 5.0f)
		{
			ptrCamera->SetEye(Camera.x + (differenceX - 5.0f), 5.0f, Camera.z);
			ptrCamera->SetAt(pos.x - differenceX, 5.0f, pos.z);
		}
		else if (differenceX <= -5.0f)
		{
			ptrCamera->SetEye(Camera.x + (differenceX + 5.0f), 5.0f, Camera.z);
			ptrCamera->SetAt(pos.x - differenceX, 5.0f, pos.z);
		}
	}

	Vec3 Player::RoundOff(Vec3 number, int point) {
		Vec3 r = number *= pow(10, point);
		r.x = round(r.x);
		r.y = round(r.y);
		r.z = round(r.z);
		r /= pow(10, point);
		return r;
	}

	void Player::Gravity() {
		m_moveVel.y += m_gravity * _delta;
	}

	Vec3 Player::GetScale() {
		return GetComponent<Transform>()->GetScale();
	}

//====================================================================
// class AttackCollision
// プレイヤーの攻撃判定
//====================================================================

	AttackCollision::AttackCollision(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<GameObject>& player) :
		GameObject(StagePtr),
		m_player(player),
		m_distFromPlayer(Vec3(.1f, 0, 0))
	{}

	void AttackCollision::OnCreate() {
		auto trans = GetComponent<Transform>();
		trans->SetScale(0.10f, 0.10f, 0.10f);	//直径25センチの球体
		trans->SetRotation(0.0f, 0.0f, 0.0f);
		trans->SetPosition(Vec3(0, 0, 0));

		//仮(本実装時は、押し出しのないTrigger判定を独自に作る？)
		auto coll = AddComponent<TriggerColl>();
		coll->SetUpdateActive(true);
		coll->SetDrawActive(true);

	}

	void AttackCollision::OnUpdate() {
		FollowPlayer();
	}

	void AttackCollision::FollowPlayer() {
		auto trans = GetComponent<Transform>();
		//weakからsharedに格上げ
		auto ptrPlayer = m_player.lock();

		if (ptrPlayer != nullptr) {
			auto matPlayer = ptrPlayer->GetComponent<Transform>()->GetWorldMatrix();
			matPlayer.scaleIdentity();
			Mat4x4 matrix;
			matrix.affineTransformation(
				Vec3(1),
				Vec3(0),
				Vec3(0),
				m_distFromPlayer);
			matrix *= matPlayer;

			auto posPlayer = matrix.transInMatrix();
			trans->SetPosition(posPlayer);
			trans->SetQuaternion(matrix.quatInMatrix());
		}
	}
}
//end basecross

