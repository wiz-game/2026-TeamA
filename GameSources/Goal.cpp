#include "stdafx.h"
#include "Project.h"


namespace basecross
{
	void Goal::OnCreate()
	{
		InitTransComp();
		InitDrawComp();
	}

	void Goal::InitTransComp()
	{
		auto stage = GetStage();

		m_transComp = GetComponent<Transform>();
		m_transComp->SetPosition(Vec3(0, 2, 10));
		m_transComp->SetScale(Vec3(1, 1, 1));
		m_transComp->SetRotation(Vec3(0));

		auto col = AddComponent<CollisionObb>();
		auto grv = AddComponent<Gravity>();
	}

	void Goal::InitDrawComp()
	{
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
		m_drawComp->SetDiffuse(Col4(1, 0, 1, 0));

	}

	void Goal::OnUpdate()
	{

	}

	void Goal::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		auto stage = GetStage();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (other && player)
		{
			auto& app = App::GetApp();
			auto scene = app->GetScene<Scene>();
			wstringstream wss(L"");

			wss << L"GameClear" << endl;
			scene->SetDebugString(wss.str());



		}
	}
}