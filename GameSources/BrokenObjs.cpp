#include "stdafx.h"
#include "Project.h"

namespace basecross
{

	void BrokenObjs::OnCreate()
	{
		AddTag(L"Brakable");
	}

	void BrokenObjs::OnUpdate()
	{
		if (hp <= 0)
		{
			auto stage = GetStage();
			stage->RemoveGameObject<GameObject>(GetThis<GameObject>());
			m_isBroken = true;
		}
	}

	void BrokenObjs::takeDamage()
	{

		if (!m_isBroken)
		{
			hp -= damage;
		}
		else
		{
			return;
		}
	}

}