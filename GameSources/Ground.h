#pragma once
#include "stdafx.h"

namespace basecross
{
	class Ground : public GameObject
	{
	public:
		Ground(const shared_ptr<Stage>& stage) :
			GameObject(stage)
		{

		}

		shared_ptr<Transform> m_transComp;
		shared_ptr<PNTStaticDraw> m_drawComp;

		void InitTransComp();
		void InitDrawComp();


		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}