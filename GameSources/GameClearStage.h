/*!
@file GameClearStage.h
@brief ゲームクリアステージ
*/

#pragma once
#include "stdafx.h"
#include "JoltManager.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	ゲームクリアステージクラス
	//--------------------------------------------------------------------------------------
	class GameClearStage : public Stage
	{
		void CreateViewLight(); //ビューの作成
		void CreateUI();
		void LoadTextures();
		shared_ptr<SoundItem> m_BGM; // サウンド用ポインタ

	public:
		//構築と破棄
		GameClearStage() :Stage() {}
		virtual ~GameClearStage() {}

		virtual void OnCreate()override; //初期化
		virtual void OnUpdate()override; //更新
		void OnDestroy()override;

	};
}