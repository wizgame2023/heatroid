/*!
@file Raycast.h
@brief レイを飛ばす
担当：逸見
*/

#pragma once
#include "stdafx.h"
#include "Player.h"
#include "Enemy.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class LineObject : public GameObject; //線を描画するオブジェクト
	//--------------------------------------------------------------------------------------
	class LineObject : public GameObject {
	private:
		Vec3 m_startPos;
		Vec3 m_endPos;
		Vec3 m_direc;

		Col4 m_startColor;
		Col4 m_endColor;

		vector<VertexPositionColor> m_vertices;
		vector<uint16_t> m_indices;
		shared_ptr<PCStaticDraw> m_draw;

		weak_ptr<Player> m_player;
		weak_ptr<Enemy> m_enemy;

	public:
		LineObject(const shared_ptr<Stage>& stage);
		LineObject(const shared_ptr<Stage>& stage, const shared_ptr<Player>& player,const shared_ptr<Enemy>& enemy);
		virtual ~LineObject() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void SetLinePosition(const Vec3& startPos, const Vec3& endPos);
		void SetLineColor(const Col4& startColor, const Col4& endColor);
		void VerticesUpdate();
		Vec3 GetStartPos();
		Vec3 GetEndPos();
		Vec3 GetDirec();
		Vec3 LinePos(Vec3 pos);
	};

	//--------------------------------------------------------------------------------------
	//	class RayMark : public GameObject; //オブジェクトに触れているかどうか
	//--------------------------------------------------------------------------------------
	class RayMark : public GameObject {
	private : 
		bool m_hitEnemyFlag;
		bool m_activeFlag;
		int m_wallCnt;
		int m_doorCnt;

		vector<bool> m_hitWallFlag;
		vector<bool> m_hitDoorFlag;
		shared_ptr<Transform> m_trans;
		shared_ptr<PNTStaticDraw> m_draw;
		weak_ptr<Player> m_player;
		weak_ptr<Enemy> m_enemy;


	public:
		RayMark(const shared_ptr<Stage>& stage);
		RayMark(const shared_ptr<Stage>& stage, const shared_ptr <Player>& player,const shared_ptr<Enemy>& enemy);
		virtual ~RayMark() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void Debug();
		Vec3 enemyPos(Vec3 pos);
		bool GetActiveFlag();

	};


}