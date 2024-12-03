/*!
@file SelectSprite.h
@brief　セレクトステージのUI など
担当：逸見
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	class SelectSprite : public GameObject {
	private:
		int m_widthNum;
		int m_heightNum;
		float m_wSize;
		float m_hSize;
		float m_wSpace;
		float m_hSpace;
		float m_widthSpace;
		float m_heightSpace;
		float m_keepTime;
		float m_maxKeepTime;
		float m_coolTime;
		float m_maxCoolTime;
		float m_sum;
		int m_selectNum;
		int m_keepNum;

		bool m_sCheck[4];
		bool m_nextFlag;
		bool m_coolTimeFlag;
		bool m_select;
		Vec3 m_pos;
		Vec3 m_selectPos;
		Vec2 m_maxPos;
		Vec2 m_minPos;
		shared_ptr<GameSprite> m_sprite[20];
		shared_ptr<GameSprite> m_selectSprite;
		shared_ptr<Transform> m_trans;
		shared_ptr<GameSprite> m_sText[20];
		shared_ptr<NumberSprite> m_sNum[20];
	public:
		SelectSprite(const shared_ptr<Stage>& stage);
		virtual ~SelectSprite() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void StickSelect();
		void KeepStick();
		void CoolTimeStick();
		void PosSetting();
		void Decision();
		void Debug();
		int GetSelectNum();

	};

}
//end basecross
