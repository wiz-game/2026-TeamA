#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class TurningPoint : public StageObject
	{
		shared_ptr<Transform> m_transComp;
		Vec3 m_scale;
		Vec3 m_rotation;
		Vec3 m_position;

		void OnCollisionEnter(shared_ptr<GameObject>& other);

		Vec3 nextEye; //このオブジェクトに触れた際の次のカメラ位置座標
		Vec3 nextAt;  //このオブジェクトに触れた際の次のカメラの注視点座標
		PlayerCamera* playerCamera;

	public:
		TurningPoint(const shared_ptr<Stage>& stage);
		~TurningPoint();

		void SetPosition(Vec3 newPosition)
		{
			m_position = newPosition;
		}

		Vec3 GetPosition() const
		{
			return m_position;
		}
		
		void OnCreate() override;
		void OnUpdate() override;

	};
}