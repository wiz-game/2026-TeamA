#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	PlayerCamera::PlayerCamera() :
		Camera(),
		m_isFirstFrame(true)
	{
	}
	PlayerCamera::~PlayerCamera() {}

	void PlayerCamera::OnCreate()
	{
		//auto gameStage = m_gameStage.lock();
		//if (!gameStage) return;
		//auto player = gameStage->GetSharedGameObject<Player>(L"Player");
		//if (!player) return;

		//m_currentCameraForward = player->GetComponent<Transform>()->GetPosition();
		m_currentCameraForward = Vec3(0, 0, 1);
	}

	void PlayerCamera::OnUpdate()
	{
		m_gameStage = App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetThis<GameStage>();

		auto gameStage = m_gameStage.lock();
		if (!gameStage) return;
			//auto player = gameStage->GetSharedGameObject<Player>(L"Player");
			//if (player)
			//{
			//	auto trans = player->GetComponent<Transform>();
			//	float delta = App::GetApp()->GetElapsedTime();
			//	float fixedDelta = delta * 6;
			//	Vec3 playerPos = trans->GetPosition();

			//	Vec3 eye = playerPos + Vec3(0.0f, 10.0f, -30.0f);
			//	Vec3 at = playerPos + Vec3(0.0f, 0.0f, 0.0f);

			//	if (m_isFirstFrame)
			//	{
			//		m_isFirstFrame = false;
			//		SetEye(Vec3(eye.x,
			//			eye.y,
			//			eye.z
			//		));
			//		////元の座標　＋　元と現在の座標を差を乗算した値　＋　オフセット
			//		SetAt(at);
			//	}
			//	else
			//	{
			//		SetEye(Vec3(eye.x - GetEye().x + (eye.x - GetEye().x) * fixedDelta,
			//			GetEye().y + (eye.y - GetEye().y) * fixedDelta,
			//			GetEye().z + (eye.z - GetEye().z)
			//		));
			//		////元の座標　＋　元と現在の座標を差を乗算した値　＋　オフセット
			//		SetAt(GetAt() + (at - GetAt()) * fixedDelta);
			//	}
			//}

			SetCameraToPlayerPos();
			//ClarifyMovementDirection();

	}

	void PlayerCamera::SetCameraToPlayerPos()
	{
		auto gameStage = m_gameStage.lock();
		if (!gameStage) return;
		auto player = gameStage->GetSharedGameObject<Player>(L"Player");
		if (!player) return;
		auto playerTrans = player->GetComponent<Transform>();
		Vec3 playerPos = playerTrans->GetPosition();
		Vec3 forward = playerTrans->GetForward();
		Vec3 up = Vec3(0, 1, -0.5);

		float distance = 18.0f; // プレイヤーとの距離
		float height = 13.0f;   // カメラの高さ
		float lookAtHeight = 2.0f; // 注視点の高さ調整

		float delta = App::GetApp()->GetElapsedTime();
		float fixedDelta = (std::min)(delta * 4.0f, 1.0f);

		//if (m_isFirstFrame)
		//{
		//	m_currentCameraForward = forward;
		//}
		//else
		//{
		//	m_currentCameraForward = m_currentCameraForward + (forward - m_currentCameraForward) * fixedDelta;
		//	m_currentCameraForward.normalize();
		//}

		Vec3 eye = playerPos - (m_currentCameraForward * distance) + (up * height);
		//Vec3 at = playerPos /*+ (up * 2.0f)*/;
		Vec3 at = Vec3(playerPos.x,playerPos.y,playerPos.z + 8.0f);
		if (m_isFirstFrame)
		{
			SetEye(eye);
			SetAt(at);
			m_isFirstFrame = false;
		}
		else
		{
			//補間
			float speed = (std::min)(delta * 6.0f, 1.0f);
			SetEye(GetEye() + (eye - GetEye()) * speed);
			SetAt(GetAt() + (at - GetAt()) * speed * 2.0f);
		}

		//Vec3 eye = playerPos - (forward * distance) + (up * height);
		////Vec3 eye = playerPos - forward * 20.0f + Vec3(0.0f, 10.0f, -20.0f);

		//Vec3 at = playerPos + Vec3(0.0f, 0.0f, 10.0f);
		//Vec3 at = playerPos + (up * lookAtHeight) * fixedDelta;

		//if (m_isFirstFrame)
		//{
		//	SetEye(eye);
		//	SetAt(at);
		//	m_isFirstFrame = false;
		//}
		//else
		//{
		//	SetEye(Vec3(eye.x - GetEye().x + (eye.x - GetEye().x) * fixedDelta,
		//		GetEye().y + (eye.y - GetEye().y) * fixedDelta,
		//		GetEye().z + (eye.z - GetEye().z)));
		//	SetAt(GetAt() + (at - GetAt()) * fixedDelta);
		//}
	}

	void PlayerCamera::ClarifyMovementDirection()
	{
		auto gameStage = m_gameStage.lock();
		if (!gameStage) return;
		auto player = gameStage->GetSharedGameObject<Player>(L"Player");
		auto goal = gameStage->GetSharedGameObject<Goal>(L"Goal");

		if (!player && goal) return;
		auto playerTrans = player->GetComponent<Transform>();
		auto goalTrans = goal->GetComponent<Transform>();
		Vec3 playerPos = playerTrans->GetPosition();
		Vec3 goalPos = goalTrans->GetPosition();

		Vec3 toGoal = goalPos - playerPos;
		float distanceToGoal = toGoal.length();

		//ゴール方向に少しずらす
		float bias = 0.25f;

		Vec3 targetAt = playerPos + (toGoal * bias);

		//補間
		float delta = App::GetApp()->GetElapsedTime();
		float fixedDelta = (std::min)(delta * 6.0f, 1.0f);

		// 現在の注視点(GetAt)から目標の注視点(targetAt)へ徐々に近づける
		Vec3 currentAt = GetAt();
		Vec3 nextAt = currentAt + (targetAt - currentAt) * fixedDelta;

		SetAt(nextAt);

		//Vec3 diff = goalPos - GetEye();
		//float distance = diff.length();
		//float lookAheadOffset = 5.0f;
		//Vec3 dir = diff.normalize();
		//Vec3 at = playerPos + Vec3(dir);

		//if (distance < lookAheadOffset)
		//{
		//	SetAt(at);
		//}
	}
}