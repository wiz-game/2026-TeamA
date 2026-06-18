#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TurningPoint::TurningPoint(const shared_ptr<Stage>& stage) :
		StageObject(stage),
		m_trigger(false),
		m_isInRange(false)
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

		if (diff >= 10.0f) // scale‚ğl—¶‚µ‚±‚Ì’l‚É‚·‚é
			m_isInRange = false; //	10.0f‚æ‚è—£‚ê‚Ä‚¢‚éê‡‚É”ÍˆÍŠO”»’è‚É‚·‚é

		if (diff <= 10.0f && !m_isInRange && !m_trigger) 
		{
			m_trigger = true;
			m_isInRange = true; // 10.0fˆÈ“à‚É—ˆ‚½ê‡‚É”ÍˆÍ“à”»’è‚É‚·‚é
		}
		else if (diff <= 10.0f && !m_isInRange && m_trigger)
		{
			m_trigger = false;
			m_isInRange = true;
		}



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