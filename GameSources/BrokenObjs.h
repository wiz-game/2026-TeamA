#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class BrokenObjs : public GameObject
	{

	public:
		BrokenObjs(const shared_ptr<GameStage>& stage) :
			GameObject(stage)
		{
		}

		void OnCreate()override;
		void OnUpdate()override;

		void RemoveItSelf();
	};
}
