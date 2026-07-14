#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	PauseButtonUI::PauseButtonUI(const shared_ptr<Stage>& stage) :
		UIManager(stage)
	{}
	PauseButtonUI::~PauseButtonUI() {}

	void PauseButtonUI::OnCreate()
	{
		CreateUI();
		UIManager::SetUI(m_buttonUI);
	}

	void PauseButtonUI::CreateUI()
	{
		auto stage = GetStage();
		m_buttonUI = stage->AddGameObject<Sprite>(L"TEX_PAUSEBUTTONUI", true, Vec3(1024, 256, 0) * 0.0035f, Vec3(-515, -350, 0));
	}
}