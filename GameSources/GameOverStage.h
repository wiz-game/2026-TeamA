/*!
@file GameOverStage.h
@brief ゲームオーバーステージ
*/

#pragma once
#include "stdafx.h"
#include "JoltManager.h"

namespace basecross
{
	enum class OvStageItem
	{
		BackToTitle,
		Restart
	};

	//--------------------------------------------------------------------------------------
	//	ゲームオーバーステージクラス
	//--------------------------------------------------------------------------------------
	class GameOverStage : public Stage
	{
		void CreateViewLight(); //ビューの作成
		void CreateUI();
		void LoadTextures();
		shared_ptr<SoundItem> m_BGM; // サウンド用ポインタ
		shared_ptr<GameObject> m_cursol; //カーソル用ポインタ
		Vec3 cursolposition; //カーソル位置更新用変数
		OvStageItem m_item;
		shared_ptr<GameObject> m_bt; //backtotitle用
		shared_ptr<GameObject> m_re; //restart用
		Vec2 m_prevLStick;
		shared_ptr<GameObject> m_bg;

	public:
		//構築と破棄
		GameOverStage() :Stage(),m_item(OvStageItem::BackToTitle){}
		virtual ~GameOverStage() {}

		virtual void OnCreate()override; //初期化
		virtual void OnUpdate()override; //更新
		void OnDestroy()override;

	};
}