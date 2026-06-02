#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class Vine : public StageObject
	{
		shared_ptr<Transform> m_transComp;
		shared_ptr<PNTStaticDraw> m_drawComp;
		BrokenObjs m_broken;

		void OnCollisionEnter(shared_ptr<GameObject>& other)override;
		int hp;
		int damage;//後でplayreからダメージをとってくる

	public:
		Vine(shared_ptr<Stage>& stage);
		~Vine();

		void OnCreate()override;
		void OnUpdate()override;

	};

}