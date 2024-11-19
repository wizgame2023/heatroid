/*!
@file EmenyImage.h
@brief ìGÇÃÉQÅ[ÉW
íSìñÅFàÌå©
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Square : public GameObject{

	protected:
		float m_width;
		float m_height;
		float m_moveX;
		float m_moveY;
		float m_ratio;

		float m_test;
		Vec3 m_pos;

		vector<VertexPositionNormalTexture> m_normalVertices;
		vector<VertexPositionColorTexture> m_vertices;
		vector<uint16_t> m_indices;
		shared_ptr<PCTStaticDraw> m_draw;
		shared_ptr<Transform> m_trans;
		shared_ptr<MeshResource> m_squareMesh;

		Col4 m_color;

		
	public :
		Square(const shared_ptr<Stage>& stage);
		virtual ~Square() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void ThisDestroy();
		void Debug();
		Quat RotCorrection(const Vec3& Line);
	};

	class GaugeSquare : public Square {
	private:
		Vec3 m_enemyPos;
		Vec3 m_enemyScal;

		weak_ptr<Enemy> m_enemy;

	public:
		GaugeSquare(const shared_ptr<Stage>& stage, const shared_ptr<Enemy>& enemy);
		virtual ~GaugeSquare() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void UpdateValue(float ratio);

	};
}