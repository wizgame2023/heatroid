/*!
@file Player.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Player : public GameObject {
	public:
		//�\�z�Ɣj��

		Player(const shared_ptr<Stage>& StagePtr);

		virtual ~Player() {}
		//�A�N�Z�T
		//������
		virtual void OnCreate() override;
		//�X�V
		//virtual void OnUpdate() override;
		//virtual void OnUpdate2() override;
		//virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		//virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
		////A�{�^��
		//void OnPushA();
	};

}
//end basecross

