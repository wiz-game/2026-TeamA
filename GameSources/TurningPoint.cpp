#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TurningPoint::TurningPoint(const shared_ptr<Stage>& stage) :
		StageObject(stage),
		m_scale(1.0f, 1.0f, 0.0f),
		m_rotation(0.0f),
		m_position(0.0f)
	{
	}
	TurningPoint::~TurningPoint(){}

	void TurningPoint::OnCreate()
	{
		AddTag(L"CameraTrigger");
		m_transComp = AddComponent<Transform>();
		m_transComp->SetScale(m_scale);
		m_transComp->SetRotation(m_rotation);
		m_transComp->SetPosition(m_position);

	}

	void TurningPoint::OnUpdate()
	{

	}
	
	void TurningPoint::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		auto stage = GetStage();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (other && player);
		{
			playerCamera->SetNextCameraAngle(nextEye = Vec3(0, 0, 0), nextAt = Vec3(0, 0, 0));
			playerCamera->ChangeAngle();
		}
	}
}