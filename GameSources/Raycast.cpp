/*!
@file Raycast.cpp
@brief ���C���΂�
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class LineObject : public GameObject; //����`�悷��I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	LineObject::LineObject(const shared_ptr<Stage>& stage
	):
		GameObject(stage)
	{}
	LineObject::LineObject(const shared_ptr<Stage>& stage,
		const shared_ptr<Player>& player,
		const shared_ptr<Enemy>& enemy
	) :
		GameObject(stage),
		m_player(player),
		m_enemy(enemy),
		m_direc(0.0f),
		m_startPos(Vec3(0.0f)),
		m_endPos(Vec3(0.0f)),
		m_startColor(Col4(1.0f)),
		m_endColor(Col4(1.0f))
	{}
	void LineObject::OnCreate() {
		//�����\������2�_
		m_vertices = {
			{m_startPos, m_startColor},
			{m_endPos, m_endColor}
		};
		//�n�_�ƏI�_���Ȃ��C���f�b�N�X
		m_indices = {
			0,1
		};

		//�`��
		m_draw = AddComponent<PCStaticDraw>(); //�ʒu�ƐF�̂�
		m_draw->SetOriginalMeshUse(true); //���삵�����b�V�����g�p
		m_draw->CreateOriginalMesh(m_vertices, m_indices); //���b�V���̍쐬
		auto meshResoure = m_draw->GetMeshResource(); //���b�V�����\�[�X���擾���A�v���~�e�B�u�|���W�[�i���_���p���@�j��ύX����
		meshResoure->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP); //�|���S���ł͂Ȃ��Ő���\��
	}
	void LineObject::OnUpdate() {
		auto player = m_player.lock();
		if (!player) return;
		auto enemy = m_enemy.lock();
		if (!enemy) return;



		SetLinePosition(player->GetComponent<Transform>()->GetPosition(), LinePos(Vec3(0.0f,3.0f,0.0f)));
		
	}
	Vec3 LineObject::LinePos(Vec3 pos) {
		auto enemy = m_enemy.lock();
		if (enemy) {
			Vec3 enemyPos = enemy->GetWorldPos();
			Vec3 forward = enemy->GetComponent<Transform>()->GetForward();
			float face = atan2f(forward.z, forward.x);
			Vec3 linePos;
			linePos.x = (cosf(face) * pos.x) - (sinf(face) * pos.z);
			linePos.y = pos.y;
			linePos.z = (cosf(face) * pos.z) + (sinf(face) * pos.x);
			return enemyPos +linePos;
		}
	}

	//���_�̍X�V
	void LineObject::VerticesUpdate() {
		m_vertices = {
			{m_startPos, m_startColor},
			{m_endPos, m_endColor}
		};

		m_draw->UpdateVertices(m_vertices);
	}

	//���_�̐ݒ�
	void LineObject::SetLinePosition(const Vec3& startPos, const Vec3& endPos) {
		m_startPos = startPos;
		m_endPos = endPos;
		m_direc = endPos - startPos;
		m_direc = m_direc.normalize();

		VerticesUpdate();
	}

	//���̐F�̐ݒ�
	void LineObject::SetLineColor(const Col4& startColor, const Col4& endColor) {
		m_startColor = startColor;
		m_endColor = endColor;

		VerticesUpdate();
	}

	Vec3 LineObject::GetStartPos() {
		return m_startPos;
	}
	Vec3 LineObject::GetEndPos() {
		return m_endPos;
	}
	Vec3 LineObject::GetDirec() {
		return m_direc;
	}
	//--------------------------------------------------------------------------------------
	//	class RayMark : public GameObject; //�Փ˂����Ƃ��̕\�������I�u�W�F�N�g
	//--------------------------------------------------------------------------------------

	RayMark::RayMark(const shared_ptr<Stage>& stage
	):
		GameObject(stage)
	{}
	void RayMark::OnCreate() {
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_SPHERE");
		Col4 color(0.0f, 0.0f, 1.0f, 0.4f);
		draw->SetDiffuse(color);
		draw->SetEmissive(color);
		
		SetAlphaActive(true);

		m_trans = GetComponent<Transform>();
		m_trans->SetScale(Vec3(1.0f));
	}
	void RayMark::OnUpdate() {
		auto stage = GetStage();
		//�\�������
		//auto line = stage->GetSharedGameObject<LineObject>(L"Line");


		Vec3 pos = Vec3(0.0f);
		Vec3 rayStart;
		Vec3 rayEnd;
		Vec3 crossPos;
		//TRIANGLE triangle;
		//size_t triangleIndex;

		auto lineGroup = GetStage()->GetSharedObjectGroup(L"Line");
		auto& lineVec = lineGroup->GetGroupVector();
		for (auto v : lineVec) {
			auto shObj = v.lock();
			auto lineDraw = shObj->GetComponent<PCStaticDraw>();
			auto line = dynamic_pointer_cast<LineObject>(shObj);
			TRIANGLE triangle;
			size_t triangleIndex;

			rayStart = line->GetStartPos();
			rayEnd = line->GetEndPos();
		}


		//�G
		//auto target = stage->GetSharedGameObject<Enemy>(L"Enemys");
		//auto enemyDraw = target->GetComponent<PNTBoneModelDraw>();
		//��
		auto enemyGroup = GetStage()->GetSharedObjectGroup(L"Enemy");
		auto& enemyVec = enemyGroup->GetGroupVector();
		for (auto v : enemyVec) {
			auto shObj = v.lock();
			auto m_enemyDraw = shObj->GetComponent<PNTBoneModelDraw>();
			TRIANGLE triangle;
			size_t triangleIndex;
			m_hitEnemyFlag = m_enemyDraw->HitTestStaticMeshSegmentTriangles(rayStart, rayEnd, crossPos, triangle, triangleIndex);
		}


		//m_hitWallFlag = wallDraw->HitTestStaticMeshSegmentTriangles(rayStart, rayEnd, crossPos, triangle, triangleIndex);
		//m_hitDoorFlag = floorDraw->HitTestStaticMeshSegmentTriangles(rayStart, rayEnd, crossPos, triangle, triangleIndex);

		if (m_hitEnemyFlag) {
			pos = crossPos;
		}
		m_trans->SetPosition(pos);
	}
	void RayMark::OnDraw() {
		if (m_hitEnemyFlag) {
			GameObject::OnDraw();
			if (!m_hitWallFlag && !m_hitFloorFlag && !m_hitDoorFlag) {
				
			}
		}
	}
}

//end basecross
