/*!
@file FixedBox.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vec3 m_scale;
		Vec3 m_rotation;
		Vec3 m_position;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	class NoneBox : public GameObject {
		Vec3 m_scale;
		Vec3 m_rotation;
		Vec3 m_position;
	public:
		//�\�z�Ɣj��
		NoneBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~NoneBox();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
//�@�^�C�����O����Œ�̃{�b�N�X
//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//�\�z�Ɣj��
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};

}
//end basecross
