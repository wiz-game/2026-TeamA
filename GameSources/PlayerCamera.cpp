#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	PlayerCamera::PlayerCamera() :
		Camera(),
		m_isFirstFrame(true),
		m_changeAngle(false),
		m_currentCameraForward(0, 0, 1),
		m_prevRStick(0,0),
		m_baseYaw(0.0f),
		m_offsetYaw(0.0f),
		m_cameraAngleState(Center)
	{
	}
	PlayerCamera::~PlayerCamera() {}

	void PlayerCamera::OnCreate()
	{
		//auto gameStage = m_gameStage.lock();
		//if (!gameStage) return;
		//auto player = gameStage->GetSharedGameObject<Player>(L"Player");
		//if (!player) return;
	}

	void PlayerCamera::OnUpdate()
	{
		m_gameStage = App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetThis<GameStage>();

		auto gameStage = m_gameStage.lock();
		if (!gameStage) return;
		auto player = gameStage->GetSharedGameObject<Player>(L"Player");
		if (!player) return;
		auto playerTrans = player->GetComponent<Transform>();
		Vec3 playerPos = playerTrans->GetPosition();

		//bool isOK = playerPos.x >= 10.0f ? true : false; // プレイヤーがこの位置に来たら

		//if(!isOK)
		//{
		//	SetCameraToPlayerPos();
		//}
		//else if(isOK)
		//{
		//	//ChangeAngle();
		//}

		SetCameraToPlayerPos();

		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();

		// 前回からの経過時間：デルタタイムを取得する
		float delta = app->GetElapsedTime();

		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
		// 右スティックの値取得
		Vec2 RStick(pad.fThumbRX, pad.fThumbRY);

		if (!gameStage->IsActive()) return; //ポーズ中は動作をしない

		switch (m_cameraAngleState)
		{
		case Center:
			if (m_prevRStick.x <= 0.25f && RStick.x > 0.25f) // 通常時に右に倒した場合
			{
				//m_changeAngle = true;
				m_offsetYaw = 40.0f; // 右に向く角度を決める
				SetCameraAngleState(Right);
			}
			if (m_prevRStick.x >= -0.25f && RStick.x < -0.25f) //通常時に左に倒した場合
			{
				m_offsetYaw = -40.0f;
				SetCameraAngleState(Left);
			}
			break;
		case Right:
			if (m_prevRStick.x >= -0.25f && RStick.x < -0.25f) // 右を向いているときに左に倒した場合
			{
				//m_changeAngle = false;
				m_offsetYaw = 0.0f;
				SetCameraAngleState(Center);
			}
			break;
		case Left:
			if (m_prevRStick.x <= 0.25f && RStick.x > 0.25f) // 左を向いているときに右に倒した場合
			{
				m_offsetYaw = 0.0f;
				SetCameraAngleState(Center); 
			}
			break;
		}

		if (m_changeAngle)
		{
			Vec3 view = GetAt();
			SetAt(view.x + 0.5f, view.y, view.z);
		}

		//ChangeViewWithStick();
		m_prevRStick = RStick;
	}

	void PlayerCamera::SetCameraToPlayerPos()
	{
		auto gameStage = m_gameStage.lock();
		if (!gameStage) return;
		auto player = gameStage->GetSharedGameObject<Player>(L"Player");
		if (!player) return;
		auto playerTrans = player->GetComponent<Transform>();
		Vec3 playerPos = playerTrans->GetPosition();
		Vec3 up = Vec3(0, 1, -0.5);

		float distance = 18.0f; // プレイヤーとの距離
		float height = 13.0f;   // カメラの高さ
		//float lookAtHeight = 2.0f; // 注視点の高さ調整

		float delta = App::GetApp()->GetElapsedTime();
		float fixedDelta = (std::min)(delta * 4.0f, 1.0f);

		//Vec3 eye = playerPos - (m_currentCameraForward * distance) + (up * height);
		////Vec3 at = playerPos /*+ (up * 2.0f)*/;
		//Vec3 at = Vec3(playerPos.x,playerPos.y,playerPos.z + 8.0f);
		float yaw = m_baseYaw + m_offsetYaw;
		float rad = XMConvertToRadians(yaw);

		Vec3 rotatedForward = Vec3(sin(rad), 0, cos(rad));

		Vec3 eye = playerPos - rotatedForward * distance + up * height;
		Vec3 at = playerPos + rotatedForward * 8.0f;

		if (!m_isFirstFrame)
		{

			SetEye(eye);
			SetAt(at);
		}
		else
		{
			//補間
			float speed = (std::min)(delta * 6.0f, 1.0f);
			SetEye(GetEye() + (eye - GetEye()) * speed);
			SetAt(GetAt() + (at - GetAt()) * speed * 2.0f);
		}
	}

	void PlayerCamera::ChangeAngle()
	{
		auto gameStage = m_gameStage.lock();
		if (!gameStage) return;
		auto player = gameStage->GetSharedGameObject<Player>(L"Player");
		if (!player) return;
		auto playerTrans = player->GetComponent<Transform>();
		Vec3 playerPos = playerTrans->GetPosition();
		Vec3 forward = playerTrans->GetForward();
		Vec3 up = Vec3(0.5f, 1, -0.5); // カメラをずらす方向
		float distance = 15.0f; // カメラのz方向の距離
		float height = 10.0f;   // upの補間
		float delta = App::GetApp()->GetElapsedTime();
		float fixedDelta = (std::min)(delta * 4.0f, 1.0f);

		Vec3 eye = playerPos - (m_currentCameraForward * distance) + (up * height);
		Vec3 at = Vec3(playerPos.x + 5.0f, playerPos.y, playerPos.z);

		if (!m_isFirstFrame)
		{
			SetEye(eye);
			SetAt(at);
		}
		else
		{
			float speed = (std::min)(delta * 5.0f, 1.0f);
			SetEye(GetEye() + (eye - GetEye()) * speed);
			SetAt(GetAt() + (at - GetAt()) * speed * 2.0f);
		}

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