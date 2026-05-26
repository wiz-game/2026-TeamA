/*!
@file GameClearStage.cpp
@brief ゲームクリアステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{

	//--------------------------------------------------------------------------------------
	//	ゲームクリアステージクラス実体
	//--------------------------------------------------------------------------------------

	void GameClearStage::CreateViewLight() 
	{
		auto camera = ObjectFactory::Create<Camera>();
		camera->SetEye(Vec3(0.0, 2.0f, -5.0f));
		camera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting();
	}

	void GameClearStage::OnCreate()
	{
		LoadTextures();
		CreateUI();
	}

	void GameClearStage::OnUpdate()
	{
		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();

		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];

		if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");//ゲームシーンを移動する
		}

	}

	void GameClearStage::CreateUI()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();

		AddGameObject<Sprite>(L"TEX_GameClear", true, Vec3(1280, 840, 0), Vec3(0, 0, 0));

	}

	void GameClearStage::LoadTextures()
	{
		auto& app = App::GetApp();
		auto mediaPath = app->GetDataDirWString();
		auto texPath = mediaPath + L"Textures\\";

		app->RegisterTexture(L"TEX_GameClear", texPath + L"GameClear.png");
	}

}