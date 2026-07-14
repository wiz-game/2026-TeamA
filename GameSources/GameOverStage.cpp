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
		CreateViewLight();
		LoadTextures();
		CreateUI();
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto XAPtr = app->GetXAudio2Manager();
		m_BGM = XAPtr->Start(L"GameOverBGM", 0, scene->m_BGMVolume);

	}

	void GameOverStage::OnUpdate()
	{
		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();

		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
		float lStickValue = 0.5f;
		float cursolIndex = 117.0f; //カーソル移動用変数
		// 左スティックの値取得
		Vec2 LStick(pad.fThumbLX, pad.fThumbLY);

		Vec3 btPos = m_bt->GetComponent<Transform>()->GetPosition();
		Vec3 rePos = m_re->GetComponent<Transform>()->GetPosition();

		switch (m_item)
		{
		case Item::BackToTitle:
			if (m_prevLStick.y <= -lStickValue && LStick.y >= -lStickValue)
			{
				m_item = Item::Restart;
				cursolpositon = Vec3(rePos.x - 250, rePos.y, rePos.z);
			}
			if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");//ゲームシーンを移動する
			}

			break;
		case Item::Restart:
			if (m_prevLStick.y <= lStickValue && LStick.y >= lStickValue)
			{
				m_item = Item::BackToTitle;
				cursolpositon = Vec3(btPos.x - 250, btPos.y, btPos.z);
			}
			if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");//ゲームシーンを移動する
			}

			break;
		}
		m_cursol->GetComponent<Transform>()->SetPosition(cursolpositon);
		m_prevLStick = LStick;




	}

	void GameOverStage::CreateUI()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();

		AddGameObject<Sprite>(L"TEX_GameOver", true, Vec3(1024, 256, 0) * 0.01f, Vec3(0, 0, 0));
		m_bt = AddGameObject<Sprite>(L"TEX_BACKTOTITLE", true, Vec3(1024, 256, 0) * 0.005f, Vec3(0, -180, 0));
		m_re = AddGameObject<Sprite>(L"TEX_RESTART", true, Vec3(1024, 256, 0) * 0.005f, Vec3(0, -300, 0));
		m_cursol = AddGameObject<Sprite>(L"TEX_POINTERUI", true, Vec3(200, 200, 0) * 0.01f, cursolpositon); //カーソル
	}

	void GameOverStage::LoadTextures()
	{
		auto& app = App::GetApp();
		auto mediaPath = app->GetDataDirWString();
		auto texPath = mediaPath + L"Textures\\";

		app->RegisterTexture(L"TEX_GameOver", texPath + L"GameOver.png");
		app->RegisterTexture(L"TEX_BACKTOTITLE", texPath + L"backtitle.png");
		app->RegisterTexture(L"TEX_RESTART", texPath + L"restart.png");
	}

	void GameOverStage::OnDestroy()
	{
		auto& app = App::GetApp();
		auto XAPtr = app->GetXAudio2Manager();
		XAPtr->Stop(m_BGM);
	}

}