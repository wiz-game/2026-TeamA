/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------

	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		// カメラの設定
		//auto camera = ObjectFactory::Create<Camera>();
		//camera->SetEye(Vec3(0.0f, 8.0f, -8.0f));
		//camera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		m_playerCameraView = ObjectFactory::Create<SingleView>(GetThis<Stage>());
		auto playerCamera = ObjectFactory::Create<PlayerCamera>();
		auto camera = ObjectFactory::Create<Camera>();
		camera->SetEye(Vec3(0.0f, 8.0f, -15.0f));
		camera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		m_playerCameraView->SetCamera(playerCamera);
		SetView(m_playerCameraView);

		//// ビューにカメラを設定
		//auto view = CreateView<SingleView>();
		//view->SetCamera(camera);

		//マルチライトの作成
		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting(); //デフォルトのライティングを指定
	}

	void GameStage::OnCreate() {
		try {
			auto& app = App::GetApp();
			auto scene = app->GetScene<Scene>();
			LoadTextures();
			AddGameObject<SkyBox>();

			// JoltPhysicsを初期化する
			m_jphManger.Initialize();
			m_mainPlayer = AddGameObject<Player>(); // プレイヤーオブジェクトを追加
			SetSharedGameObject(L"Player", m_mainPlayer);
			m_stageObj.push_back(m_mainPlayer);
			m_mainPlayer->SetPosition(Vec3(0, 2, 0));
			Vec3 center = m_mainPlayer->GetPosition();
			Vec3 mainPlayerScale = m_mainPlayer->GetScale();
			//ビューとライトの作成
			CreateViewLight();
			m_isActive = true;
			AddGameObject<FormationUI>();
			option = AddGameObject<Option>(); //オプションを追加しておくが、表示はしない
			SetSharedGameObject(L"Option", option);

			//StageBGM
			auto XAPtr = App::GetApp()->GetXAudio2Manager();
			m_BGM = XAPtr->Start(L"StageBGM", XAUDIO2_LOOP_INFINITE, App::GetApp()->GetScene<Scene>()->m_BGMVolume);
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::OnUpdate()
	{
		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();
		auto scene = App::GetApp()->GetScene<Scene>();
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
		//デバック用
		wstringstream wss(L"");

		if (m_isActive)
		{
			// ゲーム中：STARTボタンでポーズ開始
			if (pad.wPressedButtons & XINPUT_GAMEPAD_START)
			{
				SetIsActive(false);       // 関数を使って停止させる
				option->SetVisible(true); // メニュー表示
			}
		}
		else
		{
			if (pad.wPressedButtons & XINPUT_GAMEPAD_START)
			{
				SetIsActive(true);
				option->SetVisible(false);

			}
		}
		m_BGM->m_SourceVoice->SetVolume(scene->m_BGMVolume); //���ʕύX�̍X�V
	}

	void GameStage::OnUpdate2()
	{
		m_jphManger.Update(1.0f / 60.0f);
	}

	void GameStage::OnDraw()
	{
	}

	void GameStage::LoadTextures()
	{
		auto& app = App::GetApp();
		auto mediaPath = app->GetDataDirWString();
		auto texPath = mediaPath + L"Textures\\";

		app->RegisterTexture(L"SkyBoxBelow", texPath + L"SkyBoxBelow.png");
		app->RegisterTexture(L"SkyBoxHorizontal", texPath + L"SkyBoxHorizontal.png");
		app->RegisterTexture(L"SkyBoxTop", texPath + L"SkyBoxTop.png");
		app->RegisterTexture(L"000_000_000", texPath + L"000_000_000.bmp");
		app->RegisterTexture(L"000_090_000", texPath + L"000_090_000.bmp");
		app->RegisterTexture(L"000_180_000", texPath + L"000_180_000.bmp");
		app->RegisterTexture(L"000_270_000", texPath + L"000_270_000.bmp");
		app->RegisterTexture(L"090_000_000", texPath + L"090_000_000.bmp");
		app->RegisterTexture(L"270_000_000", texPath + L"270_000_000.bmp");

	}

}
//end basecross
