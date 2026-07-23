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
		CreateViewLight();
		LoadTextures();
		CreateUI();
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto XAPtr = app->GetXAudio2Manager();
		m_BGM = XAPtr->Start(L"GameOverBGM", 0, scene->m_BGMVolume);


	}

	void GameClearStage::CreateUI()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();

		AddGameObject<Sprite>(L"TEX_GameClear", true, Vec3(1024, 256, 0) * 0.01f, Vec3(0, 270, 0));
		m_bt = AddGameObject<Sprite>(L"TEX_BACKTOTITLE", true, Vec3(1024, 256, 0) * 0.005f, Vec3(-300, -300, 0));
		m_re = AddGameObject<Sprite>(L"TEX_RESTART", true, Vec3(1024, 256, 0) * 0.005f, Vec3(300, -300, 0));
		auto bg = AddGameObject<Sprite>(L"TEX_GameClearBG", true, Vec3(1920, 1080, 0) * 0.008f, Vec3(0, 0, 0));
		bg->SetDrawLayer(-1);

		Vec3 btPos = m_bt->GetComponent<Transform>()->GetPosition();
		cursolposition = Vec3(btPos.x - 220, btPos.y, btPos.z);

		m_cursol = AddGameObject<Sprite>(L"TEX_POINTERUI", true, Vec3(200, 200, 0) * 0.01f, cursolposition); //カーソル
	}


	void GameClearStage::OnUpdate()
	{
		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();

		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
		auto key = input.GetKeyState();
		float lStickValue = 0.5f;
		float cursolIndex = 117.0f; //カーソル移動用変数
		// 左スティックの値取得
		Vec2 LStick(pad.fThumbLX, pad.fThumbLY);

		Vec3 btPos = m_bt->GetComponent<Transform>()->GetPosition();
		Vec3 rePos = m_re->GetComponent<Transform>()->GetPosition();

		switch (m_item)
		{
		case ClStageItem::BackToTitle:
			if (m_prevLStick.x <= lStickValue && LStick.x >= lStickValue || key.m_bPressedKeyTbl[VK_RIGHT])
			{
				m_item = ClStageItem::Restart;
				cursolposition = Vec3(rePos.x - 220, rePos.y, rePos.z);
			}
			if (pad.wPressedButtons & XINPUT_GAMEPAD_A || key.m_bPressedKeyTbl[VK_SPACE])
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");//ゲームシーンを移動する
			}

			break;
		case ClStageItem::Restart:
			if (m_prevLStick.x <= -lStickValue && LStick.x >= -lStickValue || key.m_bPressedKeyTbl[VK_LEFT])
			{
				m_item = ClStageItem::BackToTitle;
				cursolposition = Vec3(btPos.x - 220, btPos.y, btPos.z);
			}
			if (pad.wPressedButtons & XINPUT_GAMEPAD_A || key.m_bPressedKeyTbl[VK_SPACE])
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");//ゲームシーンを移動する
			}

			break;
		}
		m_cursol->GetComponent<Transform>()->SetPosition(cursolposition);
		m_prevLStick = LStick;

	}


	void GameClearStage::LoadTextures()
	{
		auto& app = App::GetApp();
		auto mediaPath = app->GetDataDirWString();
		auto texPath = mediaPath + L"Textures\\";

		app->RegisterTexture(L"TEX_GameClear", texPath + L"GameClear.png");
		app->RegisterTexture(L"TEX_GameClearBG", texPath + L"clear.jpg");
		app->RegisterTexture(L"TEX_BACKTOTITLE", texPath + L"backtitle.png");
		app->RegisterTexture(L"TEX_RESTART", texPath + L"restart.png");

	}

	void GameClearStage::OnDestroy()
	{
		auto& app = App::GetApp();
		auto XAPtr = app->GetXAudio2Manager();
		XAPtr->Stop(m_BGM);
	}

}