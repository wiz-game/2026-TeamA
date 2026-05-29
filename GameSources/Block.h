#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class Block : public StageObject
	{
		shared_ptr<PNTStaticDraw> m_drawComp;

		//void OnCollisionEnter(shared_ptr<GameObject>& other)override;

	public:
		Block(const shared_ptr<Stage>& stage) :
			StageObject(stage)
		{

		}

		void OnCreate()override;
		void OnUpdate()override;

		void SetCollar(Col4 collar);
	};
}