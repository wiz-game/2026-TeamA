#include "stdafx.h"
#include "Project.h"


namespace basecross
{
	void Wall::OnCreate()
	{

		InitDrawComp();
		InitTransComp();

		damage = 2;

		BrokenObjs::OnCreate();
		BrokenObjs::SetHP(hp);

	}

	void Wall::InitDrawComp()
	{
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");

	}

	void Wall::InitTransComp()
	{
		auto stage = GetStage();

		m_transComp = GetComponent<Transform>();
		m_transComp->SetPosition(Vec3(0, 15, 5));
		m_transComp->SetScale(Vec3(3, 10, 1));
		m_transComp->SetRotation(Vec3(0));

		auto col = AddComponent<CollisionObb>();
		auto grv = AddComponent<Gravity>();

	}

	void Wall::OnUpdate()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		wstringstream wss;

		wss << L"WallHP" << BrokenObjs::GetHP() << endl;
		scene->SetDebugString(wss.str());

		BrokenObjs::OnUpdate();

	}

	void Wall::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Player"))
		{
			BrokenObjs::SetDamage(damage);
			BrokenObjs::takeDamage();
		}
	}
}