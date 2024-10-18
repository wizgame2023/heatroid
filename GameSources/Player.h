/*!
@file Player.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Player : public GameObject {
		//�v���C���[���g�p����R���g���[���ƃL�[�{�[�h�̓���
		Vec2 GetInputState() const;
		// �R���g���[����������x�N�g���𓾂�
		Vec3 GetMoveVector() const;
		//�v���C���[�̈ړ�
		void MovePlayer();
		//������̕\��
		void DrawStrings();
		//���̓n���h���[
		InputHandler<Player> m_InputHandler;
		//�X�s�[�h
		float m_Speed;
		//�W�����v���x
		float m_jumpHeight;

		enum Stats {
			stand,		//�n��
			air,		//��
			hit			//����
		};

		Stats m_stat;

	public:
		//�\�z�Ɣj��

		Player(const shared_ptr<Stage>& StagePtr);

		virtual ~Player() {}
		//�A�N�Z�T
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//virtual void OnUpdate2() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		//virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
		////A�{�^��
		void OnPushA();
	};

//--------------------------------------------------------------------------------------
//	�v���C���[ �ڒn����I�u�W�F�N�g
//--------------------------------------------------------------------------------------
	class LandingCollider : public GameObject
	{
		shared_ptr<Player> m_Player;
	public:
		// �\�z�Ɣj��
		LandingCollider(const shared_ptr<Stage>& stage) :
			GameObject(stage)
		{
		}
		virtual ~LandingCollider()
		{
		}

		virtual void OnCreate() override; // ������
		//virtual void OnUpdate() override; // �X�V
		//virtual void OnDraw() override; // �`��
	};
}
//end basecross

