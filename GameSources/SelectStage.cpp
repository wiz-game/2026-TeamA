/*!
@file GameClearStage.cpp
@brief ステージセレクトクラス実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{

	//--------------------------------------------------------------------------------------
	//	ステージセレクトクラス実体
	//--------------------------------------------------------------------------------------

	void SelectStage::CreateViewLight()
	{
		auto camera = ObjectFactory::Create<Camera>();
		camera->SetEye(Vec3(0.0, 2.0f, -5.0f));
		camera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting();
	}

	void SelectStage::OnCreate()
	{
		CreateViewLight();
		LoadTextures();
		CreateUI();
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto XAPtr = app->GetXAudio2Manager();
		m_BGM = XAPtr->Start(L"TitleBGM", 0, scene->m_BGMVolume);


	}

	void SelectStage::CreateUI()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();

		m_back.push_back(AddGameObject<Sprite>(L"TEX_StageSelect", true, Vec3(1920, 1080, 0) * 0.006f, Vec3(0, 0, 0)));     //0:ステージセレクト画像
		m_back.push_back(AddGameObject<Sprite>(L"TEX_StageSelect2", true, Vec3(1920, 1080, 0) * 0.006f, Vec3(-1920, 0, 0)));//1:ステージセレクト画像
		m_back.push_back(AddGameObject<Sprite>(L"TEX_SkyBoxHorizontal", true, Vec3(1920, 1080, 0) * 0.01f, Vec3(0, 0, 0))); //2:背景

		m_other.push_back(AddGameObject<Sprite>(L"TEX_BACK", true, Vec3(1272, 789, 0) * 0.003f, Vec3(250, 0, 0)));          //0:ステージの一部分画像
		m_other.push_back(AddGameObject<Sprite>(L"TEX_YZR", true, Vec3(2048, 2048, 0) * 0.001f, Vec3(550, 0, 0)));          //1:右矢印
		m_other.push_back(AddGameObject<Sprite>(L"TEX_YZL", true, Vec3(2048, 2048, 0) * 0.001f, Vec3(-550, 0, 0)));         //2:左矢印
		m_other.push_back(AddGameObject<Sprite>(L"TEX_STAGE1", true, Vec3(1536, 1024, 0) * 0.0025f, Vec3(0, 0, 0)));        //3:ステージ１文字
		m_other.push_back(AddGameObject<Sprite>(L"TEX_STAGE2", true, Vec3(1264, 843, 0) * 0.0030f, Vec3(0, 0, 0)));        //4:ステージ２文字

		m_back[0]->SetDrawLayer(1);
		m_back[1]->SetDrawLayer(1);
		m_back[2]->SetDrawLayer(0);

		m_other[0]->SetDrawLayer(2);
		m_other[1]->SetDrawLayer(2);
		m_other[2]->SetDrawLayer(2);
		m_other[3]->SetDrawLayer(2);
		m_other[4]->SetDrawLayer(2);
	}

	void SelectStage::OnUpdate()
	{
		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();

		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
		auto key = input.GetKeyState();
		float elapsedTime = app->GetElapsedTime();
		bool ok = false;

		switch (m_stageCount)
		{
		case StageCount::stage1:
			m_other[0]->SetDrawActive(true);
			m_other[1]->SetDrawActive(true);
			m_other[2]->SetDrawActive(false);
			m_other[3]->GetComponent<Transform>()->SetPosition(-250, 0, 0);
			m_other[4]->GetComponent<Transform>()->SetPosition(-1000, 0, 0);

			if (pad.wPressedButtons & XINPUT_GAMEPAD_A || key.m_bPressedKeyTbl[VK_SPACE])
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");//ゲームシーンを移動する
			}
			if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				//ok = true;
				m_back[0]->GetComponent<Transform>()->SetPosition(1920, 0, 0);
				m_back[1]->GetComponent<Transform>()->SetPosition(0, 0, 0);

				m_stageCount = stage2;
			}
			break;
		case StageCount::stage2:
			m_other[0]->SetDrawActive(false);
			m_other[1]->SetDrawActive(false);
			m_other[2]->SetDrawActive(true);
			m_other[3]->GetComponent<Transform>()->SetPosition(-1000, 0, 0);
			m_other[4]->GetComponent<Transform>()->SetPosition(-250, 0, 0);

			if (pad.wPressedButtons & XINPUT_GAMEPAD_A || key.m_bPressedKeyTbl[VK_SPACE])
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");//ゲームシーンを移動する
			}
			if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			{
				m_back[0]->GetComponent<Transform>()->SetPosition(0, 0, 0);
				m_back[1]->GetComponent<Transform>()->SetPosition(-1920, 0, 0);

				m_stageCount = stage1;
			}
			break;
		}


		if (ok)
		{
			phaseTimer += elapsedTime;
			float t = phaseTimer / phaseDuration;
			if (t > 1.0f) t = 1.0f;

			if (t >= 1.0f)
			{
				phaseTimer = 0.0f;
			}
		}

		// 右矢印の色の設定
		auto drawR = m_other[1]->GetComponent<PCTSpriteDraw>();
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
		auto drawL = m_other[2]->GetComponent<PCTSpriteDraw>();
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			// LBが押されている間はグレーにする
			drawL->SetDiffuse(Col4(0.5f, 0.5f, 0.5f, 1.0f));
		}
		else
		{
			drawL->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		}

	}



	void SelectStage::LoadTextures()
	{
		auto& app = App::GetApp();
		auto mediaPath = app->GetDataDirWString();
		auto texPath = mediaPath + L"Textures\\";

		app->RegisterTexture(L"TEX_StageSelect", texPath + L"StageSelect.png");
		app->RegisterTexture(L"TEX_StageSelect2", texPath + L"StageSelect2.png");
		app->RegisterTexture(L"TEX_SkyBoxHorizontal", texPath + L"SkyBoxHorizontal2.jpg");
		app->RegisterTexture(L"TEX_BACK", texPath + L"back.png");
		app->RegisterTexture(L"TEX_STAGE1", texPath + L"stage1.png");
		app->RegisterTexture(L"TEX_STAGE2", texPath + L"stage2.png");
		app->RegisterTexture(L"TEX_YZR", texPath + L"yzr.png");
		app->RegisterTexture(L"TEX_YZL", texPath + L"yzl.png");

	}

	void SelectStage::OnDestroy()
	{
		auto& app = App::GetApp();
		auto XAPtr = app->GetXAudio2Manager();
		XAPtr->Stop(m_BGM);
	}

}