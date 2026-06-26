#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class PreviewButtonUI : public UIManager
	{
		shared_ptr<GameObject> m_buttonUI;
		void CreateUI();

	public:
		PreviewButtonUI(const shared_ptr<Stage>& stage);
		~PreviewButtonUI();

		virtual void OnCreate()override;
	};
}