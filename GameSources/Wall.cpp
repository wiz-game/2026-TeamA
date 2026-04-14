#include "stdafx.h"
#include "Project.h"


namespace basecross
{
	void Wall::OnCreate()
	{

		InitDrawComp();
		InitTransComp();

		damage = 2;

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
		m_transComp->SetPosition(Vec3(0, 25, 5));
		m_transComp->SetScale(Vec3(3, 10, 1));
		m_transComp->SetRotation(Vec3(0));

		auto col = AddComponent<CollisionObb>();
		auto grv = AddComponent<Gravity>();

	}

	void Wall::OnUpdate()
	{
	}

	void Wall::takeDamage()
	{
	}

	void Wall::OnCollisionEnter(const shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Player"))
		{
			BrokenObjs::SetDamage(damage);
			//BrokenObjs::takeDamage();
			takeDamage();
		}
	}
}