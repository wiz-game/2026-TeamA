#include "stdafx.h"
#include "Project.h"

namespace basecross
{

	void BrokenObjs::OnCreate()
	{

	}

	void BrokenObjs::OnUpdate()
	{

	}

	void BrokenObjs::RemoveItSelf()
	{
		auto stage = GetStage();

		stage->RemoveGameObject<GameObject>(GetThis<GameObject>());
	}

}