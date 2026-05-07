#pragma once
#include "stdafx.h"
#include "GameStage.h"

namespace basecross
{
	//class GameStage;

	class PlayerCamera : public Camera
	{
		std::weak_ptr<GameStage> m_gameStage;
		bool m_isFirstFrame;

		void ClarifyMovementDirection();
		Vec3 m_currentCameraForward; //現在のカメラ方向
	public:

		PlayerCamera();

		virtual ~PlayerCamera();

		void SetCameraToPlayerPos();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}