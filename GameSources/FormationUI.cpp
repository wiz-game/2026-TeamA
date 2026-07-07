#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	FormationUI::FormationUI(const shared_ptr<Stage>& stage) :
		UIManager(stage),
		m_formation(Formation::Hammer)
	{
	}
	FormationUI::~FormationUI(){}

	void FormationUI::OnCreate()
	{
		CreateUI();
		UIManager::SetUI(m_formationUI);
	}

	void FormationUI::CreateUI()
	{
		auto stage = GetStage();

		//m_formationUIs.push_back(stage->AddGameObject<Sprite>(L"TEX_YZUI", true, Vec3(1280, 840, 0) * 0.003f, Vec3(450, -250, 0)));     //0:矢印
		//m_formationUIs.push_back(stage->AddGameObject<Sprite>(L"TEX_HAMMERUI", true, Vec3(1280, 840, 0) * 0.002f, Vec3(450, -270, 0))); //1:ハンマー
		//m_formationUIs.push_back(stage->AddGameObject<Sprite>(L"TEX_STAIRSUI", true, Vec3(1280, 840, 0) * 0.002f, Vec3(450, -270, 0))); //2:階段(スロープ)
		//m_formationUIs.push_back(stage->AddGameObject<Sprite>(L"TEX_SPEARUI", true, Vec3(1280, 840, 0) * 0.002f, Vec3(450, -270, 0)));  //3:スピアー
		//m_formationUIs.push_back(stage->AddGameObject<Sprite>(L"TEX_BRIDGEUI", true, Vec3(1280, 840, 0) * 0.002f, Vec3(450, -270, 0))); //4:橋

		//m_formationUIs[2]->SetDrawActive(false);
		//m_formationUIs[3]->SetDrawActive(false);
		//m_formationUIs[4]->SetDrawActive(false);

		Vec3 pos = Vec3(440, -280, 0);

		m_formationUIs.push_back(m_formationUI = stage->AddGameObject<Sprite>(L"TEX_YZUI_R",   true, Vec3(1024, 2048, 0) * 0.0015f,  Vec3(530, -260, 0))); //0:矢印（右）
		m_formationUIs.push_back(m_formationUI = stage->AddGameObject<Sprite>(L"TEX_YZUI_L",   true, Vec3(1024, 2048, 0) * 0.0015f,  Vec3(350, -260, 0))); //1:矢印（左）
		m_formationUIs.push_back(m_formationUI = stage->AddGameObject<Sprite>(L"TEX_HAMMERUI", true, Vec3(2048, 2048, 0) * 0.0011f, pos)); //2:ハンマー
		m_formationUIs.push_back(m_formationUI = stage->AddGameObject<Sprite>(L"TEX_STAIRSUI", true, Vec3(2048, 2048, 0) * 0.0011f, pos)); //3:階段(スロープ)
		m_formationUIs.push_back(m_formationUI = stage->AddGameObject<Sprite>(L"TEX_SPEARUI",  true, Vec3(2048, 2048, 0) * 0.0011f, pos)); //4:スピアー
		m_formationUIs.push_back(m_formationUI = stage->AddGameObject<Sprite>(L"TEX_BRIDGEUI", true, Vec3(2048, 2048, 0) * 0.0011f, pos)); //5:橋

		m_formationUIs[3]->SetDrawActive(false);
		m_formationUIs[4]->SetDrawActive(false);
		m_formationUIs[5]->SetDrawActive(false);

	}

	void FormationUI::OnUpdate()
	{
		auto& app = App::GetApp();
		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];

		switch (m_formation)
		{
		case Formation::Hammer:
			if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				//m_yz_RDrawComp->SetDiffuse(Col4(1, 1, 1, 0.5f));
				m_formationUIs[2]->SetDrawActive(false);
				m_formationUIs[3]->SetDrawActive(true);
				m_formation = Formation::Slope;
			}
			else if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			{
				m_formationUIs[2]->SetDrawActive(false);
				m_formationUIs[5]->SetDrawActive(true);
				m_formation = Formation::Bridge;
			}
			break;
		case Formation::Slope:
			if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				//m_yz_RDrawComp->SetDiffuse(Col4(1, 1, 1, 0.5f));
				m_formationUIs[3]->SetDrawActive(false);
				m_formationUIs[4]->SetDrawActive(true);
				m_formation = Formation::Spear;
			}
			else if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			{
				//m_yz_LDrawComp->SetDiffuse(Col4(1, 1, 1, 0.5f));
				m_formationUIs[3]->SetDrawActive(false);
				m_formationUIs[2]->SetDrawActive(true);
				m_formation = Formation::Hammer;
			}
			break;
		case Formation::Spear:
			if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				//m_yz_RDrawComp->SetDiffuse(Col4(1, 1, 1, 0.5f));
				m_formationUIs[4]->SetDrawActive(false);
				m_formationUIs[5]->SetDrawActive(true);
				m_formation = Formation::Bridge;
			}
			else if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			{
				//m_yz_LDrawComp->SetDiffuse(Col4(1, 1, 1, 0.5f));
				m_formationUIs[4]->SetDrawActive(false);
				m_formationUIs[3]->SetDrawActive(true);
				m_formation = Formation::Slope;
			}
			break;
		case Formation::Bridge:
			if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				//m_yz_LDrawComp->SetDiffuse(Col4(1, 1, 1, 0.5f));
				m_formationUIs[5]->SetDrawActive(false);
				m_formationUIs[2]->SetDrawActive(true);
				m_formation = Formation::Hammer;
			}
			else if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			{
				m_formationUIs[5]->SetDrawActive(false);
				m_formationUIs[4]->SetDrawActive(true);
				m_formation = Formation::Spear;
			}
			break;
		}

		// 右矢印(index 0)の色の設定
		auto drawR = m_formationUIs[0]->GetComponent<PCTSpriteDraw>();
		if (pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			// RBが押されている間はグレーにする（暗くする）
			drawR->SetDiffuse(Col4(0.5f, 0.5f, 0.5f, 1.0f));
		}
		else {
			// 離している間は白（通常）
			drawR->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// 左矢印(index 1)の色の設定
		auto drawL = m_formationUIs[1]->GetComponent<PCTSpriteDraw>();
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			// LBが押されている間はグレーにする
			drawL->SetDiffuse(Col4(0.5f, 0.5f, 0.5f, 1.0f));
		}
		else {
			drawL->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}