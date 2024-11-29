/*!
@file GameStageSprite.cpp
@brief ゲームステージのスプライトなど実体
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	GameSprite::GameSprite(const shared_ptr<Stage>& stage,
		float width, float heigth,
		wstring meshName, Vec3 pos,
		Col4 color
	):
		GameObject(stage),
		m_width(width),
		m_height(heigth),
		m_meshName(meshName),
		m_pos(pos),
		m_color(color),
		m_time(1.0f),
		m_layer(1)
	{}
	void GameSprite::OnCreate() {
		m_vertices = {
		{Vec3(-m_width * 0.5f,m_height * 0.5f,0),m_color,Vec2(0.0f,0.0f)},
		{Vec3(m_width * 0.5f,m_height * 0.5f,0),m_color,Vec2(1.0f,0.0f)},
		{Vec3(-m_width * 0.5f,-m_height * 0.5f,0),m_color,Vec2(0.0f,1.0f)},
		{Vec3(m_width * 0.5f,-m_height * 0.5f,0),m_color,Vec2(1.0f,1.0f)}
		};
		m_indices = {
			0,1,2,
			2,1,3
		};
		m_draw = AddComponent<PCTSpriteDraw>(m_vertices, m_indices);
		m_draw->SetTextureResource(m_meshName);
		m_draw->SetSamplerState(SamplerState::LinearWrap);
		m_draw->SetDiffuse(Col4(m_color));
		SetAlphaActive(true);
		m_draw->SetDrawActive(true);

		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);

		SetDrawLayer(m_layer);
	}
	void GameSprite::OnUpdate() {

	}
	void GameSprite::SetColor(Col4 color) {
		m_draw->SetDiffuse(color);
	}
	FadeOut::FadeOut(const shared_ptr<Stage>& stage
	):
		GameObject(stage),
		m_time(0.0f),
		m_touka(0.9f)
	{}
	void FadeOut::OnCreate() {
		auto stage = GetStage();
		m_sprite = stage->AddGameObject<GameSprite>(1280, 800,
			L"White", Vec3(0.0f), Col4(0.0f, 0.0f, 0.0f, 1.0f));

	}
	void FadeOut::OnUpdate() {
		float elpased = App::GetApp()->GetElapsedTime();
		m_time += elpased * 0.5f;
		if (m_time >= m_touka) {
			m_time = m_touka;
		}
		m_sprite->SetColor(Col4(0.0f, 0.0f, 0.0f, m_time));
	}
	bool FadeOut::GetFadeOutEnd() {
		if (m_time >= m_touka) {
			return true;
		}
		else {
			return false;
		}
	}
	void FadeOut::SetColor(Col4 color) {
		m_sprite->SetColor(color);
	}

	FadeIn::FadeIn(const shared_ptr<Stage>& stage
	) :
		GameObject(stage),
		m_time(1.0f)
	{}
	void FadeIn::OnCreate() {
		auto stage = GetStage();
		m_sprite = stage->AddGameObject<GameSprite>(1280, 800,
			L"White", Vec3(0.0f), Col4(1.0f, 1.0f, 1.0f, 1.0f));

	}
	void FadeIn::OnUpdate() {
		float elpased = App::GetApp()->GetElapsedTime();
		m_time -= elpased * 0.5f;
		if (m_time <=0.0f) {
			m_time = 0.0f;
		}
		m_sprite->SetColor(Col4(1.0f, 1.0f, 1.0f, m_time));
	}

	GameOverSprite::GameOverSprite(const shared_ptr<Stage>& stage
	):
		GameObject(stage)
	{}
	void GameOverSprite::OnCreate() {
		auto stage = GetStage();
		m_fadeOut = stage->AddGameObject<FadeOut>();
		m_fadeOut->SetDrawLayer(1);
		m_textSprite = stage->AddGameObject<GameSprite>(1280,800,L"GameOverText",Vec3(0.0f));
		m_textSprite->SetDrawActive(false);
		m_backEffSprite = stage->AddGameObject<GameSprite>(1280, 800, L"GameOverBackEffect", Vec3(0.0f));
		m_backEffSprite->SetDrawActive(false);
		m_backEffSprite->SetColor(Col4(1.0f, 1.0f, 1.0f, 0.5f));
		m_backEffSprite->SetDrawLayer(1);
		
	}
	void GameOverSprite::OnUpdate() {
		if (m_fadeOut->GetFadeOutEnd()) {
			m_textSprite->SetDrawActive(true);
			m_backEffSprite->SetDrawActive(true);
			m_textSprite->SetDrawLayer(2);
			m_fadeOut->SetColor(Col4(0.0f, 0.0f, 0.0f, 0.6f));
		}
	}

	SelectSprite::SelectSprite(const shared_ptr<Stage>&stage
	):
		GameObject(stage),
		m_widthNum(2),
		m_heightNum(6),
		m_wSize(50),
		m_hSize(50),
		m_wSpace(20),
		m_hSpace(20),
		m_widthSpace(m_wSpace+m_wSize),
		m_heightSpace(m_hSpace+m_hSize),
		m_selectNum(1),
		m_keepTime(0.3f),
		m_maxKeepTime(m_keepTime),
		m_coolTime(0.3f),
		m_maxCoolTime(m_coolTime),
		m_pos(Vec3(0.0f,0.0f,0.0f)),
		m_maxPos(0.0f),
		m_minPos(0.0f),
		m_sCheck{false},
		m_nextFlag(false),
		m_coolTimeFlag(false)
	{}
	void SelectSprite::OnCreate() {
		auto stage = GetStage();
		PosSetting();
		m_maxPos = Vec2(m_pos.x,m_pos.y);
		m_minPos = Vec2(m_pos.x + (m_widthNum - 1) * m_widthSpace,
			m_pos.y - (m_heightNum - 1) * m_heightSpace);
		m_trans = GetComponent<Transform>();
		for (int j = 0; j < m_heightNum; j++) {
			for (int i = 0; i < m_widthNum; i++) {
				m_sprite[i + j * m_widthNum] = stage->AddGameObject<GameSprite>(
					m_wSize, m_hSize, L"White", Vec3(m_pos.x + i * m_widthSpace, m_pos.y + j * -m_heightSpace, 0.0), Col4(1.0f));
			}
		}
		stage->AddGameObject<GameSprite>(20, 20, L"White", Vec3(0.0f), Col4(0.0f, 0.0f, 1.0f, 1.0f));
		m_selectSprite = stage->AddGameObject<GameSprite>(m_wSize*1.2f, m_hSize*1.2f, L"White", Vec3(m_pos), Col4(0.0f, 1.0f, 1.0f, 1.0f));
		m_selectSprite->SetDrawLayer(-1);
		m_selectPos = m_pos;
	}
	void SelectSprite::OnUpdate() {
		auto selectTrans = m_selectSprite->GetComponent<Transform>();
		
		StickSelect();
		KeepStick();
		selectTrans->SetPosition(m_selectPos);
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
		//if (m_coolTimeFlag&&m_nextFlag==false) return;
		//上
		if (cntVec[0].fThumbLY > 0.7f || keyState.m_bPressedKeyTbl[VK_UP]) {
			if ((!m_sCheck[0] ||m_nextFlag) &&
				m_maxPos.y > m_selectPos.y) {
				m_selectPos.y += m_heightSpace;
				m_sCheck[0] = true;
				m_selectNum--;
			}
		}
		else if ((cntVec[0].fThumbLY < 0.7f||keyState.m_bLastKeyTbl[VK_UP])) {
			m_sCheck[0] = false;
		}
		//下
		if (cntVec[0].fThumbLY < -0.7f || keyState.m_bPressedKeyTbl[VK_DOWN]) {
			if ((!m_sCheck[2] || m_nextFlag) &&
				m_minPos.y < m_selectPos.y) {
				m_selectPos.y -= m_heightSpace;
				m_sCheck[2] = true;
				m_selectNum++;
			}
		}
		else if (cntVec[0].fThumbLY > -0.7f || keyState.m_bLastKeyTbl[VK_DOWN]) {
			m_sCheck[2] = false;
		}
		if (cntVec[0].fThumbLX > 0.7f || keyState.m_bPressedKeyTbl[VK_RIGHT]) {
			if ((!m_sCheck[1] || m_nextFlag) &&
				m_minPos.x > m_selectPos.x) {
				m_selectPos.x += m_heightSpace;
				m_sCheck[1] = true;
				m_selectNum += m_heightNum;
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
				m_selectNum -= m_heightNum;
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
	void SelectSprite::Debug() {
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss(L"");
		wss << L"selectPos.y : "
			<<m_selectPos.y
			<<L"\nnextFlag : "
			<<m_nextFlag
			<<L"\nUp : "
			<< m_sCheck[0]
			<<L"\nKeepTime : "
			<< m_keepTime
			<<L"\n"
			<< m_selectNum
			<< endl;
		scene->SetDebugString(wss.str());

	}
}

