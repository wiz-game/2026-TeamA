/*!
@file NumberSprites.h
@brief 数字のスプライト
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	class NumberSprite : public GameObject
	{
	public:

		Col4 m_color;
		std::vector<VertexPositionColorTexture> m_vertices;
		std::vector<uint16_t> indices;
		shared_ptr<PCTSpriteDraw> m_drawComp;
		shared_ptr<Transform> m_transComp;

		float m_rotate;

		int m_number;
		// 構築と破棄
		NumberSprite(const shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_color(Col4(1, 1, 1, 1)),
			m_rotate(0.0f),
			m_number(0)
		{}
		virtual ~NumberSprite()
		{}

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		//virtual void OnDraw() override; // 描画

		//表示する数値を設定する
		void SetNumber(int num);

		//現在管理している数値を返す
		int GetNumber() const
		{
			return m_number;
		}
	};

}
//end basecross
