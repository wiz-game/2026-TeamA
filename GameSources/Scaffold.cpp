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
		StageObject(stage)
	{
	}


	// 初期化
	void Wall::OnCreate()
	{
		StageObject::OnCreate();

		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
		m_drawComp->SetDiffuse(Col4(0.25f, 0.25f, 0.25f, 1.0f));

		auto collision = AddComponent<CollisionObb>();
		collision->SetFixed(true);

	}

}

//end basecross
