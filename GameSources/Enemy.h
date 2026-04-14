#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class Enemy : public GameObject
	{

	public:
		Enemy(const shared_ptr<GameStage>& stage) :
			GameObject(stage)
		{
		}
	};

}