/*!
@file GameStageSprite.h
@brief �Q�[���X�e�[�W�̃X�v���C�g�Ȃǎ���
�S���F�팩
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class GameSprite : public GameObject {
	private:
		//�\���T�C�Y
		float m_width;  //���̃T�C�Y
		float m_height; //�c�̃T�C�Y
		int m_layer;   //�\�����C���[
		Vec3 m_pos;
		wstring m_meshName;
		Col4 m_color;

		vector<VertexPositionColorTexture> m_vertices;
		vector<uint16_t> m_indices;
		shared_ptr<PCTSpriteDraw> m_draw;
		shared_ptr<Transform> m_trans;

	public:
		GameSprite(const shared_ptr<Stage>& stage,
			float width, float heigth,
			wstring meshName, Vec3 pos,
			Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f),
			int layer = 1
		);
		virtual ~GameSprite() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void SetColor(Col4 color);
	};
	class FadeOut : public GameObject {
	private: 
		float m_time;
		float m_touka;
		shared_ptr<GameSprite> m_sprite;
	public:
		FadeOut(const shared_ptr<Stage>& stage);
		virtual ~FadeOut() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		bool GetFadeOutEnd();
		void SetColor(Col4 color);
	};
	class FadeIn : public GameObject {
	private:
		float m_time;
		shared_ptr<GameSprite> m_sprite;
	public:
		FadeIn(const shared_ptr<Stage>& stage);
		virtual ~FadeIn() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	};
	//�S�[�����̃t�F�[�h�A�E�g/�t�F�[�h�C��(Player����Ăяo���H)
	class GoalFade : public GameObject {
	private:
		float m_time;
		const float m_fadeinTime = 1.5f;
		const float m_endTime = m_fadeinTime + 1.0f;
		shared_ptr<GameSprite> m_sprite;
	public:
		GoalFade(const shared_ptr<Stage>& stage);
		virtual ~GoalFade() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	};
	class GameOverSprite :public GameObject {
	private:
		shared_ptr<FadeOut> m_fadeOut;
		shared_ptr<GameSprite> m_textSprite;
		shared_ptr<GameSprite> m_backEffSprite;
	public:
		GameOverSprite(const shared_ptr<Stage>& stage);
		virtual ~GameOverSprite() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	class NumberSprite :public GameObject {
	private:
		int m_number;
		float m_width;
		float m_height;
		float m_moveW;
		float m_moveH;
		Vec3 m_pos;
		Col4 m_color;
		wstring m_meshName;

		vector<VertexPositionColorTexture> m_vertices;
		vector<uint16_t> m_indices;
		shared_ptr<PCTSpriteDraw> m_draw;
		shared_ptr<Transform> m_trans;

	public:
		NumberSprite(const shared_ptr<Stage>& stage,const int& number,const Vec3& pos);
		virtual ~NumberSprite() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void UpdateVar(int num);
	};
}