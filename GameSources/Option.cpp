#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Option::Option(const shared_ptr<Stage>& stage) :
		GameObject(stage),
		m_prevLStick(0),
		m_isActive(false),
		m_optionItem(OptionItem::Return),
		cursolpositon(Vec3(-250,170,0)),
		itemCount(static_cast<int>(OptionItem::Count))
	{
	}
	Option::~Option(){}

	void Option::OnCreate()
	{
		CreateUI();

		SetVisible(false);

		m_pauseUI[0]->SetDrawActive(m_isActive);
	}

	void Option::CreateUI()
	{
		auto stage = GetStage();
		m_pauseUI.push_back(stage->AddGameObject<Sprite>(L"TEX_PAUSEUI", true, Vec3(1280, 800, 0) * 0.01f, Vec3(0, 0, 0))); //ポーズ背景
		m_cursol = stage->AddGameObject<Sprite>(L"TEX_POINTERUI", true, Vec3(200, 200, 0) * 0.01f, cursolpositon); //カーソル
		m_pauseUI.push_back(m_cursol);

	}

	void Option::SetVisible(bool visible)
	{
		m_isActive = visible;
		// Option自身と、中にあるUIスプライト全ての表示・更新状態を切り替える
		for (auto& ui : m_pauseUI)
		{
			ui->SetDrawActive(visible);   // 描画のON/OFF
			ui->SetUpdateActive(visible); // 更新のON/OFF
		}
	}

	void Option::OnUpdate()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
		auto stage = GetStage()->GetThis<GameStage>();
		float cursolIndex = 117.0f; //カーソル移動用変数
		int wrapIndex = cursolIndex * (itemCount - 1); //一番上や下の時用の変数

		// 左スティックの値取得
		Vec2 LStick(pad.fThumbLX, pad.fThumbLY);
		if (!stage->GetIsActive())
		{

			switch (m_optionItem)
			{
			case OptionItem::Return:
				if (m_prevLStick.y <= -0.25f && LStick.y >= -0.25f)
				{
					m_optionItem = OptionItem::Restart;
					cursolpositon.y -= cursolIndex;
				}
				else if (m_prevLStick.y <= 0.25f && LStick.y >= 0.25f)
				{
					m_optionItem = OptionItem::Title;
					cursolpositon.y -= wrapIndex;
				}
				if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
				{
					stage->SetIsActive(true);
					SetVisible(false);
				}
				break;
			case OptionItem::Restart:
				if (m_prevLStick.y <= -0.25f && LStick.y >= -0.25f)
				{
					m_optionItem = OptionItem::StageSelect;
					cursolpositon.y -= cursolIndex;
				}
				else if (m_prevLStick.y <= 0.25f && LStick.y >= 0.25f)
				{
					m_optionItem = OptionItem::Return;
					cursolpositon.y += cursolIndex;
				}
				break;
			case OptionItem::StageSelect:
				if (m_prevLStick.y <= -0.25f && LStick.y >= -0.25f)
				{
					m_optionItem = OptionItem::SoundTest;
					cursolpositon.y -= cursolIndex;
				}
				else if (m_prevLStick.y <= 0.25f && LStick.y >= 0.25f)
				{
					m_optionItem = OptionItem::Restart;
					cursolpositon.y += cursolIndex;
				}
				if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
				{
					PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"ToGameOverStage");//ゲームシーンを移動する
				}
				break;
			case OptionItem::SoundTest:
				if (m_prevLStick.y <= -0.25f && LStick.y >= -0.25f)
				{
					m_optionItem = OptionItem::Title;
					cursolpositon.y -= cursolIndex;
				}
				else if (m_prevLStick.y <= 0.25f && LStick.y >= 0.25f)
				{
					m_optionItem = OptionItem::StageSelect;
					cursolpositon.y += cursolIndex;
				}
				break;
			case OptionItem::Title:
				if (m_prevLStick.y <= -0.25f && LStick.y >= -0.25f)
				{
					m_optionItem = OptionItem::Return;
					cursolpositon.y += wrapIndex;
				}
				else if (m_prevLStick.y <= 0.25f && LStick.y >= 0.25f)
				{
					m_optionItem = OptionItem::SoundTest;
					cursolpositon.y += cursolIndex;
				}
				if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
				{
					PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"ToGameClearStage");//ゲームシーンを移動する
				}

				break;

			}
		
			m_cursol->GetComponent<Transform>()->SetPosition(cursolpositon);
			m_prevLStick = LStick;
		}

	}
}