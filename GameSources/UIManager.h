#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class UIManager : public GameObject
	{
		void CreateUI();
	public:
		UIManager(const shared_ptr<Stage>& stage);
		~UIManager();

		vector<shared_ptr<GameObject>> m_gameUI;

		void SetUI(shared_ptr<GameObject> ui)
		{
			m_gameUI.push_back(ui);
		}

		void SetVisible(bool visible);
		

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}