#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class Goal : public StageObject
	{
		shared_ptr<PNTStaticDraw> m_drawComp;

		void OnCollisionEnter(shared_ptr<GameObject>& other)override;

	public:
		Goal(const shared_ptr<Stage>& stage) :
			StageObject(stage)
		{

		}

		void OnCreate()override;
		void OnUpdate()override;


	};
}