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

	}

	void GameClearStage::OnUpdate()
	{

	}

	void GameClearStage::CreateUI()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();

		AddGameObject<Sprite>(L"TEX_GameCLlear", true, Vec3(1280, 840, 0), Vec3(0, 0, 0));

	}

	void GameClearStage::LoadTextures()
	{
		auto& app = App::GetApp();
		auto mediaPath = app->GetDataDirWString();
		auto texPath = mediaPath + L"Tetures\\";

		app->RegisterTexture(L"TEX_GameClear", texPath + L"GameClear.png");
	}

}