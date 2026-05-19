/*!
@file Scene.cpp
@brief シーン実装
*/

#include "stdafx.h"
#include "Project.h"
#include "DXModelResource.h"
#include "JoltManager.h"
#include "Scene.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::CreateResourses() 
	{
		// アプリケーションオブジェクトを取得する
		auto& app = App::GetApp();

		// メディアフォルダーのパスを取得する
		auto mediaPath = app->GetDataDirWString();

		// リソースの読込

		// playerモデルの読み込む
		app->RegisterTexture(L"TEX_PLAYER", mediaPath + L"Models\\Player\\Sample_Player.png");
		auto playerModel = MeshResource::CreateStaticModelMesh(mediaPath + L"Models\\Player\\",L"Sample_Player.bmf");
		playerModel->SetTextureResource(L"TEX_PLAYER");
		app->RegisterResource(L"MODEL_PLAYER", playerModel);

	}


	void Scene::OnCreate()
	{
		try {
			JoltManager::StaticInitialize();

			// 背景色を設定
			SetClearColor(Col4(0.0f, 0.11328125f, 0.2578125, 1.0f)); // ミッドナイトブルー
			
			//リソース作成
			CreateResourses();

			//自分自身にイベントを送る
			//これによりゲームステージのオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");

			//GameManager::CreateGameManager();


		}
		catch (...) {
			throw;
		}
	}
	
	void Scene::OnEvent(const shared_ptr<Event>& event) 
	{
		if (event->m_MsgStr == L"ToGameStage") {
			//ゲームステージの設定
			ResetActiveStage<GameStage>();

			StageManager::CreateStageManager();

			StageManager::GetStageManager()->LoadStageFromCSV(L"stage1.csv");
		}
		if (event->m_MsgStr == L"ToGameClearStage")
		{
			ResetActiveStage<GameClearStage>();
		}
		if (event->m_MsgStr == L"ToGameOverStage")
		{
			ResetActiveStage<GameOverStage>();
		}
	}
	Scene::~Scene() {
		JoltManager::StaticTerminate();
	}

}
//end basecross
