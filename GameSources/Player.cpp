/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Speed(10.0f),
		m_jumpHeight(20.0f),
		m_gravity(-30.0f),
		m_moveVel(Vec2(0, 0)),
		m_stateType(stand)
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
		if (KeyState.m_bPushKeyTbl['W']) { ret.y = 1.0f; }
		if (KeyState.m_bPushKeyTbl['A']) { ret.x = -1.0f; }
		if (KeyState.m_bPushKeyTbl['S']) { ret.y = -1.0f; }
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
		float elapsedTime = App::GetApp()->GetElapsedTime();
		auto angle = GetMoveVector();
		if (angle.length() > 0.0f) {
			auto pos = GetComponent<Transform>()->GetPosition();
			pos += angle * elapsedTime * m_Speed;
			GetComponent<Transform>()->SetPosition(pos.x, pos.y, 0);
		}
		//回転の計算
		if (angle.length() > 0.0f) {
			auto utilPtr = GetBehavior<UtilBehavior>();
			utilPtr->RotToHead(angle, 1.0f);
		}
	}

	void Player::OnCreate() {
		//経過時間を取得
		float elapsedTime = App::GetApp()->GetElapsedTime();

		AddTag(L"Player");

		//初期位置などの設定
		auto ptr = AddComponent<Transform>();
		ptr->SetScale(1.00f, 1.00f, 1.00f);	//直径25センチの球体
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(Vec3(0, 0.125f, 0));

		m_prevPos = GetComponent<Transform>()->GetPosition();

		//CollisionSphere衝突判定を付ける
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(true);

		//各パフォーマンスを得る
		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

		//重力をつける
		auto ptrGra = AddComponent<Gravity>();
		ptrGra->SetGravity(Vec3(0, m_gravity, 0));

		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネントの設定
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		
		//描画するメッシュを設定
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetFogEnabled(true);

	}

	void Player::OnUpdate() {
		//コントローラチェックして入力があればコマンド呼び出し
		m_InputHandler.PushHandle(GetThis<Player>());
		float elapsed = App::GetApp()->GetElapsedTime();
		frameElapsed += elapsed;

		if (frameElapsed > _frame) {
			frameElapsed -= _frame;

			auto pos = GetComponent<Transform>()->GetPosition();
			RoundOff(pos, 2);
			m_moveVel = RoundOff(pos - m_prevPos, 2);

			if (m_stateType == air && m_moveVel.y < 0) {
				m_stateType = fall;
			}

			m_prevPos = pos;
		}

		auto grav = GetComponent<Gravity>();

		MovePlayer();
		MoveCamera();

		ShowDebug();
	}

	void Player::ShowDebug() {
		wstringstream wss;
		auto pos = GetComponent<Transform>()->GetPosition();

		wss << "stateType : " << m_stateType << endl;
		wss << "pos : " << pos.x << ", " << pos.y << endl;
		wss << "prevPos : " << m_prevPos.x << ", " << m_prevPos.y << endl;
		wss << "vel : " << m_moveVel.x << ", " << m_moveVel.y << endl;

		auto scene = App::GetApp()->GetScene<Scene>();
		scene->SetDebugString(L"Player\n" + wss.str());
	}

	void Player::OnPushA()
	{
		if (m_stateType != stand) return;	//立ち状態以外ではジャンプしない
		auto grav = GetComponent<Gravity>();
		grav->StartJump(Vec3(0, m_jumpHeight, 0)); 
		m_stateType = air;
	}

	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (m_stateType != stand && Other->FindTag(L"FixedBox")) {
			m_stateType = stand;
		}

		//メモ　地形オブジェクトのタグをWallとFloorに分けて接地判定を実装したい

	}
	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		if (m_stateType == stand) m_stateType = air;

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

	float Player::GetCollisionScale() {
		return GetComponent<CollisionSphere>()->GetMakedRadius();
	}


	Vec3 Player::RoundOff(Vec3 number, int point) {
		Vec3 r;
		r.x = round(number.x * pow(10, point));
		r.y = round(number.y * pow(10, point));
		r.z = round(number.z * pow(10, point));
		r /= pow(10, point);
		return r;
	}
  
}
//end basecross

