#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class TurningPoint : public StageObject
	{
		shared_ptr<Transform> m_transComp;
		Vec3 m_position;

	public:
		TurningPoint(const shared_ptr<Stage>& stage);
		~TurningPoint();

		void SetPosition(Vec3 newPosition)
		{
			m_position = newPosition;
		}

		Vec3 GetPosition() const
		{
			return m_position;
		}

		void OnUpdate() override;
		void OnCreate() override;

	};
}