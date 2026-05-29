#include "stdafx.h"
#include "Project.h"


namespace basecross
{
	void Goal::OnCreate()
	{
		StageObject::OnCreate();
		m_stage = GetStage();
		//auto grv = AddComponent<Gravity>();
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
		m_drawComp->SetDiffuse(Col4(1, 0, 1, 1));
	}

	void Goal::OnUpdate()
	{
		StageObject::OnUpdate();

	}

	void Goal::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		auto stage = GetStage();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (other && player)
		{
			auto& app = App::GetApp();
			auto scene = app->GetScene<Scene>();
			wstringstream wss(L"");

			wss << L"GameClear" << endl;
			scene->SetDebugString(wss.str());

			PostEvent(0.0f, GetThis<ObjectInterface>(),scene, L"ToGameClearStage");//ゲームシーンを移動する

		}
	}
}