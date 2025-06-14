/*!
@file EmenyImage.cpp
@brief キャラクターなど実体
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class Square : public GameObject;   //スクエアの表示
	//--------------------------------------------------------------------------------------
	Square::Square(const shared_ptr<Stage>& stage,
		const float width, 
		const float height,
		const wstring meshName,
		const Col4 color,
		const shared_ptr<Enemy>& enemy,
		const Vec3 pos
	):
		GameObject(stage),
		m_width(width),
		m_height(height),
		m_meshName(meshName),
		m_color(color),
		m_enemy(enemy),
		m_pos(pos),
		m_posHight(0.0f)
	{}
	
	void Square::OnCreate() {
		m_trans = GetComponent<Transform>();
		auto enemy = m_enemy.lock();
		if (enemy) {
			auto enemyTrans = enemy->GetComponent<Transform>();
			auto enemyPos = enemy->GetWorldPos();
			auto enemyScal = enemyTrans->GetScale();
			m_trans->SetPosition(Vec3(enemyPos.x, enemyPos.y + enemyScal.y / 2, enemyPos.z));
			m_trans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
			m_trans->SetRotation(Vec3(0.0f, 0.0f, 0.0f));
		}
		else {
			m_trans->SetPosition(m_pos);
			m_trans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
			m_trans->SetRotation(Vec3(0.0f, 0.0f, 0.0f));
		}
		//頂点データ
		m_normalVertices = {
			{Vec3(-m_width * 0.5f, m_height * 0.5f,0.0f), bsm::Vec3(0.0f, 0.0f, -1.0f),Vec2(0.0f,0.0f)},
			{Vec3(m_width * 0.5f, m_height * 0.5f,0.0f), bsm::Vec3(0.0f, 0.0f, -1.0f),Vec2(1.0f,0.0f)},
			{Vec3(-m_width * 0.5f,-m_height * 0.5f,0.0f), bsm::Vec3(0.0f, 0.0f, -1.0f),Vec2(0.0f,1.0f)},
			{Vec3(m_width * 0.5f,-m_height * 0.5f,0.0f), bsm::Vec3(0.0f, 0.0f, -1.0f),Vec2(1.0f,1.0f)}
		};
		m_indices = {
			0,1,2,
			2,1,3
		};
		for (auto& v : m_normalVertices) {
			VertexPositionColorTexture nv;
			nv.position = v.position;
			nv.color = m_color;
			nv.textureCoordinate = v.textureCoordinate;
			m_vertices.push_back(nv);
		}

		m_squareMesh = MeshResource::CreateMeshResource<VertexPositionColorTexture>(m_vertices, m_indices, true);

		m_draw = AddComponent<PCTStaticDraw>();
		m_draw->SetMeshResource(m_squareMesh);
		m_draw->SetTextureResource(m_meshName);
		m_draw->SetOriginalMeshResource(m_squareMesh);
		m_draw->SetDepthStencilState(DepthStencilState::Read);
		SetAlphaActive(true);//透過処理有効
		SetDrawLayer(1);
		if (enemy) {
			m_draw->SetDrawActive(false);

		}
		m_posHight = enemy->GetComponent<Transform>()->GetScale().y * 2.0f;
	}
	void Square::OnUpdate() {
		auto enemy = m_enemy.lock();
		if (enemy) {
			Vec3 enemyScal = enemy->GetComponent<Transform>()->GetScale();
			m_pos = enemy->GetWorldPos();
			m_trans->SetPosition(Vec3(m_pos.x, m_pos.y + m_posHight, m_pos.z));
			
			//オーバーヒートの時のみ表示
			//if (enemy->GetOverHeat()) {
			//	m_draw->SetDrawActive(true);
			//}
			//else {
			//	m_draw->SetDrawActive(false);
			//}
		}
		auto ptrCamera = GetStage()->GetView()->GetTargetCamera();
		Quat Qt = RotCorrection(ptrCamera->GetAt() - ptrCamera->GetEye());
		m_trans->SetQuaternion(Qt);

		
		//Debug();
	}
	//回転の補正
	Quat Square::RotCorrection(const Vec3& Line) {
		Vec3 temp = Line;
		Vec2 tempVec2(temp.x, temp.z);
		Vec3 DefUp(0.0f, 1.0f, 0.0f);
		Mat4x4 RotMatrix;

		//if (tempVec2.length() < 0.1f) {
		//	DefUp = Vec3(0.0f, 0.0f, 1.0f);
		//}
		temp.normalize();
		RotMatrix = XMMatrixLookAtLH(Vec3(0.0f), temp, DefUp);
		RotMatrix.inverse();
		Quat Qt;
		Qt = RotMatrix.quatInMatrix();
		Qt.normalize();
		return Qt;
	}
	void Square::DrawGauge(bool flag) {
		m_draw->SetDrawActive(flag);
	}
	//削除
	void Square::ThisDestroy() {
		auto stage = GetStage();
		stage->RemoveGameObject<Square>(GetThis<Square>());
	}
	void Square::SetPosHight(float hight) {
		m_posHight = hight;
	}
	//デバック用
	void Square::Debug() {
		//auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		//auto scene = App::GetApp()->GetScene<Scene>();
		//auto enemy = m_owner.lock();
		//auto enemyPos = enemy->GetComponent<Transform>()->GetPosition();
		//wstringstream wss(L"");
		//wss << L"pos ¥n("
		//	<<m_pos.x
		//	<<L"¥n,"
		//	<<m_pos.y
		//	<<L"¥n,"
		//	<<m_pos.z
		//	<<L")"
		//	<< L"enemyPos ¥n("
		//	<< enemyPos.x
		//	<< L"¥n,"
		//	<< enemyPos.y
		//	<< L"¥n,"
		//	<< enemyPos.z
		//	<< L")"
		//	<<L"¥n"
		//	<<m_test
		//	<< endl;
		//scene->SetDebugString(wss.str());
	}

	//オーバーヒートゲージ
	//--------------------------------------------------------------------------------------
	//	class GaugeSquare : public Square;   //スクエアのゲージ表示
	//--------------------------------------------------------------------------------------
	GaugeSquare::GaugeSquare(const shared_ptr<Stage>& stage,
		const float width, const float height,
		const wstring meshName,
		const Col4 color,
		const shared_ptr<Enemy>& enemy,
		const Vec3 pos
		) :
		Square(stage, width, height, meshName, color, enemy, pos),
		m_enemy(enemy)
	{}
	void GaugeSquare::OnUpdate() {
		Square::OnUpdate();
		auto enemy = m_enemy.lock();
		if (enemy) {
			UpdateValue(enemy->GetHeatRatio());
		}
		else {
			ThisDestroy();
		}
	}
	//頂点の更新処理
	void GaugeSquare::UpdateValue(float ratio) {
		auto moveX = (m_width * ratio) / m_width;
		m_vertices[1].position.x = -m_width / 2 + (m_width * ratio);
		m_vertices[3].position.x = -m_width / 2 + (m_width * ratio);

		m_vertices[1].textureCoordinate.x = moveX;
		m_vertices[3].textureCoordinate.x = moveX;

		m_draw->UpdateVertices(m_vertices);
	}

}