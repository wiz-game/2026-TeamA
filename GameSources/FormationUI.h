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

	class FormationUI : public GameObject
	{
		Formation m_formation;
		void CreateUI();
		vector<shared_ptr<GameObject>> m_formationUI;
	public:
		FormationUI(const shared_ptr<Stage>& stage);
		~FormationUI();

		void OnCreate() override;
		void OnUpdate() override;

	};
	
}
