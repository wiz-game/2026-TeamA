/*!
@file Foo.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{

	// コンストラクタ
	Wall::Wall(const std::shared_ptr<Stage>& stage) :
		GameObject(stage)
	{
	}


	// 初期化
	void Wall::OnCreate()
	{
		m_transComp = GetComponent<Transform>();
		m_transComp->SetPosition(m_position);
		m_transComp->SetScale(m_scale);
		m_transComp->SetRotation(m_rotation);

		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
		m_drawComp->SetDiffuse(Col4(0.5f, 0.5f, 0.5f, 1.0f));

		auto collision = AddComponent<CollisionObb>();
		collision->SetFixed(true);

	}

	void Wall::SetPosition(const Vec3& pos)
	{
		m_position = pos;
		if (m_transComp)
		{
			m_transComp->SetPosition(m_position);
		}
	}
	void Wall::SetRotation(const Vec3& rot)
	{
		m_rotation = rot;
		if (m_transComp)
		{
			m_transComp->SetRotation(m_rotation);
		}
	}
	void Wall::SetScale(const Vec3& scale)
	{
		m_scale = scale;
		if (m_transComp)
		{
			m_transComp->SetScale(m_scale);
		}
	}
}

//end basecross
