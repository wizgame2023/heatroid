/*!
@file SelectSprite.cpp
@brief セレクトステージのUI実体
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	SelectSprite::SelectSprite(const shared_ptr<Stage>& stage
	) :
		GameObject(stage),
		m_widthNum(1),
		m_heightNum(6),
		m_wSize(80*10.0f),//800
		m_hSize(15*10.0f),//150
		m_wSpace(420),
		m_hSpace(-30),
		m_widthSpace(m_wSpace + m_wSize),
		m_heightSpace(m_hSpace + m_hSize),
		m_selectNum(1),
		m_keepTime(0.3f),
		m_maxKeepTime(m_keepTime),
		m_coolTime(0.3f),
		m_maxCoolTime(m_coolTime),
		m_pos(Vec3(400.0f, -600.0f, 0.0f)),//Vec3(400.0f, -500.0f, 0.0f)
		m_maxPos(0.0f),
		m_minPos(0.0f),
		m_sCheck{ false },
		m_nextFlag(false),
		m_coolTimeFlag(false),
		m_select(false)
	{}
	void SelectSprite::OnCreate() {
		auto stage = GetStage();
		PosSetting();
		m_maxPos = Vec2(m_pos.x + (m_widthNum - 1) * -m_widthSpace,
			m_pos.y + (m_heightNum - 1) * m_heightSpace);
		m_minPos = Vec2(m_pos.x, m_pos.y);
		m_trans = GetComponent<Transform>();
		for (int i = 0; i < m_widthNum; i++) {
			for (int j = 0; j < m_heightNum; j++) {
				int sNum = i + j * m_widthNum;
				m_sprite[sNum] = stage->AddGameObject<GameSprite>(
					m_wSize, m_hSize, L"SelectFram", Vec3(m_pos.x + i * -m_widthSpace, m_pos.y + j * m_heightSpace, 0.0), Col4(1.0f));
				if (sNum < m_heightNum * m_widthNum-1) {
					m_sText[sNum] = stage->AddGameObject<GameSprite>(800, 150, L"SelectText",
						Vec3(m_pos.x + i * -m_widthSpace, m_pos.y + (j+1) * m_heightSpace, 0.0), Col4(1.0f));
					m_sText[sNum]->SetDrawLayer(3);
					m_sNum[sNum] = stage->AddGameObject<NumberSprite>
						(j + i * m_heightNum + 1, Vec3(m_pos.x + i * -m_widthSpace + 60, 
							(m_pos.y + (j+1) * m_heightSpace) + 70, 0.0f));
					m_sNum[sNum]->SetDrawLayer(3);
				}
			}
		}
		m_selectSprite = stage->AddGameObject<GameSprite>
			(m_wSize, m_hSize, L"SelectFram", Vec3(m_pos), Col4(0.0f, 1.0f, 1.0f, 0.5f));
		m_selectSprite->SetDrawLayer(2);
		m_selectPos = Vec3(m_pos.x, m_pos.y+m_heightSpace, 0.0f);
		auto TitleText = stage->AddGameObject<GameSprite>(800, 150, L"SelectTitle", Vec3(m_pos.x+70,m_pos.y,m_pos.z));
		TitleText->SetDrawLayer(3);
		
	}
	void SelectSprite::OnUpdate() {
		auto selectTrans = m_selectSprite->GetComponent<Transform>();
		Decision();
		StickSelect();
		//KeepStick();
		selectTrans->SetPosition(m_selectPos);
		//m_sprite[m_selectNum+1]->SetColor(Col4(0.0f, 1.0f, 1.0f, 1.0f));
		Debug();
	}
	//座標を真ん中にする計算
	void SelectSprite::PosSetting() {
		//横
		if (m_widthNum % 2 == 0) {
			m_pos.x = m_pos.x - (m_wSize * (m_widthNum / 2 - 1) + m_wSpace * (m_widthNum / 2 - 1) + m_wSize / 2 + m_wSpace / 2);
		}
		else {
			m_pos.x = m_pos.x + (m_wSize * (m_widthNum / 2) + m_wSpace * (m_widthNum / 2));
		}
		//縦
		if (m_heightNum % 2 == 0) {
			m_pos.y = m_pos.y + (m_hSize * (m_heightNum / 2 - 1) + m_hSpace * (m_heightNum / 2 - 1) + m_hSize / 2 + m_hSpace / 2);
		}
		else {
			m_pos.y = m_pos.y + (m_hSize * (m_heightNum / 2) + m_hSpace * (m_heightNum / 2));
		}

	}
	void SelectSprite::StickSelect() {
		auto cntVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keyState = App::GetApp()->GetInputDevice().GetKeyState();
		CoolTimeStick();
		if (m_select) return;
		//上
		if (cntVec[0].fThumbLY > 0.7f || keyState.m_bPressedKeyTbl[VK_UP]) {
			if ((!m_sCheck[0] || m_nextFlag) &&
				m_maxPos.y > m_selectPos.y) {
				m_selectPos.y += m_heightSpace;
				m_sCheck[0] = true;
				m_selectNum++;
			}
		}
		else if ((cntVec[0].fThumbLY < 0.7f || keyState.m_bLastKeyTbl[VK_UP])) {
			m_sCheck[0] = false;
		}
		//下
		if (cntVec[0].fThumbLY < -0.7f || keyState.m_bPressedKeyTbl[VK_DOWN]) {
			if ((!m_sCheck[2] || m_nextFlag)&&
				m_minPos.y < m_selectPos.y) {
				m_selectPos.y -= m_heightSpace;
				m_sCheck[2] = true;
				m_selectNum--;
			}
		}
		else if (cntVec[0].fThumbLY > -0.7f || keyState.m_bLastKeyTbl[VK_DOWN]) {
			m_sCheck[2] = false;
		}

		if (cntVec[0].fThumbLX > 0.7f || keyState.m_bPressedKeyTbl[VK_RIGHT]) {
			if ((!m_sCheck[1] || m_nextFlag) &&
				m_minPos.x > m_selectPos.x) {
				m_selectPos.x += m_widthSpace;
				m_sCheck[1] = true;
				m_selectNum -= m_heightNum;
			}
		}
		else if (cntVec[0].fThumbLX < 0.7f || keyState.m_bLastKeyTbl[VK_RIGHT]) {
			m_sCheck[1] = false;
		}
		//左
		if (cntVec[0].fThumbLX < -0.7f || keyState.m_bPressedKeyTbl[VK_LEFT]) {
			if ((!m_sCheck[3] || m_nextFlag) &&
				m_maxPos.x < m_selectPos.x) {
				m_selectPos.x -= m_widthSpace;
				m_sCheck[3] = true;
				m_selectNum += m_heightNum;
			}
		}
		else if (cntVec[0].fThumbLX > -0.7f || keyState.m_bLastKeyTbl[VK_LEFT]) {
			m_sCheck[3] = false;
		}
	}
	void SelectSprite::KeepStick() {
		float elapsed = App::GetApp()->GetElapsedTime();
		//if (m_sCheck[0] || m_sCheck[1] || m_sCheck[2] || m_sCheck[3]) {
		//	m_keepTime -= elapsed;
		//}
		for (int i = 0; i < 4; i++) {
			if (m_sCheck[i]) {
				m_keepTime -= elapsed;
				m_keepNum = i;
			}
		}
		if (m_keepTime <= 0.0f) {
			m_nextFlag = true;
			m_keepTime = m_maxKeepTime;
			if (!m_sCheck[m_keepNum]) {
				//m_keepTime = m_maxKeepTime;
			}
		}
		else {
			m_nextFlag = false;
		}
	}
	void SelectSprite::CoolTimeStick() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		for (int i = 0; i < 4; i++) {
			if (m_sCheck[i]) {
				m_sum++;
				m_coolTimeFlag = true;
				if (m_sum > 1) {
					if (m_coolTimeFlag) {
						m_coolTime -= elapsed;
					}
				}
			}
		}
		if (m_coolTime <= 0.0f) {
			m_coolTime = m_maxCoolTime;
			m_coolTimeFlag = false;
		}
	}
	void SelectSprite::Decision() {
		//auto cntVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//auto keyState = App::GetApp()->GetInputDevice().GetKeyState();
		//if (cntVec[0].wPressedButtons && XINPUT_GAMEPAD_A || keyState.m_bPressedKeyTbl[VK_SPACE]) {
		//	m_select = true;
		//}
	}
	int SelectSprite::GetSelectNum() {
		return m_selectNum;
	}
	void SelectSprite::Debug() {
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss(L"");
		wss << L"selectPos.y : "
			<< m_selectPos.y
			<< L"\nnextFlag : "
			<< m_nextFlag
			<< L"\nUp : "
			<< m_sCheck[0]
			<< L"\nKeepTime : "
			<< m_keepTime
			<< L"\nselectNum : "
			<< m_selectNum
			<<L"\n"
			<< m_maxPos.y
			<< L"\n"
			<<m_selectPos.x
			<< endl;
		scene->SetDebugString(wss.str());

	}

}
//end basecross
