#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class Goal : public GameObject
	{
		shared_ptr<Transform> m_transComp;
		shared_ptr<PNTStaticDraw> m_drawComp;
		void InitTransComp();
		void InitDrawComp();

		void OnCollisionEnter(shared_ptr<GameObject>& other)override;

	public:
		Goal(const shared_ptr<Stage>& stage) :
			GameObject(stage)
		{

		}

		void OnCreate()override;
		void OnUpdate()override;


	};
}