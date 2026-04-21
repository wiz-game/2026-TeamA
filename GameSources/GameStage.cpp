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

			// JoltPhysicsを初期化する
			m_jphManger.Initialize();
			m_mainPlayer = AddGameObject<Player>(); // プレイヤーオブジェクトを追加
			SetSharedGameObject(L"Player", m_mainPlayer);
			m_mainPlayer->SetPosition(Vec3(0, 2, 0));
			Vec3 center = m_mainPlayer->GetPosition();
			Vec3 mainPlayerScale = m_mainPlayer->GetScale();
			//ビューとライトの作成
			CreateViewLight();

			auto ground = AddGameObject<Ground>();
			SetSharedGameObject(L"Ground", ground);
			auto goal = AddGameObject<Goal>();
			SetSharedGameObject(L"Goal", goal);
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
		////デバック用
		wstringstream wss(L"");



		//scene->SetDebugString(wss.str());
		if (pad.wPressedButtons & XINPUT_GAMEPAD_X)
		{
			wss<<L"ハンマー" << endl;;
			scene->SetDebugString(wss.str());
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_Y)
		{
			wss << L"キューブ" << endl;;
			scene->SetDebugString(wss.str());
		}

	}

	void GameStage::OnUpdate2()
	{
		m_jphManger.Update(1.0f / 60.0f);
	}

	void GameStage::OnDraw()
	{
	}

}
//end basecross
