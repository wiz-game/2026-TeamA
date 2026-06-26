#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	PreviewButtonUI::PreviewButtonUI(const shared_ptr<Stage>& stage):
		UIManager(stage)
	{
	}
	PreviewButtonUI::~PreviewButtonUI(){}

	void PreviewButtonUI::OnCreate()
	{
		CreateUI();
		UIManager::SetUI(m_buttonUI);	
	}

	void PreviewButtonUI::CreateUI()
	{
		auto stage = GetStage();
		m_buttonUI = stage->AddGameObject<Sprite>(L"TEX_PREVIEW", true, Vec3(1550, 700, 0) * 0.002f, Vec3(-450, -300, 0));
	}

}