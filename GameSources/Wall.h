/*!
@file Foo.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class Wall : public GameObject
	{
	private:
		shared_ptr<Transform> m_transComp;
		shared_ptr<PNTStaticDraw> m_drawComp;

		Vec3 m_position; // 位置
		Vec3 m_rotation; // 回転
		Vec3 m_scale;    // スケーリング
	public:
		Wall(const std::shared_ptr<Stage>& stage);
		virtual ~Wall(){}

		void OnCreate()override;

		void SetPosition(const Vec3& pos);
		void SetRotation(const Vec3& rot);
		void SetScale(const Vec3& scale);

	};

}
//end basecross
