#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class TurningPoint : public StageObject
	{
		shared_ptr<Transform> m_transComp;
		shared_ptr<PNTStaticDraw> m_drawComp;

		Vec3 nextEye; //このオブジェクトに触れた際の次のカメラ位置座標
		Vec3 nextAt;  //このオブジェクトに触れた際の次のカメラの注視点座標
		//shared_ptr<PlayerCamera>* playerCamera;
		bool m_trigger;
		void OnCollisionEnter(shared_ptr<GameObject>& other)override;

	public:
		TurningPoint(const shared_ptr<Stage>& stage);
		~TurningPoint();


		Vec3 GetPosition() const
		{
			return m_position;
		}

		bool GetTrigger()
		{
			return m_trigger;
		}
		
		void OnCreate() override;
		void OnUpdate() override;

	};
}