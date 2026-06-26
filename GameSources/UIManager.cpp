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