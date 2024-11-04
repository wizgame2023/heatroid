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
		m_width(0.25),
		m_height(0.25),
		m_moveX(0.0f),
		m_moveY(0.0f),
		m_color(1.0f,1.0f,1.0f,1.0f)

	{}
	void GaugeSquare::OnCreate() {
		m_trans = GetComponent<Transform>();
		if (!m_enemy.expired()) {
			auto enemy = m_enemy.lock();
			auto enemyTrans = enemy->GetComponent<Transform>();
			auto enemyPos = enemyTrans->GetPosition();
			float rad = XMConvertToRadians(180);
			m_trans->SetPosition(Vec3(enemyPos.x, enemyPos.y, enemyPos.z + 1.0f));
			m_trans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
			m_trans->SetRotation(Vec3(0.0f, rad, 0.0f));

			MeshUtill::CreateSquare(1.0f, m_normalVertices, m_indices);
			m_normalVertices = {
				{Vec3(0,0,0),Vec3(1.0f),Vec2(0.0f,0.0f)},
				{Vec3(m_width,0,0),Vec3(1.0f),Vec2(1.0f,0.0f)},
				{Vec3(0,-m_height,0),Vec3(1.0f),Vec2(0.0f,1.0f)},
				{Vec3(m_width,-m_height,0.0f),Vec3(1.0f),Vec2(1.0f,1.0f)}
			};
		
			//m_normalVertices[0].textureCoordinate = Vec2(0.0f, 0.0f);
			//m_normalVertices[1].textureCoordinate = Vec2(1.0f, 0.0f);
			//m_normalVertices[2].textureCoordinate = Vec2(0.0f, 1.0f);
			//m_normalVertices[3].textureCoordinate = Vec2(1.0f, 1.0f);


			//m_vertices = {
			//	{Vec3(0,0,0),m_color,Vec2(0.0f,0.0f)},
			//	{Vec3(m_width,0,0),m_color,Vec2(1.0f,0.0f)},
			//	{Vec3(0,-m_height,0),m_color,Vec2(0.0f,1.0f)},
			//	{Vec3(m_width,-m_height,0.0f),m_color,Vec2(1.0f,1.0f)}
			//};
			//m_indices = {
			//	0,1,2,
			//	2,1,3
			//};
		
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
			SetAlphaActive(true);//透過処理有効
			SetDrawLayer(1);
			m_draw->SetOriginalMeshResource(m_squareMesh);
			UpdateValue(enemy->GetHpRatio());
		}


	}
	void GaugeSquare::OnUpdate() {
		if (!m_enemy.expired()) {
			auto enemy = m_enemy.lock();
			auto enemyTrans = enemy->GetComponent<Transform>();
			m_trans = GetComponent<Transform>();
			m_pos = enemyTrans->GetPosition();
			//m_pos.z += 1.0f;
			m_trans->SetPosition(Vec3(m_pos.x, m_pos.y, m_pos.z + 1.0f));
			m_trans->SetScale(1.0f, 1.0f, 1.0f);
			//auto camera = GetStage()->GetView()->GetTargetCamera();
			//Debug();

		}
	}

	void GaugeSquare::UpdateValue(float ratio) {
		auto moveX = (m_width * ratio) / m_width;
		m_vertices[1].position.x = (m_width * ratio);
		m_vertices[3].position.x = (m_width * ratio);

		m_vertices[1].textureCoordinate.x = moveX;
		m_vertices[3].textureCoordinate.x = moveX;

		m_draw->UpdateVertices(m_vertices);
	}

	void GaugeSquare::ThisDestroy() {
		auto stage = GetStage();
		stage->RemoveGameObject<GaugeSquare>(GetThis<GaugeSquare>());
	}
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
			<< endl;
		scene->SetDebugString(wss.str());

	}
}