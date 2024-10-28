/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	Player::Player(const shared_ptr<Stage>& StagePtr) : GameObject(StagePtr),
		m_speed(.225f),
		m_accel(.1f),
		m_friction(.9f),
		m_frictionThreshold(.001f),
		m_jumpHeight(.3f),
		m_gravity(-.098f),
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
		ptr->SetScale(0.10f, 0.10f, 0.10f);	//直径25センチの球体
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

		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wss << "stateType : " << m_stateType << endl;
		wss << "pos : " << pos.x << ", " << pos.y << endl;
		wss << "vel : " << m_moveVel.x << ", " << m_moveVel.y << endl;
		wss << "exitcnt : " << m_collideCount << endl;
		wss << "hp : " << m_HP << " / " << m_HP_max << endl;
		wss << "fps :" << fps << " / " << endl;
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
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if ((Other->FindTag(L"GimmickButton")))
		{
			GimmickButton::SetButton(true);
		}

	}

	void Player::MoveCamera()
	{
		auto ptrCamera = dynamic_pointer_cast<Camera>(OnGetDrawCamera());
		auto pos = GetComponent<Transform>()->GetPosition();
		Vec3 Camera = ptrCamera->GetEye();
		float differenceX = pos.x - Camera.x;
		if (differenceX >= 0.5f)
		{
			ptrCamera->SetEye(Camera.x + (differenceX - 0.5f), 0.5f, Camera.z);
			ptrCamera->SetAt(pos.x - differenceX, 0.5f, pos.z);
		}
		else if (differenceX <= -0.5f)
		{
			ptrCamera->SetEye(Camera.x + (differenceX + 0.5f), 0.5f, Camera.z);
			ptrCamera->SetAt(pos.x - differenceX, 0.5f, pos.z);
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
}
//end basecross

