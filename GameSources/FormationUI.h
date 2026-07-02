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
		shared_ptr<PCTSpriteDraw> m_yz_RDrawComp;
		shared_ptr<PCTSpriteDraw> m_yz_LDrawComp;


		void CreateUI();
	public:
		FormationUI(const shared_ptr<Stage>& stage);
		~FormationUI();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		
	};
	
}
