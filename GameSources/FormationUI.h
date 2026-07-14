#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	enum class Formation
	{
		Hammer,
		Slope,
		Spear,
		Bridge
	};

	class FormationUI : public UIManager
	{
		Formation m_formation;
		
		vector<shared_ptr<GameObject>> m_formationUIs;
		shared_ptr<GameObject> m_formationUI;
		vector<shared_ptr<GameObject>> m_counts;
		int m_count;

		void CreateUI();
	public:
		FormationUI(const shared_ptr<Stage>& stage);
		~FormationUI();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		
	};
	
}
