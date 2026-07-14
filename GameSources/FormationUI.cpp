#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	FormationUI::FormationUI(const shared_ptr<Stage>& stage) :
		UIManager(stage),
		m_count(0),
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

		for (int i = 0; i < 2; i++)
		{
			auto count = stage->AddGameObject<NumberSprite>();
			m_counts.push_back(count); //隊列を組む際に必要な数の表示用UI
			m_counts[i]->GetThis<NumberSprite>()->GetComponent<PCTSpriteDraw>()->SetTextureResource(L"TEX_NUMBER2");
			m_counts[i]->GetComponent<Transform>()->SetPosition(Vec3((i * 15) + 497, -195, 0));
			m_counts[i]->GetComponent<Transform>()->SetScale(Vec3(0.15f));
			
			
		}
	}

	void FormationUI::OnUpdate()
	{
		auto& app = App::GetApp();
		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
		auto stage = GetStage()->GetThis<GameStage>();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		//int formationNumber = player->GetFormationManager()->GetFormationNumber();

		//switch (formationNumber)
		//{
		//case 0:
		//	m_formation = Formation::Hammer;
		//	break;
		//case 1:
		//	m_formation = Formation::Slope;
		//	break;
		//case 2:
		//	m_formation = Formation::Spear;
		//	break;
		//case 3:
		//	m_formation = Formation::Bridge;
		//	break;
		//default:
		//	return;
		//	break;
		//}

		switch (m_formation)
		{
		case Formation::Hammer:
			m_count = 20;
			if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
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
			m_count = 15;
			if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				m_formationUIs[3]->SetDrawActive(false);
				m_formationUIs[4]->SetDrawActive(true);
				m_formation = Formation::Spear;
			}
			else if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			{
				m_formationUIs[3]->SetDrawActive(false);
				m_formationUIs[2]->SetDrawActive(true);
				m_formation = Formation::Hammer;
			}
			break;
		case Formation::Spear:
			m_count = 20;
			if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				m_formationUIs[4]->SetDrawActive(false);
				m_formationUIs[5]->SetDrawActive(true);
				m_formation = Formation::Bridge;
			}
			else if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			{
				m_formationUIs[4]->SetDrawActive(false);
				m_formationUIs[3]->SetDrawActive(true);
				m_formation = Formation::Slope;
			}
			break;
		case Formation::Bridge:
			m_count = 15;
			if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
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

		// 右矢印の色の設定
		auto drawR = m_formationUIs[0]->GetComponent<PCTSpriteDraw>();
		if (pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) 
		{
			// RBが押されている間はグレーにする（暗くする）
			drawR->SetDiffuse(Col4(0.5f, 0.5f, 0.5f, 1.0f));
		}
		else 
		{
			// 離している間は白（通常）
			drawR->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// 左矢印の色の設定
		auto drawL = m_formationUIs[1]->GetComponent<PCTSpriteDraw>();
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) 
		{
			// LBが押されている間はグレーにする
			drawL->SetDiffuse(Col4(0.5f, 0.5f, 0.5f, 1.0f));
		}
		else 
		{
			drawL->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		//int count = player->GetFormationManager()->GetFormationCharacterNum();

		//int digit[2] = { (count / 10) % 10, count % 10 };
		for (int i = 0; i < m_counts.size(); i++)
		{
			auto sp = dynamic_pointer_cast<NumberSprite>(m_counts[i]);
			if (sp)
			{
				if (i == 0)
				{
					sp->SetNumber(m_count / 10);
				}
				else
				{
					sp->SetNumber(m_count % 10);
				}
			}

		}
		wstringstream wss;
		wss << L"Count" << m_count << endl;
		app->GetScene<Scene>()->SetDebugString(wss.str());
	}
}