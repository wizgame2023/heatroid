/*!
@file LoadScreen.h
@brief ���[�h����ChildStage�Ƃ��ĕ\��������
@author �؈�D��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class LoadScreen;
	class SpriteLoadBG;
	class SpriteLoad;

	//====================================================================
	// class LoadScreen
	// ���[�h��ʂ��ǂ�
	//====================================================================
	class LoadScreen : public Stage
	{
		//���[�h�I��
		bool m_loadEnd;
		//�o�ߎ���
		float m_time;
		//�i�s�x�������Ǘ����邽�߂�vector
		vector<float> m_progress;
		//���Ԗڂ܂Ői��ł��邩�̃J�E���g
		int m_progCnt = 1;

		shared_ptr<SpriteLoad> m_loadSpr;

	public:
		LoadScreen(): Stage() {}
		virtual ~LoadScreen() {}

		void CreateViewLight();
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
		SpriteLoadBG(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr)
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
		float m_time;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		const float m_width = 256.0f;
		const float m_height = 128.0f;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpriteLoad(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr)
		{}

		~SpriteLoad() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;

		//�����ɊO������i�s�x�̌����ڂ�ς�����֐������
		void UpdateProgress(float load);
	};
}