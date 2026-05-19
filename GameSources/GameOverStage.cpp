/*!
@file GameOverStage.cpp
@brief ゲームオーバーステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{

	//--------------------------------------------------------------------------------------
	//	ゲームオーバーステージクラス実体
	//--------------------------------------------------------------------------------------

	void GameOverStage::CreateViewLight()
	{
		auto camera = ObjectFactory::Create<Camera>();
		camera->SetEye(Vec3(0.0, 2.0f, -5.0f));
		camera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting();
	}

	void GameOverStage::OnCreate()
	{

	}

	void GameOverStage::OnUpdate()
	{

	}

	void GameOverStage::CreateUI()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();

		AddGameObject<Sprite>(L"TEX_GameOver", true, Vec3(1280, 840, 0), Vec3(0, 0, 0));

	}

	void GameOverStage::LoadTextures()
	{
		auto& app = App::GetApp();
		auto mediaPath = app->GetDataDirWString();
		auto texPath = mediaPath + L"Textures\\";

		app->RegisterTexture(L"TEX_GameOver", texPath + L"GameOver.png");
	}

}