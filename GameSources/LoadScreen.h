/*!
@file LoadScreen.h
@brief ���[�h����ChildStage�Ƃ��ĕ\��������
@author �؈�D��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//====================================================================
	// class LoadScreen
	// ���[�h���ɕ\������g�����W�V�������
	//====================================================================
	class LoadScreen : public Stage
	{
		//0 = ���[�h�J�n�A1 = ���[�h�I��
		int m_loadState;
	public:
		LoadScreen();
		virtual ~LoadScreen() {}

		virtual void OnCreate()override; //������
		virtual void OnUpdate()override; //������
	};

	//====================================================================
	// class SpriteLoadBG
	// ���[�f�B���O��ʔw�i
	//====================================================================
	class SpriteLoadBG : public GameObject {
		int m_loadState;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpriteLoadBG(const shared_ptr<Stage>& StagePtr, int loadState) :
			GameObject(StagePtr),
			m_loadState(loadState)
		{}

		~SpriteLoadBG() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;
	};

	//====================================================================
	// class SpriteLoad
	// ���[�f�B���O�̕���
	//====================================================================
	class SpriteLoad : public GameObject {
		int m_loadState;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		const float m_width = 256.0f;
		const float m_height = 128.0f;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpriteLoad(const shared_ptr<Stage>& StagePtr, int loadState) :
			GameObject(StagePtr),
			m_loadState(loadState)
		{}

		~SpriteLoad() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;
	};
}