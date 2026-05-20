/*!
 @file SkyBox.h
 @brief 背景のテクスチャ
 @authors Kato Yuki
 */

#pragma once
#include "stdafx.h"

namespace basecross
{
	class Plane : public GameObject
	{
	public:
		Plane(const shared_ptr<Stage>& ptrStage) : GameObject(ptrStage) {}
		~Plane() {}

		void OnCreate() override;
	};

	class SkyBox : public GameObject
	{
	private:
		vector<shared_ptr<GameObject>> m_Planes;

	public:
		SkyBox(const shared_ptr<Stage>& ptrStage) : GameObject(ptrStage) {}
		~SkyBox() {}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override; // 自分で描画を行うためオーバーライドする
	};
}
//end basecross
