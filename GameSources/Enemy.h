#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class Enemy : public BrokenObjs
	{

		shared_ptr<Transform> m_transComp;
		shared_ptr<PNTStaticDraw> m_drawComp;
		void InitTransComp();
		void InitDrawComp();
		int hp;

		int damage;
		weak_ptr<Player> m_player;
		Vec3 enemyScale;
		Vec3 enemyPos;
		Vec3 playerScale;
		Vec3 playerPos;

		void SetValue();
		void OnCollisionEnter(shared_ptr<GameObject>& other)override;

		bool isHit;
		float timer;

		float rx;
		float rz;
		Vec3 dir;
	public:
		Enemy(const shared_ptr<Stage>& stage) :
			BrokenObjs(stage),
			hp(4)
		{
		}

		void OnCreate()override;
		void OnUpdate()override;


	};

}