/*!
@file Timer.h
@brief タイマーなど
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
		// 構築と破棄
		Timer(const shared_ptr<Stage>& stage, const Vec3 pos);
		virtual ~Timer(){}

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		float GetTime();

		void Debug();
	};

}
//end basecross
