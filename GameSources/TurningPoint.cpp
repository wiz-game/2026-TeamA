#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TurningPoint::TurningPoint(const shared_ptr<Stage>& stage) :
		StageObject(stage)
	{
	}
	TurningPoint::~TurningPoint(){}

	void TurningPoint::OnCreate()
	{
		m_transComp = AddComponent<Transform>();
		m_transComp->SetPosition(m_position);
	}

	void TurningPoint::OnUpdate()
	{

	}
	
}