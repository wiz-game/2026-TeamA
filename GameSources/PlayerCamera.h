#pragma once
#include "stdafx.h"
#include "GameStage.h"

namespace basecross
{
	class GameStage;
	class TurningPoint;

	enum CameraAngleState
	{
		Center,
		Right,
		Left
	};

	class PlayerCamera : public Camera
	{
		CameraAngleState m_cameraAngleState;
		std::weak_ptr<GameStage> m_gameStage;
		bool m_isFirstFrame;
		bool m_changeAngle;
		Vec2 m_prevRStick;
		float m_baseYaw;     // 通常時の角度
		float m_offsetYaw;   // 右スティックで加える角度

		void ClarifyMovementDirection();
		Vec3 m_currentCameraForward; //現在のカメラ方向

		const float StickThreshold = 0.25f; // スティック入力の閾値
		const float CameraYawRight = 40.0f; // 右に向く角度
		const float CameraDistance = 18.0f; // プレイヤーとの距離
		const float CameraHeight = 13.0f;   // カメラの高さ
		const float CameraLookAhead = 8.0f; // 注視点の前方距離
		const float CameraLerpSpeed = 6.0f; // 補間速度
		const Vec3 CameraUp = Vec3(0, 1, -0.5f);   // カメラの傾き補正

		Vec3 m_nextEye; //カメラアングル切り替え用
		Vec3 m_nextAt;  //カメラアングル切り替え用

		vector<shared_ptr<TurningPoint>> m_tp;
		//shared_ptr<TurningPoint> m_tp;

		bool On;
	public:

		PlayerCamera();

		virtual ~PlayerCamera();

		void SetCameraToPlayerPos();
		void ChangeAngle();
		void SetCameraAngleState(CameraAngleState newAngle)
		{
			m_cameraAngleState = newAngle;
		}

		void SetNextCameraAngle(Vec3 nextEye, Vec3 nextAt)
		{
			m_nextEye = nextEye;
			m_nextAt = nextAt;
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}