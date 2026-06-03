/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"
#include "JoltManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage
	{
		JoltManager m_jphManger; // Jolt Physics マネージャー

		void CreateViewLight(); //ビューの作成
		void LoadTextures();


		shared_ptr<Player> m_mainPlayer; // メインプレイヤー
		Vec3 m_mainPlayerScale; // メインプレイヤーのスケーリング
		std::shared_ptr<SingleView> m_playerCameraView;	

		shared_ptr<SoundItem> m_BGM; // サウンド用ポインタ
		bool m_isActive;
		shared_ptr<Option> option;
		vector<shared_ptr<GameObject>> m_stageObj;
	public:
		//構築と破棄
		GameStage() :Stage(){}
		virtual ~GameStage() {}
		
		void SetStageObj(shared_ptr<GameObject> newStageObj)
		{
			m_stageObj.push_back(newStageObj);
		}

		void SetIsActive(bool isActive)
		{
			m_isActive = isActive;

			for (auto& obj : m_stageObj)
			{
				obj->SetUpdateActive(m_isActive);
			}
		}

		bool GetIsActive() const 
		{
			return m_isActive;
		}



		virtual void OnCreate()override; //初期化
		virtual void OnUpdate()override; //更新
		virtual void OnUpdate2() override;
		virtual void OnDraw()override; //描画
		void OnDestroy()override;

	};
}
//end basecross

