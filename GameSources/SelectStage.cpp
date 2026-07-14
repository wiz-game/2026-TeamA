/*!
@file GameClearStage.cpp
@brief ゲームクリアステージ実体
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

		m_back = AddGameObject<Sprite>(L"TEX_StageSelect", true, Vec3(1920, 1080, 0) * 0.006f, Vec3(0, 0, 0));
		m_back2 = AddGameObject<Sprite>(L"TEX_StageSelect2", true, Vec3(1920, 1080, 0) * 0.006f, Vec3(-1920, 0, 0));
		AddGameObject<Sprite>(L"TEX_BACK", true, Vec3(1272, 789, 0) * 0.003f, Vec3(250, 0, 0));
		AddGameObject<Sprite>(L"TEX_STAGE1", true, Vec3(1536, 1024, 0) * 0.0025f, Vec3(-250, 0, 0));
		AddGameObject<Sprite>(L"TEX_YZR", true, Vec3(2048, 2048, 0) * 0.001f, Vec3(550, 0, 0));
		AddGameObject<Sprite>(L"TEX_YZL", true, Vec3(2048, 2048, 0) * 0.001f, Vec3(-550, 0, 0));

	}

	void SelectStage::OnUpdate()
	{
		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();

		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
		float elapsedTime = app->GetElapsedTime();
		bool ok = false;

		switch (m_stageCount)
		{
		case StageCount::stage1:
			if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");//ゲームシーンを移動する
			}
			if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				ok = true;
				m_stageCount = stage2;
			}
			break;
		case StageCount::stage2:
			if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");//ゲームシーンを移動する
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

	}



	void SelectStage::LoadTextures()
	{
		auto& app = App::GetApp();
		auto mediaPath = app->GetDataDirWString();
		auto texPath = mediaPath + L"Textures\\";

		app->RegisterTexture(L"TEX_StageSelect", texPath + L"StageSelect.png");
		app->RegisterTexture(L"TEX_StageSelect2", texPath + L"StageSelect2.png");
		app->RegisterTexture(L"TEX_BACK", texPath + L"back.png");
		app->RegisterTexture(L"TEX_STAGE1", texPath + L"stage1.png");
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