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

		weak_ptr<Player> m_player;
		Vec3 enemyScale;
		Vec3 enemyPos;
		Vec3 playerScale;
		Vec3 playerPos;

		void OnCollisionEnter(shared_ptr<GameObject>& other)override;

		bool isHit;
		float timer;
	public:
		Enemy(const shared_ptr<GameStage>& stage) :
			BrokenObjs(stage),
			hp(4)
		{
		}

		void OnCreate()override;
		void OnUpdate()override;


	};

}