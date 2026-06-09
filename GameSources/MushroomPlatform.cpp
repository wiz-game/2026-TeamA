#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	MushroomPlatform::MushroomPlatform(shared_ptr<Stage>& stage) :
		GameObject(stage),
		jumpPower(15.0f),
		isHit(false)
	{
	}
	MushroomPlatform::~MushroomPlatform(){}

	void MushroomPlatform::OnCreate()
	{
		m_transComp = GetComponent<Transform>();
		m_transComp->SetScale(10, 1, 10);
		m_transComp->SetPosition(-25, -2.5f, 0);

		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");

		auto col = AddComponent<CollisionObb>();
		col->SetFixed(true);
	}

	void MushroomPlatform::OnUpdate()
	{
		if (isHit)
		{
			auto& app = App::GetApp();
			auto& scene = app->GetScene<Scene>();
			auto& stage = scene->GetActiveTypeStage<GameStage>();
			auto& player = stage->GetSharedGameObject<Player>(L"Player");
			if (!player) return;
			auto elapsedTime = app->GetElapsedTime();
			auto playerTrans = player->GetComponent<Transform>();
			Vec3 playerPos = playerTrans->GetPosition();
			Vec3 pos = GetComponent<Transform>()->GetPosition();

			playerPos.y += jumpPower;

			playerTrans->SetPosition(playerPos);
		}

	}

	void MushroomPlatform::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		//auto player = dynamic_pointer_cast<Player>(other);
		auto& app = App::GetApp();
		auto& scene = app->GetScene<Scene>();
		auto& stage = scene->GetActiveTypeStage<GameStage>();
		auto& player = stage->GetSharedGameObject<Player>(L"Player");

		if (!player) return;

		if (player)
		{
			auto playerTrans = player->GetComponent<Transform>();
			Vec3 playerPos = playerTrans->GetPosition();
			Vec3 pos = GetComponent<Transform>()->GetPosition();
			if (playerPos.y >= pos.y)
			{
				isHit = true;
			}
		}
	}

}