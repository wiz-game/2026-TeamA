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
		col->SetDrawActive(true);
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
		StageObject::OnCreate();

	}

	void TurningPoint::OnUpdate()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveTypeStage<GameStage>();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		Vec3 playerPos = player->GetPosition();
		Vec3 dir = m_position - playerPos;
		float diff = dir.length();
		if (diff <= 10.0f)
			m_trigger = true;
		else if (diff <= 9.0f)
			m_trigger = false;


		StageObject::OnUpdate();
	}
	
	void TurningPoint::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		//auto& app = App::GetApp();
		//auto scene = app->GetScene<Scene>();
		//auto stage = scene->GetActiveTypeStage<GameStage>();
		//auto player = stage->GetSharedGameObject<Player>(L"Player");
		//if (other->FindTag(L"Player"))
		//{
		//	m_trigger = true;
		//}
	}
}