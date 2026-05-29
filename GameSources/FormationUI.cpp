#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	FormationUI::FormationUI(const shared_ptr<Stage>& stage) :
		GameObject(stage),
		m_formation(Formation::Hammer)
	{
	}
	FormationUI::~FormationUI(){}

	void FormationUI::OnCreate()
	{
		CreateUI();
	}

	void FormationUI::CreateUI()
	{
		auto stage = GetStage();
		m_formationUI.push_back(stage->AddGameObject<Sprite>(L"TEX_YZUI", true, Vec3(1280, 800, 0) * 0.003f, Vec3(450, -250, 0)));     //0:矢印
		m_formationUI.push_back(stage->AddGameObject<Sprite>(L"TEX_HAMMERUI", true, Vec3(1280, 800, 0) * 0.002f, Vec3(450, -250, 0))); //1:ハンマー
		m_formationUI.push_back(stage->AddGameObject<Sprite>(L"TEX_STAIRSUI", true, Vec3(1280, 800, 0) * 0.002f, Vec3(450, -250, 0))); //2:階段(スロープ)
		m_formationUI.push_back(stage->AddGameObject<Sprite>(L"TEX_SPEARUI", true, Vec3(1280, 800, 0) * 0.002f, Vec3(450, -250, 0)));  //3:スピアー
		m_formationUI.push_back(stage->AddGameObject<Sprite>(L"TEX_BRIDGEUI", true, Vec3(1280, 800, 0) * 0.002f, Vec3(450, -250, 0))); //4:橋

		m_formationUI[2]->SetDrawActive(false);
		m_formationUI[3]->SetDrawActive(false);
		m_formationUI[4]->SetDrawActive(false);
	}

	void FormationUI::OnUpdate()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];

		switch (m_formation)
		{
		case Formation::Hammer:
			if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				m_formationUI[1]->SetDrawActive(false);
				m_formationUI[2]->SetDrawActive(true);
				m_formation = Formation::Slope;
			}
			break;
		case Formation::Slope:
			if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				m_formationUI[2]->SetDrawActive(false);
				m_formationUI[3]->SetDrawActive(true);
				m_formation = Formation::Spear;
			}
			else if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			{
				m_formationUI[2]->SetDrawActive(false);
				m_formationUI[1]->SetDrawActive(true);
				m_formation = Formation::Hammer;
			}
			break;
		case Formation::Spear:
			if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				m_formationUI[3]->SetDrawActive(false);
				m_formationUI[4]->SetDrawActive(true);
				m_formation = Formation::Bridge;
			}
			else if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			{
				m_formationUI[3]->SetDrawActive(false);
				m_formationUI[2]->SetDrawActive(true);
				m_formation = Formation::Slope;
			}
			break;
		case Formation::Bridge:
			if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			{
				m_formationUI[4]->SetDrawActive(false);
				m_formationUI[3]->SetDrawActive(true);
				m_formation = Formation::Spear;
			}
		}
	}
}