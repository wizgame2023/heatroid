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
		Vec3 m_scale;
		Vec3 m_rotation;
		Vec3 m_position;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedBox();
		//������
		virtual void OnCreate() override;
		Vec3 GetPositoin();
		Vec3 GetScale();
	};

	//--------------------------------------------------------------------------------------
	//	class BreakWall : public GameObject;
	//--------------------------------------------------------------------------------------
	class BreakWall :public GameObject {

	public:
		BreakWall(const shared_ptr<Stage>& m_stage);
		virtual ~BreakWall();
	};
}
//end basecross
