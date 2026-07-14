/*!
@file GameClearStage.h
@brief ステージセレクト
*/

#pragma once
#include "stdafx.h"
#include "JoltManager.h"

namespace basecross
{
	enum StageCount
	{
		stage1,
		stage2
	};

	//--------------------------------------------------------------------------------------
	//	ステージセレクトクラス
	//--------------------------------------------------------------------------------------
	class SelectStage : public Stage
	{
		void CreateViewLight(); //ビューの作成
		void CreateUI();
		void LoadTextures();
		shared_ptr<SoundItem> m_BGM; // サウンド用ポインタ
		StageCount m_stageCount;
		vector<shared_ptr<GameObject>> m_back;
		float phaseTimer;
		float phaseDuration;
		vector<shared_ptr<GameObject>> m_other;

	public:
		//構築と破棄
		SelectStage() :Stage(),
		m_stageCount(stage1),
		phaseTimer(0.0f),
		phaseDuration(1.0f)
		{}
		virtual ~SelectStage() {}

		virtual void OnCreate()override; //初期化
		virtual void OnUpdate()override; //更新
		void OnDestroy()override;

	};
}