/*!
@file LoadScreen.h
@brief ƒ[ƒh’†‚ÉChildStage‚Æ‚µ‚Ä•\¦‚·‚é‰æ–Ê
@author ’Øˆä—D–í
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class LoadScreen : public Stage
	{

	public:
		LoadScreen() : Stage() {}
		virtual ~LoadScreen() {}

		virtual void OnCreate()override; //‰Šú‰»
		virtual void OnUpdate()override; //‰Šú‰»
	};
}