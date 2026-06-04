#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class Baite : public GameObject
	{
		shared_ptr<Transform> m_transComp; // トランスフォームコンポーネント
		shared_ptr<PNTStaticDraw> m_drawComp; // ドローコンポーネント
		Vec3 pos;
		bool m_isPicked;
		float m_speed;

	public:
		Baite(shared_ptr<Stage>& stage);
		~Baite();

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& other)override;

	};

	class BigBaite : public Baite
	{
		shared_ptr<Transform> m_transComp;

	public:
		BigBaite(shared_ptr<Stage>& stage);
		~BigBaite();

		void OnCreate()override;
		void OnUpdate()override;
		void OnCollisionEnter(shared_ptr<GameObject>& other)override;

	};
}