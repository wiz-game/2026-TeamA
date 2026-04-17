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
			for (int i = 0; i < 10; i++)
			{
				auto player = AddGameObject<Player>(mainPlayerScale*0.5f);

				float angle = (2.0f * XM_PI / 10) * i;   // 10体を均等に配置
				float radius =1.5f;                  // mainPlayer からの距離

				Vec3 offset = Vec3(cos(angle) * radius, 0.0f, sin(angle) * radius);
				player->SetPosition(center + offset);
				m_sabPlayer.push_back(player);
			}			//ビューとライトの作成
			CreateViewLight();

			auto ground = AddGameObject<Ground>();
			SetSharedGameObject(L"Ground", ground);

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
		if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
		{
			RemoveGameObject<GameObject>(m_sabPlayer[0]);
			m_sabPlayer.erase(m_sabPlayer.begin());

			UpdateFormation();
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_B)
		{
			auto player = AddGameObject<Player>(Vec3(0.5f));
			m_sabPlayer.push_back(player);

			UpdateFormation(); 
		}

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

	void GameStage::UpdateFormation()
	{
		if (m_sabPlayer.empty()) return;

		Vec3 center = m_mainPlayer->GetPosition();
		int count = m_sabPlayer.size();
		float radius = 1.5f;

		for (int i = 0; i < count; i++)
		{
			float angle = (2.0f * XM_PI / count) * i;
			Vec3 offset = Vec3(cos(angle) * radius, 0, sin(angle) * radius);

			m_sabPlayer[i]->SetPosition(center + offset);
		}
	}
}
//end basecross
