/*!
@file Timer.cpp
@brief タイマーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	//--------------------------------------------------------------------------------------
	//	class Timer : public GameObject;
	//--------------------------------------------------------------------------------------
	Timer::Timer(const shared_ptr<Stage>& stage,
		const Vec3 pos
	):
		GameObject(stage),
		m_pos(pos),
		m_time(0.0f),
		m_timeNum{0,0,0,0}
	{}
	void Timer::OnCreate()
	{
		m_numSprite[0] = GetStage()->AddGameObject<NumberSprite>(m_time, m_pos);
		m_numSprite[1] = GetStage()->AddGameObject<NumberSprite>(m_time, Vec3(m_pos.x + 45, m_pos.y, m_pos.z));
		m_numSprite[2] = GetStage()->AddGameObject<NumberSprite>(m_time, Vec3(m_pos.x + 110, m_pos.y, m_pos.z));
		m_numSprite[3] = GetStage()->AddGameObject<NumberSprite>(m_time, Vec3(m_pos.x + 155, m_pos.y, m_pos.z));

	}
	void Timer::OnUpdate() {
		float elapsed = App::GetApp()->GetElapsedTime();
		auto stage = GetStage();
		m_time += elapsed;

		int minutes = m_time / 60;
		int seconds = (int)m_time % 60;
		m_timeNum[0] = (minutes / 10) % 10;
		m_timeNum[1] = minutes % 10;
		m_timeNum[2] = (seconds / 10) % 10;
		m_timeNum[3] = seconds % 10;

		m_numSprite[0]->UpdateVar(m_timeNum[0] + 9);
		m_numSprite[1]->UpdateVar(m_timeNum[1] + 9);
		m_numSprite[2]->UpdateVar(m_timeNum[2] + 9);
		m_numSprite[3]->UpdateVar(m_timeNum[3] + 9);

		//1時間たった場合
		if (m_time > 3600) {
			m_numSprite[0]->UpdateVar(0);
			m_numSprite[1]->UpdateVar(0);
			m_numSprite[2]->UpdateVar(0);
			m_numSprite[3]->UpdateVar(0);
		}
		Debug();

	}
	void Timer::Debug() {
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss(L"");
		wss << m_time
			<< endl;
		scene->SetDebugString(wss.str());

	}
	float Timer::GetTime() {
		return m_time;
	}
}
//end basecross
