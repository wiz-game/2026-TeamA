#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class Wall : public BrokenObjs
	{
		shared_ptr<Transform> m_transComp;
		shared_ptr<PNTStaticDraw> m_drawComp;

		void InitTransComp();
		void InitDrawComp();

		int hp;
		int damage;//後でplayreからダメージをとってくる

		void OnCollisionEnter(shared_ptr<GameObject>& other)override;

	public:
		Wall(const shared_ptr<Stage>& stage):
		BrokenObjs(stage),
			hp(3)
		{
		}

		void OnCreate()override;
		void OnUpdate()override;

	};
	

}
