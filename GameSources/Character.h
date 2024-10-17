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

}
//end basecross
