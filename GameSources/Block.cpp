#include "stdafx.h"
#include "Project.h"


namespace basecross
{
	void Block::OnCreate()
	{
		StageObject::OnCreate();
		m_stage = GetStage();
		//auto grv = AddComponent<Gravity>();
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
	}

	void Block::OnUpdate()
	{
		StageObject::OnUpdate();

	}

	void Block::SetCollar(Col4 collar)
	{
		m_drawComp->SetDiffuse(collar);
	}

}