#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class BrokenObjs : public GameObject
	{
		bool m_isBroken;
		int hp;
		int damage;

	public:
		BrokenObjs(const shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_isBroken(false)
		{
		}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		virtual void takeDamage();

		virtual void SetHP(int newHP)
		{
			hp = newHP;
		}

		int GetHP()
		{
			return hp;
		}

		virtual void SetDamage(int newDamage)
		{
			damage = newDamage;
		}
	};
}
