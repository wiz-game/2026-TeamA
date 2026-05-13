#pragma once
#include "stdafx.h"
#include "Scene.h"

namespace basecross
{
	class Sprite : public GameObject
	{
		Col4 m_color;
		wstring m_texName;
		shared_ptr<PCTSpriteDraw> m_drawComp;
		bool m_Clear;
		Vec3 m_scale;
		Vec3 m_position;

	public:
		Sprite(const shared_ptr<Stage>& stage, const wstring& texName, bool Clear, const Vec3& scale, const Vec3& position) :
			GameObject(stage),
			m_color(1, 1, 1, 1),
			m_texName(texName),
			m_Clear(Clear),
			m_scale(scale),
			m_position(position)
		{}
		virtual ~Sprite()
		{}

		float m_alpha;
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void SetTexture(const wstring& texture)
		{
			if (m_drawComp)
				m_drawComp->SetTextureResource(texture);
		}
		void SetColor(const Col4& color)
		{
			m_color = color;
		}
		virtual void OnClear(bool OnOff);

	};
}