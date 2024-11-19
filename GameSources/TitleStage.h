/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class TitleStage : public Stage
	{
		shared_ptr<SoundItem> m_BGM;
		std::shared_ptr<basecross::XAudio2Manager> m_ptrXA = App::GetApp()->GetXAudio2Manager();
		void CreateViewLight(); //ビューの作成

	public:
		TitleStage() : Stage() {}
		virtual ~TitleStage() {}

		virtual void OnCreate()override; //初期化
		virtual void OnUpdate()override; //初期化
		void OnDestroy();
		void OnPushA();
		void OnDraw();
		void OnTitleSprite();
		void PlayBGM(const wstring& StageBGM);
	};

	class Draw : public GameObject
	{
		Vec3 m_scale;
		Vec3 m_position;
	public:
		Draw(const shared_ptr<Stage>& stage,
			const Vec3& Scale,
			const Vec3& Position
		);
		~Draw() {};
		virtual void OnCreate()override; //初期化
	};

}