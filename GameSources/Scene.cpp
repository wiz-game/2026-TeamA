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
		app->RegisterWav(L"TitleBGM", audioPath + L"titlebgm.wav");
		app->RegisterWav(L"GameOverBGM", audioPath + L"gameoverbgm.wav");
		app->RegisterWav(L"GameClearBGM", audioPath + L"gameclearbgm.wav");

		// テクスチャの読込
		app->RegisterTexture(L"TEX_PAUSEUI", texPath + L"Pause.png");
		app->RegisterTexture(L"TEX_POINTERUI", texPath + L"pointer.png");
		app->RegisterTexture(L"TEX_BGMICON", texPath + L"BGMIcon.png");
		app->RegisterTexture(L"TEX_SEICON", texPath + L"SEIcon.png");
		app->RegisterTexture(L"TEX_BSQUARE", texPath + L"BSquare.png");
		app->RegisterTexture(L"TEX_ALPHA", texPath + L"alphablended.png");
		app->RegisterTexture(L"TEX_YZUI", texPath + L"yz.png");
		app->RegisterTexture(L"TEX_YZUI_R", texPath + L"yz_r.png");
		app->RegisterTexture(L"TEX_YZUI_L", texPath + L"yz_l.png");
		app->RegisterTexture(L"TEX_HAMMERUI", texPath + L"hammer.png");
		app->RegisterTexture(L"TEX_SPEARUI", texPath + L"spear.png");
		app->RegisterTexture(L"TEX_STAIRSUI", texPath + L"stairs.png");
		app->RegisterTexture(L"TEX_BRIDGEUI", texPath + L"bridge.png");
		app->RegisterTexture(L"TEX_TITLEUI", texPath + L"title.png");
		app->RegisterTexture(L"TEX_PREVIEWUI", texPath + L"preview.png");
		app->RegisterTexture(L"TEX_PAUSEBUTTONUI", texPath + L"pausebutton.png");
		app->RegisterTexture(L"TEX_FORMATIONCOUNTUI", texPath + L"fc.png");
		app->RegisterTexture(L"TEX_NUMBER", texPath + L"number.png");
		app->RegisterTexture(L"TEX_NUMBER2", texPath + L"number2.png");
		app->RegisterTexture(L"TEX_NUMBER2", texPath + L"number2.png");
		app->RegisterTexture(L"TEX_APUSHUI", texPath + L"pushA.png");

		app->RegisterTexture(L"TEX_LOADANIMETION", texPath + L"rdrp.png");
		app->RegisterTexture(L"TEX_LOADINGUI", texPath + L"loading.png");

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
		app->RegisterTexture(L"TEX_BOARD", modelPath + L"Board\\T_Board.png");
		auto boardModel = MeshResource::CreateStaticModelMesh(modelPath + L"Board\\", L"M_Board.bmf");
		boardModel->SetTextureResource(L"TEX_BOARD");
		app->RegisterResource(L"MODEL_BOARD", boardModel);

		// 土モデルの読み込み
		app->RegisterTexture(L"TEX_SOIL", modelPath + L"Block\\T_Soil.png");
		auto soilModel = MeshResource::CreateStaticModelMesh(modelPath + L"Block\\", L"M_Soil.bmf");
		soilModel->SetTextureResource(L"TEX_SOIL");
		app->RegisterResource(L"MODEL_SOIL", soilModel);

		// 倒木モデルの読み込み
		app->RegisterTexture(L"TEX_FALLENTREE", modelPath + L"FallenTree\\T_FallenTree.png");
		auto fallenTreeModel = MeshResource::CreateStaticModelMesh(modelPath + L"FallenTree\\", L"M_FallenTree.bmf");
		fallenTreeModel->SetTextureResource(L"TEX_FALLENTREE");
		app->RegisterResource(L"MODEL_FALLENTREE", fallenTreeModel);

		// 柵モデルの読み込み
		app->RegisterTexture(L"TEX_FENCE", modelPath + L"Fence\\T_Fence.png");
		auto fenceModel = MeshResource::CreateStaticModelMesh(modelPath + L"Fence\\", L"M_Fence.bmf");
		fenceModel->SetTextureResource(L"TEX_FENCE");
		app->RegisterResource(L"MODEL_FENCE", fenceModel);

		// 木モデルの読み込み
		app->RegisterTexture(L"TEX_FIRTREE", modelPath + L"FirTree\\T_FirTree.png");
		auto firTreeModel = MeshResource::CreateStaticModelMesh(modelPath + L"FirTree\\", L"M_FirTree.bmf");
		firTreeModel->SetTextureResource(L"TEX_FIRTREE");
		app->RegisterResource(L"MODEL_FIRTREE", firTreeModel);

		// 足場モデルの読み込み
		app->RegisterTexture(L"TEX_FOOTHOLD", modelPath + L"Foothold\\T_Foothold.png");
		auto footholdModel = MeshResource::CreateStaticModelMesh(modelPath + L"Foothold\\", L"M_Foothold.bmf");
		footholdModel->SetTextureResource(L"TEX_FOOTHOLD");
		app->RegisterResource(L"MODEL_FOOTHOLD", footholdModel);

		//　キノコモデルの読み込み
		app->RegisterTexture(L"TEX_MUSHROOM", modelPath + L"Mushroom\\T_Mushroom.png");
		auto mushroomModel = MeshResource::CreateStaticModelMesh(modelPath + L"Mushroom\\", L"M_Mushroom.bmf");
		app->RegisterResource(L"MODEL_MUSHROOM", mushroomModel);

		// ロングキノコモデルの読み込み
		app->RegisterTexture(L"TEX_LONG_MUSHROOM", modelPath + L"Mushroom\\T_LongMushroom.png");
		auto longMushroomModel = MeshResource::CreateStaticModelMesh(modelPath + L"Mushroom\\", L"M_LongMushroom.bmf");
		app->RegisterResource(L"MODEL_LONG_MUSHROOM", longMushroomModel);

		// 低木の読み込み
		app->RegisterTexture(L"TEX_SHRUB", modelPath + L"Shrub\\T_Shrub.png");
		auto bushModel = MeshResource::CreateStaticModelMesh(modelPath + L"Shrub\\", L"M_Shrub.bmf");
		bushModel->SetTextureResource(L"TEX_SHRUB");
		app->RegisterResource(L"MODEL_SHRUB", bushModel);

		// スロープモデルの読み込み
		app->RegisterTexture(L"TEX_SLOPE", modelPath + L"Slope\\T_Slope.png");
		auto slopeModel = MeshResource::CreateStaticModelMesh(modelPath + L"Slope\\", L"M_Slope.bmf");
		slopeModel->SetTextureResource(L"TEX_SLOPE");
		app->RegisterResource(L"MODEL_SLOPE", slopeModel);

		// 石モデルの読み込み
		app->RegisterTexture(L"TEX_STONE", modelPath + L"Stone\\T_Stone.png");
		auto stoneModel = MeshResource::CreateStaticModelMesh(modelPath + L"Stone\\", L"M_Stone.bmf");
		stoneModel->SetTextureResource(L"TEX_STONE");
		app->RegisterResource(L"MODEL_STONE", stoneModel);

		// ポンの読み込み
		app->RegisterTexture(L"TEX_PON", modelPath + L"Pon\\T_Pon.png");
		auto ponModel = MultiMeshResource::CreateBoneModelMultiMesh(modelPath + L"Pon\\", L"M_Pon.bmf");
		ponModel->SetTextureResource(L"TEX_PON", 1);
		app->RegisterResource(L"MODEL_PON", ponModel);

		// レプリンの読み込み
		app->RegisterTexture(L"TEX_REPURIN", modelPath + L"Repurin\\T_Repurin.png");
		auto RepurinModel = MultiMeshResource::CreateBoneModelMultiMesh(modelPath + L"Repurin\\", L"M_Repurin.bmf");
		RepurinModel->SetTextureResource(L"TEX_REPURIN", 1);
		app->RegisterResource(L"MODEL_REPURIN", RepurinModel);

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
			//PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToTitleStage");

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
		if (event->m_MsgStr == L"ToTitleStage")
		{
			ResetActiveStage<TitleStage>();
		}
		if (event->m_MsgStr == L"ToStageSelect")
		{
			ResetActiveStage<SelectStage>();
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
