#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class PauseButtonUI : public UIManager
	{
		shared_ptr<GameObject> m_buttonUI;
		void CreateUI();
	public:
		PauseButtonUI(const shared_ptr<Stage>& stage);
		~PauseButtonUI();

		void OnCreate()override;
	};
}