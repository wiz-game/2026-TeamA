/*!
@file Trampoline.cpp
@brief トランポリン
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Trampoline::Trampoline(const shared_ptr<Stage>& ptrStage)
		: StageObject(ptrStage)
		, m_boundPower(20.0f)
	{
	}

	void Trampoline::OnCreate()
	{
		StageObject::OnCreate();

		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(Vec3(0.0f, -20.0f, -50.0f));
		ptrTrans->SetScale(Vec3(20.0f, 5.0f, 20.0f));

		auto& ptrDraw = AddComponent<PNTStaticDraw>();


		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		ptrColl->SetDrawActive(true);
	}

	void Trampoline::OnCollisionEnter(shared_ptr<GameObject>& ptrObject)
	{

	}
}
//end basecross
