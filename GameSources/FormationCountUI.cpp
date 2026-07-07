#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	FormationCountUI::FormationCountUI(const shared_ptr<Stage>& stage) :
		UIManager(stage)
	{
	}
	FormationCountUI::~FormationCountUI(){}

	void FormationCountUI::OnCreate()
	{
		CreateUI();
		UIManager::SetUI(m_countUI);
	}

	void FormationCountUI::CreateUI()
	{
		auto stage = GetStage();
		m_countUI = stage->AddGameObject<Sprite>(L"TEX_FORMATIONCOUNTUI", true, Vec3(2048, 2048, 0) * 0.0003f, Vec3(380, -150, 0));
	}
}