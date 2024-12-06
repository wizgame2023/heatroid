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
	class SpriteLoadCircle;
	class SpriteLoadFade;

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

		const float loadLength = .05f;

		shared_ptr<SpriteLoad> m_loadSpr;
		shared_ptr<SpriteLoadFade> m_fade;

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
	// class SpriteLoadCircle
	// ���[�f�B���O��ʂ̂��邮�邷����
	//====================================================================
	class SpriteLoadCircle : public GameObject {
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		const float m_rotateSpeed = -360.0f;
		const float m_radius = 50.0f;
		float m_rotate = 0.0f;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpriteLoadCircle(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr)
		{}

		~SpriteLoadCircle() {}

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

	//====================================================================
	// class SpriteLoadFade
	// ���[�f�B���O��ʂ̉�ʑJ��
	//====================================================================
	class SpriteLoadFade : public GameObject {
		bool m_loadState = false;
		float m_fade = 1.0f;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;

		const float m_fadeSpeed = 2.0f;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();
	public:
		SpriteLoadFade(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr)
		{}

		~SpriteLoadFade() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void SetLoadState(int state);
	};
}