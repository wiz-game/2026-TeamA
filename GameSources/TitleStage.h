/*!
@file GameClearStage.h
@brief タイトルステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	//	タイトルステージクラス
	//--------------------------------------------------------------------------------------

	class TitleStage : public Stage
	{
		void CreateViewLight(); //ビューの作成
		void CreateUI();
		shared_ptr<SoundItem> m_BGM; // サウンド用ポインタ

	public:
		//構築と破棄
		TitleStage() :Stage() {}
		virtual ~TitleStage() {}

		virtual void OnCreate()override; //初期化
		virtual void OnUpdate()override; //更新
		void OnDestroy()override;
	};
}