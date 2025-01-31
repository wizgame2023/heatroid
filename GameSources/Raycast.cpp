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
	RayMark::RayMark(const shared_ptr<Stage>& stage,
		const shared_ptr<Player>& player,
		const shared_ptr<Enemy>& enemy
	) :
		GameObject(stage),
		m_player(player),
		m_enemy(enemy),
		m_wallCnt(0),
		m_activeFlag(false)
	{}

	void RayMark::OnCreate() {
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
	void RayMark::OnUpdate() {
		auto stage = GetStage();

		//�K�v�ȕϐ��̐錾
		Vec3 pos = Vec3(0.0f);
		Vec3 rayStart;
		Vec3 rayEnd;
		Vec3 enemyCrossPos;
		Vec3 objCrossPos;
		TRIANGLE triangle;
		size_t triangleIndex;

		//�v���C���[
		auto player = m_player.lock();
		if (!player) return;
		rayStart = player->GetComponent<Transform>()->GetPosition(); //���C���΂��n�_

		//���C���΂�
		//�G
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		auto enemyDraw = enemy->GetComponent<PNTBoneModelDraw>();
		rayEnd = enemyPos(Vec3(0.0f,3.0f,0.0f));		             //���C���΂��I�_
		//�������Ă��邩�̃t���O
		m_hitEnemyFlag = enemyDraw->HitTestStaticMeshSegmentTriangles
		(rayStart, rayEnd, enemyCrossPos, triangle, triangleIndex);

		//��
		auto wallGroup = GetStage()->GetSharedObjectGroup(L"Wall");
		auto& wallVec = wallGroup->GetGroupVector();
		for (auto v : wallVec) {
			auto walls = v.lock();
			auto wallDraw = walls->GetComponent<PNTStaticDraw>();
			//���ׂĂ̕ǂ��`�F�b�N�A���C���������Ă�����true
			m_hitWallFlag.push_back(wallDraw->HitTestStaticMeshSegmentTriangles
			(rayStart, rayEnd, objCrossPos, triangle, triangleIndex));
		}
		for (int i = 0; i < m_hitWallFlag.size(); i++) {
			if (!m_hitWallFlag[i]) {
				m_wallCnt++;
			}
		}

		//�h�A
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


		//�ǂ��h�A�ɂ������Ă��邩
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
		enemy->SetActiveFlag(m_activeFlag);

		//�f�o�b�N�p
		//�������Ă���ꏊ�̕`��
		if (m_hitEnemyFlag) {
			pos = enemyCrossPos;
		}
		m_trans->SetPosition(pos);
		//m_draw->SetDrawActive(m_activeFlag);
	}

	//���C���΂��ʒu�̒���
	Vec3 RayMark::enemyPos(Vec3 pos) {
		auto enemy = m_enemy.lock();
		if (enemy) {
			Vec3 enemyPos = enemy->GetWorldPos();
			Vec3 forward = enemy->GetComponent<Transform>()->GetForward();
			float face = atan2f(forward.z, forward.x);
			Vec3 linePos;
			linePos.x = (cosf(face) * pos.x) - (sinf(face) * pos.z);
			linePos.y = pos.y;
			linePos.z = (cosf(face) * pos.z) + (sinf(face) * pos.x);
			return enemyPos + linePos;
		}
	}
	bool RayMark::GetActiveFlag() {
		return m_activeFlag;
	}

	void RayMark::Debug() {
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss(L"");
		wss << L"Wall : "
			<< endl;
		scene->SetDebugString(wss.str());

	}
}

//end basecross
