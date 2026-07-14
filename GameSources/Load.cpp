/*!
@file Load.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//初期化
	void Load::OnCreate()
	{
		auto& app = App::GetApp();
		float width = static_cast<float>(app->GetGameWidth()) / 2.0f;
		float hight = static_cast<float>(app->GetGameHeight()) / 2.0f;

		m_color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vertices = {
			{Vec3(-width, +hight, 0), m_color, Vec2(0, 0)},
			{Vec3(+width, +hight, 0), m_color, Vec2(1, 0)},
			{Vec3(-width, -hight, 0), m_color, Vec2(0, 1)},
			{Vec3(+width, -hight, 0), m_color, Vec2(1, 1)},
		};
		std::vector<uint16_t> indices = { 0, 1, 2, 2, 1, 3 };
		m_drawComp = AddComponent<PCTSpriteDraw>(m_vertices, indices);
		m_drawComp->SetTextureResource(L"TEX_BSQUARE");
		SetAlphaActive(true);

		m_anime = ObjectFactory::Create<LoadAnime>(GetStage(), GetThis<GameObject>());
	}

	void Load::OnUpdate()
	{
		auto& app = App::GetApp();
		auto delta = app->GetElapsedTime();
		auto stage = GetStage();
		if (!stage)return;
		auto gameStage = dynamic_pointer_cast<GameStage>(stage);
		if (!gameStage)return;

		m_time += delta;
		if (m_time > 1.5f)
		{
			//m_color.w = 0.0f;
			gameStage->SetIsLoad(false);
			m_color.w -= delta * 2.0f;
			m_drawComp->SetDiffuse(m_color);
		}
		else
		{
			gameStage->SetIsLoad(true);
		}
		m_anime->OnUpdate();
		if (m_color.w <= 0.0f)
		{
			GetStage()->RemoveGameObject<Load>(GetThis<Load>());
		}
	}

	void Load::OnDraw()
	{
		GameObject::OnDraw();
		m_anime->OnDraw();
	}

	void Load::OnDestroy()
	{
		m_anime->DestroyGameObject();
		m_anime.reset();
	}

	void LoadAnime::OnCreate()
	{
		m_uvNum = 0;
		auto color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vertices = {
			{Vec3(-50, +50, 0), color, Vec2(0, 0)},
			{Vec3(+50, +50, 0), color, Vec2(1, 0)},
			{Vec3(-50, -50, 0), color, Vec2(0, 1)},
			{Vec3(+50, -50, 0), color, Vec2(1, 1)},
		};

		std::vector<uint16_t> indices = { 0, 1, 2, 2, 1, 3 };
		m_drawComp = AddComponent<PCTSpriteDraw>(m_vertices, indices);
		m_drawComp->SetTextureResource(L"TEX_LOADANIMETION");
		SetAlphaActive(true);

		auto transComp = GetComponent<Transform>();
		auto pos = Vec3(400, -300, 0);
		transComp->SetPosition(pos);
	}

	void LoadAnime::OnUpdate()
	{
		auto& app = App::GetApp();
		auto delta = app->GetElapsedTime();
		m_time += delta;
		if (m_time > 0.2f)
		{
			m_time = 0.0f;
			m_uvNum++;
		}
		if (m_uvNum >= 5)
		{
			m_uvNum = 0;
		}
		SetSpriteUV(m_uvNum);
		m_drawComp->UpdateVertices(m_vertices);

		auto obj = m_owner.lock();
		if (!obj) return;
		auto owner = dynamic_pointer_cast<Load>(obj);
		if (!owner)return;

		auto col = owner->GetColor();
		col.w = col.w >= 1.0f ? 1.0f : 0.0f;
		m_drawComp->SetDiffuse(col);
	}

	void LoadAnime::SetSpriteUV(int num)
	{
		auto color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		auto top = 0.0f;
		auto bottom = 1.0f;
		auto left = 200.0f * static_cast<float>(num) / 1024.0f;
		auto right = 200.0f * static_cast<float>(num + 1) / 1024.0f;

		//m_vertices = {
		//	{Vec3(-50, +50, 0), color, Vec2(left, top)},
		//	{Vec3(+50, +50, 0), color, Vec2(right, top)},
		//	{Vec3(-50, -50, 0), color, Vec2(left, bottom)},
		//	{Vec3(+50, -50, 0), color, Vec2(right, bottom)},
		//};

		m_vertices[0].textureCoordinate = Vec2(left, top);
		m_vertices[1].textureCoordinate = Vec2(right, top);
		m_vertices[2].textureCoordinate = Vec2(left, bottom);
		m_vertices[3].textureCoordinate = Vec2(right, bottom);

	}
}
//end basecross
