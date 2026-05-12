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

		//void SetPosition(const Vec3& pos);
		//void SetRotation(const Vec3& rot);
		//void SetScale(const Vec3& scale);

	};

}
//end basecross
