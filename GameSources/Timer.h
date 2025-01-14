/*!
@file Timer.h
@brief �^�C�}�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//--------------------------------------------------------------------------------------
	//	class Timer : public GameObject;
	//--------------------------------------------------------------------------------------
	class Timer : public GameObject
	{
	private:
		float m_time;
		int m_timeNum[4];
		Vec3 m_pos;
		shared_ptr<NumberSprite> m_numSprite[4];
	public:
		// �\�z�Ɣj��
		Timer(const shared_ptr<Stage>& stage, const Vec3 pos);
		virtual ~Timer(){}

		virtual void OnCreate() override; // ������
		virtual void OnUpdate() override; // �X�V
		float GetTime();

		void Debug();
	};

}
//end basecross
