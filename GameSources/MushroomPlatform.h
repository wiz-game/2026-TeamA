#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class MushroomPlatform : public GameObject
	{
		shared_ptr<Transform> m_transComp;
		shared_ptr<PNTStaticDraw> m_drawComp;

		float jumpPower;
		bool isHit;

	public:
		MushroomPlatform(shared_ptr<Stage>& stage);
		~MushroomPlatform();

		void OnCreate()override;
		void OnUpdate()override;
		void OnCollisionEnter(shared_ptr<GameObject>& other)override;

	};
}