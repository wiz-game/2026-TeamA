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

	void SelectStage::OnUpdate()
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

	void SelectStage::CreateUI()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();

		//AddGameObject<Sprite>(L"TEX_GameClear", true, Vec3(1024, 256, 0) * 0.01f, Vec3(0, 0, 0));

	}

	void SelectStage::LoadTextures()
	{
		auto& app = App::GetApp();
		auto mediaPath = app->GetDataDirWString();
		auto texPath = mediaPath + L"Textures\\";

		app->RegisterTexture(L"TEX_GameClear", texPath + L"GameClear.png");
	}

	void SelectStage::OnDestroy()
	{
		auto& app = App::GetApp();
		auto XAPtr = app->GetXAudio2Manager();
		XAPtr->Stop(m_BGM);
	}

}