/*!
@file Load.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	class Load : public GameObject;
	//--------------------------------------------------------------------------------------
	class Load : public GameObject
	{
		shared_ptr<PCTSpriteDraw> m_drawComp;
		std::vector<VertexPositionColorTexture>  m_vertices;
		Col4 m_color;
		float m_time;
		shared_ptr<GameObject> m_anime;
		shared_ptr<GameObject> m_loading;
	public:
		// 構築と破棄
		Load(const shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_color(Col4(1.0f)),
			m_time(0.0f)
		{}
		virtual ~Load()
		{}

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		virtual void OnDraw() override; // 描画
		virtual void OnDestroy() override;
		Col4 GetColor() { return m_color; }
	};

	class LoadAnime : public GameObject
	{
		shared_ptr<PCTSpriteDraw> m_drawComp;
		std::vector<VertexPositionColorTexture>  m_vertices;
		int m_uvNum;
		float m_time;
		weak_ptr<GameObject> m_owner;
		void SetSpriteUV(int num);
	public:
		// 構築と破棄
		LoadAnime(const shared_ptr<Stage>& stage, const shared_ptr<GameObject>& owner) :
			GameObject(stage),
			m_uvNum(0),
			m_time(0),
			m_owner(owner)
		{}
		virtual ~LoadAnime()
		{}

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新

	};

}
//end basecross
