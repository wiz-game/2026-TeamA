#include "stdafx.h"
#include "Project.h"


namespace basecross
{
	Baite::Baite(shared_ptr<Stage>& stage) :
		GameObject(stage),
		m_isPicked(false),
		m_speed(5.0f)
	{
	}
	Baite::~Baite(){}

	void Baite::OnCreate()
	{
		m_transComp = GetComponent<Transform>();
		m_transComp->SetScale(1, 1, 1);
		m_transComp->SetPosition(10, -1, 0);

		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
		m_drawComp->SetDiffuse(Col4(1, 0, 0, 1));

		AddComponent<CollisionObb>();
	}

	void Baite::OnUpdate()
	{
		if (m_isPicked)
		{
			auto& app = App::GetApp();
			auto& scene = app->GetScene<Scene>();
			auto stage = scene->GetActiveTypeStage<GameStage>();
			auto player = stage->GetSharedGameObject<Player>(L"Player");
			Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();
			Vec3 pos = m_transComp->GetPosition();
			float elapsedTime = app->GetElapsedTime();

			Vec3 diff = playerPos - pos;
			Vec3 dir = diff.normalize();
			pos += dir * elapsedTime * m_speed;

			m_transComp->SetPosition(pos);

		}
	}

	void Baite::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		auto& app = App::GetApp();
		auto& scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveTypeStage<GameStage>();
		auto player = stage->GetSharedGameObject<Player>(L"Player");

		if (other == player)
		{
			stage->RemoveGameObject<Baite>(GetThis<Baite>());
		}

		if (other->FindTag(L"Attack"))
		{
			m_isPicked = true;
		}
		
	}

	BigBaite::BigBaite(shared_ptr<Stage>& stage) :
		Baite(stage)
	{
	}
	BigBaite::~BigBaite(){}

	void BigBaite::OnCreate()
	{
		Baite::OnCreate();

		m_transComp = GetComponent<Transform>();
		m_transComp->SetScale(10, 10, 10);
		m_transComp->SetPosition(10, 3, 0);
	}

	void BigBaite::OnUpdate()
	{

		Baite::OnUpdate();
	}

	void BigBaite::OnCollisionEnter(shared_ptr<GameObject>& other)
	{

		Baite::OnCollisionEnter(other);
	}
}