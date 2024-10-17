/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class Foo : public GameObject;
	//--------------------------------------------------------------------------------------
	class Foo : public GameObject
	{
	public:
		// �\�z�Ɣj��
		Foo(const shared_ptr<Stage>& stage) :
			GameObject(stage)
		{
		}
		virtual ~Foo()
		{
		}

		virtual void OnCreate() override; // ������
		//virtual void OnUpdate() override; // �X�V
		//virtual void OnDraw() override; // �`��
	};

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
		//����
	};

}
//end basecross
