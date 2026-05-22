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
		// テクスチャフォルダーのパスを取得する
		auto texPath = mediaPath + L"Textures\\";
		// オーディオフォルダーのパスを取得する
		auto audioPath = mediaPath + L"Sounds\\";
		// モデルフォルダーのパスを取得する
		auto modelPath = mediaPath + L"Models\\";

		//オーディオの登録
		app->RegisterWav(L"StageBGM", audioPath + L"stagebgm.wav");

		// リソースの読込

		// playerモデルの読み込み
		app->RegisterTexture(L"TEX_PLAYER", mediaPath + L"Models\\Player\\Sample_Player.png");
		auto playerModel = MeshResource::CreateStaticModelMesh(mediaPath + L"Models\\Player\\",L"Sample_Player.bmf");
		playerModel->SetTextureResource(L"TEX_PLAYER");
		app->RegisterResource(L"MODEL_PLAYER", playerModel);

		// Blockモデルの読み込み
		app->RegisterTexture(L"TEX_GRAND", mediaPath + L"Models\\Block\\T_Grand.png");
		auto blockModel = MeshResource::CreateStaticModelMesh(mediaPath + L"Models\\Block\\", L"M_Grand.bmf");
		blockModel->SetTextureResource(L"TEX_GRAND");
		app->RegisterResource(L"MODEL_BLOCK", blockModel);

		// Boardモデルの読み込み（仮）
		app->RegisterTexture(L"TEX_BOARD", modelPath + L"Board\\T_FallenTree.png");
		auto boardModel = MeshResource::CreateStaticModelMesh(modelPath + L"Board\\", L"M_FallenTree.bmf");
		boardModel->SetTextureResource(L"TEX_BOARD");
		app->RegisterResource(L"MODEL_BOARD", boardModel);
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

			StageManager::GetStageManager()->LoadStageFromCSV(L"stage1_1.csv");
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
