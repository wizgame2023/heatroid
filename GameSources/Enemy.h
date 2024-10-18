/*!
@file Enemy.h
@brief “G‚È‚Ç
’S“–FˆíŒ©
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	class Enemy : public GameObject;
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject
	{
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;

		shared_ptr<Transform> m_trans;

	public:
		// \’z‚Æ”jŠü
		Enemy(const shared_ptr<Stage>& stage);
		Enemy(const shared_ptr<Stage>& stage, 
			const Vec3& position, 
			const Vec3& rotatoin, 
			const Vec3& scale);
		virtual ~Enemy(){}
		virtual void OnCreate() override; // ‰Šú‰»
		//virtual void OnUpdate() override; // XV
		//virtual void OnDraw() override; // •`‰æ
	};


}