#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	UIManager::UIManager(const shared_ptr<Stage>& stage) :
		GameObject(stage)
	{
	}
	UIManager::~UIManager(){}


	void UIManager::OnCreate()
	{
		CreateUI();
	}

	void UIManager::CreateUI()
	{
		auto stage = GetStage();
		stage->AddGameObject<PreviewButtonUI>();
		stage->AddGameObject<FormationUI>();
		stage->AddGameObject<PauseButtonUI>();
		stage->AddGameObject<FormationCountUI>();
		for (int i = 0; i < 100; i++)
		{
			m_formationCount.push_back(stage->AddGameObject<NumberSprite>());
			m_formationCount[i]->GetThis<NumberSprite>()->m_transComp->SetPosition(Vec3((i * 300) - 400, 400, 0));
		}

	}


	void UIManager::SetVisible(bool visible)
	{
		for (auto& ui : m_gameUI)
		{
			ui->SetDrawActive(visible);   // 描画のON/OFF
			ui->SetUpdateActive(visible); // 更新のON/OFF
		}
	}

	void UIManager::OnUpdate()
	{

	}
}