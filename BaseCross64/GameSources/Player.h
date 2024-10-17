/*!
@file Player.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Player : public GameObject
	{
	public :
		Player(const shared_ptr<Stage>& stage) :
			GameObject(stage)
		{
		}

	};
}
//end basecross

