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
		if (gameStage)
		{
			auto player = gameStage->GetSharedGameObject<Player>(L"Player");
			if (player)
			{
				auto trans = player->GetComponent<Transform>();
				float delta = App::GetApp()->GetElapsedTime();
				float fixedDelta = delta * 6;
				Vec3 playerPos = trans->GetPosition();

				Vec3 eye = playerPos + Vec3(0.0f, 8.0f, -18.0f);
				Vec3 at = playerPos + Vec3(0.0f, 0.0f, 0.0f);

				if (m_isFirstFrame)
				{
					m_isFirstFrame = false;
					SetEye(Vec3(eye.x,
						eye.y,
						eye.z
					));
					//元の座標　＋　元と現在の座標を差を乗算した値　＋　オフセット
					SetAt(at);
				}
				else
				{
					SetEye(Vec3(GetEye().x + (eye.x - GetEye().x) * fixedDelta,
						GetEye().y + (eye.y - GetEye().y) * fixedDelta,
						GetEye().z + (eye.z - GetEye().z)
					));
					//元の座標　＋　元と現在の座標を差を乗算した値　＋　オフセット
					SetAt(GetAt() + (at - GetAt()) * fixedDelta);
				}
			}
		}
	}

	void PlayerCamera::SetCameraToPlayerPos()
	{
		auto gameStage = m_gameStage.lock();
		if (gameStage)
		{
			auto player = gameStage->GetSharedGameObject<Player>(L"Player");
			if (player)
			{
				auto trans = player->GetComponent<Transform>();
				float delta = App::GetApp()->GetElapsedTime();
				float fixedDelta = delta * 6;
				Vec3 playerPos = trans->GetPosition();
				//Vec3 forward = player->GetForward();

				Vec3 eye = playerPos + Vec3(0.0f, 3.0f, -18.0f);
				Vec3 at = playerPos + Vec3(0.0f, 0.0f, 0.0f);

				SetEye(Vec3(eye.x,
					eye.y,
					eye.z));
				//元の座標　＋　元と現在の座標を差を乗算した値　＋　オフセット
				SetAt(at);
			}
		}
	}


}