/*!
@file Foo.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class StageObject : public GameObject
	{
	protected:
		shared_ptr<Transform> m_transComp;
		shared_ptr<Stage> m_Stage;

		Vec3 m_position; // 位置
		Vec3 m_rotation; // 回転
		Vec3 m_scale;    // スケーリング
	public:
		StageObject(const std::shared_ptr<Stage>& stage);
		virtual ~StageObject(){}

		void OnCreate()override;
		void OnUpdate()override;

		virtual void SetPosition(const Vec3& pos);
		virtual void SetRotation(const Vec3& rot);
		virtual void SetScale(const Vec3& scale);

	};

}
//end basecross
