#include "stdafx.h"
#include "Project.h"
#include "Sprite.h"

namespace basecross
{
	void Sprite::OnCreate()
	{
		std::vector<VertexPositionColorTexture>  vertices = {
			{Vec3(-50, +50, 0), m_color, Vec2(0, 0)},
			{Vec3(+50, +50, 0), m_color, Vec2(1, 0)},
			{Vec3(-50, -50, 0), m_color, Vec2(0, 1)},
			{Vec3(+50, -50, 0), m_color, Vec2(1, 1)},
		};
		std::vector<uint16_t> indices = { 0, 1, 2, 2, 1, 3 };
		m_drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);

		AddComponent<Transform>();

		m_drawComp->SetTextureResource(m_texName);
		SetAlphaActive(true);

		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_position);
		transComp->SetScale(m_scale);
	}
	void Sprite::OnUpdate()
	{
		OnClear(false);
	}
	void Sprite::OnClear(bool OnOff)
	{
		m_Clear = OnOff;
		if (m_drawComp)
		{
			if (OnOff)
			{
				m_color = m_drawComp->GetDiffuse();
				m_drawComp->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 0.0f));
			}
			else
			{
				m_drawComp->SetDiffuse(m_color);
			}
		}
	}	
}