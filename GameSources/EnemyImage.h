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
		float m_ratio;

		float m_test;
		Vec3 m_pos;
		wstring m_meshName;

		vector<VertexPositionNormalTexture> m_normalVertices;
		vector<VertexPositionColorTexture> m_vertices;
		vector<uint16_t> m_indices;
		shared_ptr<PCTStaticDraw> m_draw;
		shared_ptr<Transform> m_trans;
		shared_ptr<MeshResource> m_squareMesh;
		weak_ptr<Enemy> m_enemy;
		Col4 m_color;

		
	public :
		Square(const shared_ptr<Stage>& stage, 
			const float width, const float height, 
			const wstring meshName, 
			const Col4 color = Col4(1.0f),
			const shared_ptr<Enemy>& enemy = nullptr,
			const Vec3 pos = Vec3(0.0f));
		virtual ~Square() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void ThisDestroy();
		Quat RotCorrection(const Vec3& Line);
		void Debug();
	};


	class GaugeSquare : public Square {
	private:
		Vec3 m_enemyPos;
		Vec3 m_enemyScal;

		weak_ptr<Enemy> m_enemy;

	public:
		GaugeSquare(const shared_ptr<Stage>& stage,
			const float width, const float height,
			const wstring meshName,
			const Col4 color = Col4(1.0f),
			const shared_ptr<Enemy>& enemy = nullptr,
			const Vec3 pos = Vec3(0.0f));
		virtual ~GaugeSquare() {};
		virtual void OnUpdate() override;
		void UpdateValue(float ratio);

	};
}