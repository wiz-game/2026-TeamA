#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class Fence : public StageObject
	{
		shared_ptr<PNTStaticDraw> m_drawComp;

		//void OnCollisionEnter(shared_ptr<GameObject>& other)override;

	public:
		Fence(const shared_ptr<Stage>& stage) :
			StageObject(stage)
		{

		}

		void OnCreate()override;
		void OnUpdate()override;

		void SetCollar(Col4 collar);

		void SetModel(const wstring& modelKey);
	};
}