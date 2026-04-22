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


	}

	void PlayerCamera::OnUpdate()
	{
		m_gameStage = App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetThis<GameStage>();

		auto gameStage = m_gameStage.lock();
		if (!gameStage) return;
			auto player = gameStage->GetSharedGameObject<Player>(L"Player");
			if (player)
			{
				auto trans = player->GetComponent<Transform>();
				float delta = App::GetApp()->GetElapsedTime();
				float fixedDelta = delta * 6;
				Vec3 playerPos = trans->GetPosition();

				Vec3 eye = playerPos + Vec3(0.0f, 10.0f, -30.0f);
				Vec3 at = playerPos + Vec3(0.0f, 0.0f, 0.0f);

				//if (m_isFirstFrame)
				//{
				//	m_isFirstFrame = false;
				//	SetEye(Vec3(eye.x,
				//		eye.y,
				//		eye.z
				//	));
				//	//元の座標　＋　元と現在の座標を差を乗算した値　＋　オフセット
				//	SetAt(at);
				//}
				//else
				//{
				//	SetEye(Vec3(eye.x - GetEye().x + (eye.x - GetEye().x) * fixedDelta,
				//		GetEye().y + (eye.y - GetEye().y) * fixedDelta,
				//		GetEye().z + (eye.z - GetEye().z)
				//	));
				//	//元の座標　＋　元と現在の座標を差を乗算した値　＋　オフセット
				//	SetAt(GetAt() + (at - GetAt()) * fixedDelta);
				//}
			}

			SetCameraToPlayerPos();

	}

	void PlayerCamera::SetCameraToPlayerPos()
	{
		auto gameStage = m_gameStage.lock();
		if (!gameStage) return;
			auto player = gameStage->GetSharedGameObject<Player>(L"Player");
			if (player)
			{
				auto playerTrans = player->GetComponent<Transform>();
				float delta = App::GetApp()->GetElapsedTime();
				//float fixedDelta = delta * 6;
				Vec3 playerPos = playerTrans->GetPosition();
				//Vec3 forward = player->GetForward();

				Vec3 eye = playerPos + Vec3(-20.0f, 10.0f, 0.0f);
				Vec3 at = playerPos + Vec3(0.0f, 0.0f, 0.0f);

				SetEye(Vec3(eye.x,
					eye.y,
					eye.z));
				//元の座標　＋　元と現在の座標を差を乗算した値　＋　オフセット
				SetAt(at);
			}
	}

	void PlayerCamera::ClarifyMovementDirection()
	{
		auto gameStage = m_gameStage.lock();
		if (!gameStage) return;
		auto player = gameStage->GetSharedGameObject<Player>(L"Player");
		auto goal = gameStage->GetSharedGameObject<Goal>(L"Goal");

		if (player && goal)
		{
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


}