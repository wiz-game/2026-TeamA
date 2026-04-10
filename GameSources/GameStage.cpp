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
		auto camera = ObjectFactory::Create<Camera>();
		camera->SetEye(Vec3(0.0f, 8.0f, -8.0f));
		camera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		// ビューにカメラを設定
		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		//マルチライトの作成
		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting(); //デフォルトのライティングを指定
	}

	void GameStage::OnCreate() {
		try {
			auto& app = App::GetApp();

			// JoltPhysicsを初期化する
			m_jphManger.Initialize();
			auto mainPlayer = AddGameObject<Player>(); // プレイヤーオブジェクトを追加
			Vec3 center = mainPlayer->GetPosition();
			Vec3 mainPlayerScale = mainPlayer->GetScale();
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
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::OnUpdate()
	{
		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();

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
