/*!
@file FixedBox.h
@brief ����u���b�N�Ȃ�
�S���F�팩
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale
		);
		virtual ~FixedBox(){}
		//������
		virtual void OnCreate() override;
		Vec3 GetPositoin();
		Vec3 GetScale();
	};

	//--------------------------------------------------------------------------------------
	//	class BreakWall : public GameObject;
	//--------------------------------------------------------------------------------------
	class BreakWall :public GameObject {
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_scal;
	public:
		BreakWall(const shared_ptr<Stage>& m_stage,
			const Vec3& position,
			const Vec3& rotation,
			const Vec3& scale);
		virtual ~BreakWall(){}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//virtual void OnCollisionEnter(shared_ptr<GameObject>& other) override;
		void ThisDestory();

	};
}
//end basecross
