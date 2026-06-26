/*!
@file Foo.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class Trampoline : public StageObject
	{
	public:
		Trampoline(const shared_ptr<Stage>& stage);
		virtual ~Trampoline() {}

		virtual void OnCreate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& ptrObject) override;

		float GetBoundPower() const { return m_boundPower; }

	private:
		float m_boundPower;
	};

}
//end basecross
