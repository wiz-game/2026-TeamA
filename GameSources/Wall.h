/*!
@file Foo.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class Wall : public StageObject
	{
	private:
		shared_ptr<PNTStaticDraw> m_drawComp;
	public:
		Wall(const std::shared_ptr<Stage>& stage);
		virtual ~Wall(){}

		void OnCreate()override;

	};

}
//end basecross
