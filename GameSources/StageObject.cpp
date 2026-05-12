/*!
@file Foo.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{

	// コンストラクタ
	StageObject::StageObject(const std::shared_ptr<Stage>& stage) :
		GameObject(stage),
		m_position(0.0f, 0.0f, 0.0f),
		m_rotation(0.0f, 0.0f, 0.0f),
		m_scale(1.0f, 1.0f, 1.0f)
	{
	}


	// 初期化
	void StageObject::OnCreate()
	{
		m_transComp = GetComponent<Transform>();

		m_transComp->SetPosition(m_position);
		m_transComp->SetScale(m_scale);
		m_transComp->SetRotation(m_rotation);

		auto collision = AddComponent<CollisionObb>();
		collision->SetFixed(true);

	}

	void StageObject::OnUpdate()
	{

	}

	void StageObject::SetPosition(const Vec3& pos)
	{
		m_position = pos;
		if (m_transComp)
		{
			m_transComp->SetPosition(m_position);
		}
	}
	void StageObject::SetRotation(const Vec3& rot)
	{
		m_rotation = rot;
		if (m_transComp)
		{
			m_transComp->SetRotation(m_rotation);
		}
	}
	void StageObject::SetScale(const Vec3& scale)
	{
		m_scale = scale;
		if (m_transComp)
		{
			m_transComp->SetScale(m_scale);
		}
	}
}

//end basecross
