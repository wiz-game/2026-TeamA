/*!
@file NumbersUI.cpp
@brief 数値を並べて配置するUI
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//初期化
	void NumbersUI::OnCreate()
	{
		for (int i = 0; i < 4; i++)
		{
			auto sprite =
				ObjectFactory::Create<NumberSprite>(GetStage());//オブジェクトを生成して、ステージには追加しない
			sprite->SetDrawLayer(1);
			auto spriteTrans = sprite->GetComponent<Transform>();
			auto spritePos = spriteTrans->GetPosition();
			auto spriteScale = spriteTrans->GetScale();
			if (i < 2)
				spritePos.x -= i * (102.4f * m_scale.x);
			else
				spritePos.x -= 10 + i * (102.4f * m_scale.x);

			spriteTrans->SetPosition(spritePos + m_position);
			spriteTrans->SetScale(m_scale);
			m_spritesInt.push_back(sprite);
		}

		for (int i = 0; i < 2; i++)
		{
			auto sprite =
				ObjectFactory::Create<NumberSprite>(GetStage());//オブジェクトを生成して、ステージには追加しない
			sprite->SetDrawLayer(1);
			auto spriteTrans = sprite->GetComponent<Transform>();
			auto spritePos = spriteTrans->GetPosition();
			auto spriteScale = spriteTrans->GetScale();
			spritePos.x -= -10 + (-102.4f * (m_scale.x * 1.0f)) + (i * (-102.4f * m_scale.x));
			spriteTrans->SetPosition(spritePos + m_position);
			spriteTrans->SetScale(m_scale);
			m_spritesFloat.push_back(sprite);
		}
	}

	void NumbersUI::OnUpdate()
	{
		int place = 1;//切り出す桁(位)
		int minPlace = 1;
		int fPlace = 10;
		for (auto& sprite : m_spritesInt)
		{
			if (place >= 100)
			{
				int value = (int)m_minute / minPlace % 10;
				minPlace *= 10;
				place *= 10;
				sprite->SetNumber(value);
			}
			else
			{
				int value = (int)m_second / place % 10;
				place *= 10;
				sprite->SetNumber(value);
			}
		}

		for (auto& sprite : m_spritesFloat)
		{
			int value = (int)(m_second * 100) / fPlace % 10;
			fPlace /= 10;
			sprite->SetNumber(value);
		}
	}

	void NumbersUI::OnDraw()
	{
		int place = 1;
		for (const auto& sprite : m_spritesInt)
		{
			int checkValue = 1000 / place;
			place *= 10;

			if (checkValue == 0)
			{
				break;
			}
			sprite->OnDraw();
		}
		int fPlace = 1;
		for (const auto& sprite : m_spritesFloat)
		{
			int checkValue = 10 / fPlace;
			fPlace *= 10;

			if (checkValue == 0)
			{
				break;
			}
			sprite->OnDraw();
		}
		for (const auto& sprite : m_coron)
		{
			sprite->OnDraw();
		}
	}
}
//end basecross
