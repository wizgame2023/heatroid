/*!
@file LoadScreen.h
@brief ���[�h����ChildStage�Ƃ��ĕ\��������
@author �؈�D��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class LoadScreen : public Stage
	{

	public:
		LoadScreen() : Stage() {}
		virtual ~LoadScreen() {}

		virtual void OnCreate()override; //������
		virtual void OnUpdate()override; //������
	};
}