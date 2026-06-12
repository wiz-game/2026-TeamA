#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TurningPoint::TurningPoint(const shared_ptr<Stage>& stage) :
		StageObject(stage),
		m_trigger(false)
	{
	}
	TurningPoint::~TurningPoint(){}

	void TurningPoint::OnCreate()
	{
		m_transComp = GetComponent<Transform>();

		m_position = m_transComp->GetPosition();

		auto col = AddComponent<CollisionObb>();
		col->SetFixed(false);
		col->SetDrawActive(true);
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
		StageObject::OnCreate();

	}

	void TurningPoint::OnUpdate()
	{
		StageObject::OnUpdate();
	}
	
	void TurningPoint::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveTypeStage<GameStage>();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (other->FindTag(L"Player"))
		{
			//auto mView = dynamic_pointer_cast<SingleView>(stage->GetView());
			//auto camera = mView->GetCamera();
			//auto playerCamera = dynamic_pointer_cast<PlayerCamera>(camera);

			//playerCamera->SetNextCameraAngle(nextEye = Vec3(100, 100, 100), nextAt = Vec3(0, 0, 0));
			//playerCamera->ChangeAngle();

			m_trigger = true;
		}
	}
}