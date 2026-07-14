/*!
@file GameClearStage.cpp
@brief タイトルステージクラス実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	//	タイトルステージクラス実体
	//--------------------------------------------------------------------------------------

	void TitleStage::CreateViewLight()
	{
		auto camera = ObjectFactory::Create<Camera>();
		camera->SetEye(Vec3(0.0, 2.0f, -5.0f));
		camera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting();
	}

	void TitleStage::OnCreate()
	{
		CreateViewLight();
		CreateUI();
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto XAPtr = app->GetXAudio2Manager();
		m_BGM = XAPtr->Start(L"TitleBGM", XAUDIO2_LOOP_INFINITE, scene->m_BGMVolume);

	}

	void TitleStage::CreateUI()
	{
		AddGameObject<Sprite>(L"TEX_TITLEUI", true, Vec3(1024, 256, 0) * 0.01f, Vec3(0, 0, 0));
	}

	void TitleStage::OnUpdate()
	{
		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();

		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
		auto key = input.GetKeyState();

		if (pad.wPressedButtons & XINPUT_GAMEPAD_A || key.m_bPressedKeyTbl[VK_SPACE])
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToStageSelect");//ゲームシーンを移動する
		}

	}

	void TitleStage::OnDestroy()
	{
		auto& app = App::GetApp();
		auto XAPtr = app->GetXAudio2Manager();
		XAPtr->Stop(m_BGM);
	}
}