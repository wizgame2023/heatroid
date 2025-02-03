/*!
@file Raycast.cpp
@brief レイを飛ばす
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class LineObject : public GameObject; //線を描画するオブジェクト
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
		//線を構成する2点
		m_vertices = {
			{m_startPos, m_startColor},
			{m_endPos, m_endColor}
		};
		//始点と終点をつなぐインデックス
		m_indices = {
			0,1
		};

		//描画
		m_draw = AddComponent<PCStaticDraw>(); //位置と色のみ
		m_draw->SetOriginalMeshUse(true); //自作したメッシュを使用
		m_draw->CreateOriginalMesh(m_vertices, m_indices); //メッシュの作成
		auto meshResoure = m_draw->GetMeshResource(); //メッシュリソースを取得し、プリミティブポロジー（頂点利用方法）を変更する
		meshResoure->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP); //ポリゴンではなく稜線を表示
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

	//頂点の更新
	void LineObject::VerticesUpdate() {
		m_vertices = {
			{m_startPos, m_startColor},
			{m_endPos, m_endColor}
		};

		m_draw->UpdateVertices(m_vertices);
	}

	//頂点の設定
	void LineObject::SetLinePosition(const Vec3& startPos, const Vec3& endPos) {
		m_startPos = startPos;
		m_endPos = endPos;
		m_direc = endPos - startPos;
		m_direc = m_direc.normalize();

		VerticesUpdate();
	}

	//線の色の設定
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
	//	class RayMark : public GameObject; //衝突したときの表示されるオブジェクト
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
		auto enemy = m_enemy.lock();
		if (!enemy) return;
		auto enemyDraw = enemy->GetComponent<PNTBoneModelDraw>();
		rayEnd = enemyPos(Vec3(0.0f,3.0f,0.0f));		             //レイを飛ばす終点
		//当たっているかのフラグ
		m_hitEnemyFlag = enemyDraw->HitTestStaticMeshSegmentTriangles
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
		enemy->SetActiveFlag(m_activeFlag);

		//デバック用
		//当たっている場所の描画
		if (m_hitEnemyFlag) {
			pos = enemyCrossPos;
		}
		m_trans->SetPosition(pos);
		//m_draw->SetDrawActive(m_activeFlag);
	}

	//レイを飛ばす位置の調整
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
