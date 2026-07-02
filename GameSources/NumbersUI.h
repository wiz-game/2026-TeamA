/*!
@file NumbersUI.h
@brief 数値を並べて配置するUI
*/

#pragma once
#include "stdafx.h"
#include "NumberSprite.h"

namespace basecross 
{


	//--------------------------------------------------------------------------------------
	//	class NumbersUI : public GameObject;
	//--------------------------------------------------------------------------------------
	class NumbersUI : public GameObject
	{
		int m_score;
		float m_second;
		int m_minute;

		Vec3 m_position;
		Vec3 m_scale;

		std::vector<std::shared_ptr<GameObject>> m_coron;
		std::vector<std::shared_ptr<NumberSprite>> m_spritesInt;
		std::vector<std::shared_ptr<NumberSprite>> m_spritesFloat;
	public:
		// 構築と破棄
		NumbersUI(const shared_ptr<Stage>& stage, const Vec3 scale, const Vec3 position) :
			GameObject(stage),
			m_position(position),
			m_scale(scale),
			m_score(100)
		{}
		virtual ~NumbersUI()
		{}

		void SetTimer(float time, int minute)
		{
			m_second = time;
			m_minute = minute;
		}

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		virtual void OnDraw() override; // 描画
	};

}
//end basecross
