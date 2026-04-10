#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Ground::OnCreate()
	{
		InitTransComp();
		InitDrawComp();

		AddTag(L"Ground");
	}

	void Ground::OnUpdate()
	{
	}

	void Ground::InitTransComp()
	{
		auto stage = GetStage();

		m_transComp = GetComponent<Transform>();
		m_transComp->SetPosition(Vec3(0, 0, 0));
		m_transComp->SetScale(Vec3(30, 1, 30));
		m_transComp->SetRotation(Vec3(0));

		auto col = AddComponent<CollisionObb>();

	}

	void Ground::InitDrawComp()
	{
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
	}

}