#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class FormationCountUI : public UIManager
	{
		shared_ptr<GameObject> m_countUI;
		void CreateUI();
	public:
		FormationCountUI(const shared_ptr<Stage>& stage);
		~FormationCountUI();

		void OnCreate()override;

	};
}