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
		//auto stage = GetStage();
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		//auto stage = scene->GetActiveTypeStage<GameStage>();
		auto stage = GetStage()->GetThis<GameStage>();


		m_gameUI.push_back(stage->AddGameObject<PreviewButtonUI>());
		m_gameUI.push_back(stage->AddGameObject<FormationUI>());
		m_gameUI.push_back(stage->AddGameObject<PauseButtonUI>());
		m_gameUI.push_back(stage->AddGameObject<FormationCountUI>());

		for (int i = 0; i < 3; i++)
		{
			auto spCount = stage->AddGameObject<NumberSprite>();
			m_formationCount.push_back(spCount);
			m_formationCount[i]->GetThis<NumberSprite>()->m_transComp->SetPosition(Vec3((i * 35) + 420, -110, 0));
			m_formationCount[i]->GetComponent<PCTSpriteDraw>()->SetTextureResource(L"TEX_NUMBER");
			m_formationCount[i]->GetComponent<Transform>()->SetScale(Vec3(0.3f));
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
		auto stage = GetStage()->GetThis<GameStage>();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		int count = player->GetSubPlayerManager()->GetActiveNum();

		int digits[3] = { (count / 100) % 10, (count / 10) % 10, count % 10 };

		for (int i = 0; i < m_formationCount.size(); i++)
		{
			
			auto sp = dynamic_pointer_cast<NumberSprite>(m_formationCount[i]);
			if (sp) 
			{
				sp->SetNumber(digits[i]);
			}
		}
	}
}