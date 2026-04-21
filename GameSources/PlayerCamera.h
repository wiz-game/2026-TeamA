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

	public:

		PlayerCamera();

		virtual ~PlayerCamera();

		void SetCameraToPlayerPos();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}