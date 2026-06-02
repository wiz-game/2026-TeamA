#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Vine::Vine(shared_ptr<Stage>& stage) :
		StageObject(stage),
		m_broken(stage),
		hp(2),
		damage(2)
	{
	}
	Vine::~Vine(){}

	void Vine::OnCreate()
	{
		StageObject::OnCreate();

		m_transComp = GetComponent<Transform>();
		m_transComp->SetScale(10, 10, 10);
		m_transComp->SetPosition(0,50, 0);
		AddComponent<Gravity>();
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");

		m_broken.SetHP(hp);
	}

	void Vine::OnUpdate()
	{
		m_broken.OnUpdate();
		StageObject::OnUpdate();
	}

	void Vine::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Attack"))
		{
			m_broken.SetDamage(damage);
			m_broken.takeDamage();
		}
	}
}
