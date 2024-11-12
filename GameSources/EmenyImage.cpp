/*!
@file EmenyImage.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	GaugeSquare::GaugeSquare(const shared_ptr<Stage>& stage,
		const shared_ptr<Enemy>& enemy):
		GameObject(stage),
		m_enemy(enemy),
		m_sizeX(0),
		m_sizeY(0),
		m_width(1.5),
		m_height(0.15),
		m_moveX(0.0f),
		m_moveY(0.0f),
		m_color(1.0f,0.0f,0.0f,1.0f)

	{}
	void GaugeSquare::OnCreate() {
		m_trans = GetComponent<Transform>();
		if (!m_enemy.expired()) {
			auto enemy = m_enemy.lock();
			auto enemyTrans = enemy->GetComponent<Transform>();
			auto enemyPos = enemy->GetChangePos();
			float rad = XMConvertToRadians(180);
			m_trans->SetPosition(Vec3(enemyPos.x, enemyPos.y, enemyPos.z));
			m_trans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
			m_trans->SetRotation(Vec3(0.0f, rad, 0.0f));
			//MeshUtill::CreateSquare(1.0f, m_normalVertices, m_indices);
		
			//m_normalVertices = {
			//	{Vec3(-m_width * 0.0f, m_height * 0.0f,0.0f), bsm::Vec3(0.0f, 0.0f, -1.0f),Vec2(0.0f,0.0f)},
			//	{Vec3( m_width * 1.0f, m_height * 0.0f,0.0f), bsm::Vec3(0.0f, 0.0f, -1.0f),Vec2(1.0f,0.0f)},
			//	{Vec3(-m_width * 0.0f,-m_height * 1.0f,0.0f), bsm::Vec3(0.0f, 0.0f, -1.0f),Vec2(0.0f,1.0f)},
			//	{Vec3( m_width * 1.0f,-m_height * 1.0f,0.0f), bsm::Vec3(0.0f, 0.0f, -1.0f),Vec2(1.0f,1.0f)}
			//};

			//頂点データ
			m_normalVertices = {
				{Vec3(-m_width * 0.5f, m_height * 0.5f,0.0f), bsm::Vec3(0.0f, 0.0f, -1.0f),Vec2(0.0f,0.0f)},
				{Vec3( m_width * 0.5f, m_height * 0.5f,0.0f), bsm::Vec3(0.0f, 0.0f, -1.0f),Vec2(1.0f,0.0f)},
				{Vec3(-m_width * 0.5f,-m_height * 0.5f,0.0f), bsm::Vec3(0.0f, 0.0f, -1.0f),Vec2(0.0f,1.0f)},
				{Vec3( m_width * 0.5f,-m_height * 0.5f,0.0f), bsm::Vec3(0.0f, 0.0f, -1.0f),Vec2(1.0f,1.0f)}
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
			m_draw->SetTextureResource(L"White");
			m_draw->SetOriginalMeshResource(m_squareMesh);
			SetAlphaActive(true);//透過処理有効
			SetDrawLayer(1);
		}


	}
	void GaugeSquare::OnUpdate() {
		if (!m_enemy.expired()) {
			auto enemy = m_enemy.lock();
			auto enemyTrans = enemy->GetComponent<Transform>();
			m_trans = GetComponent<Transform>();
			m_pos = enemy->GetChangePos();
			m_trans->SetPosition(Vec3(m_pos.x, m_pos.y + 1.5f, m_pos.z));
			auto ptrCamera = GetStage()->GetView()->GetTargetCamera();

			//頂点の更新
			UpdateValue(enemy->GetHpRatio());

			Quat Qt = RotCorrection(ptrCamera->GetAt() - ptrCamera->GetEye());
			m_trans->SetQuaternion(Qt);
			//Debug();
		}
		else {
			//ゲージの対象のオブジェクトが消えたら自分も削除
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
	//回転の補正
	Quat GaugeSquare::RotCorrection(const Vec3& Line) {
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
	//削除
	void GaugeSquare::ThisDestroy() {
		auto stage = GetStage();
		stage->RemoveGameObject<GaugeSquare>(GetThis<GaugeSquare>());
	}
	//デバック用
	void GaugeSquare::Debug() {
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		auto scene = App::GetApp()->GetScene<Scene>();
		auto enemy = m_enemy.lock();
		auto enemyPos = enemy->GetComponent<Transform>()->GetPosition();
		wstringstream wss(L"");
		wss << L"pos \n("
			<<m_pos.x
			<<L"\n,"
			<<m_pos.y
			<<L"\n,"
			<<m_pos.z
			<<L")"
			<< L"enemyPos \n("
			<< enemyPos.x
			<< L"\n,"
			<< enemyPos.y
			<< L"\n,"
			<< enemyPos.z
			<< L")"
			<<L"\n"
			<<m_test
			<< endl;
		scene->SetDebugString(wss.str());

	}
}