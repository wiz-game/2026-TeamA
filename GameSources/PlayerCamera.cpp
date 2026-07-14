#include "stdafx.h"
#include "Project.h"
#include "GameStage.h"
#include "TurningPoint.h"


namespace basecross
{
	PlayerCamera::PlayerCamera() :
		Camera(),
		m_isFirstFrame(true),
		m_changeAngle(true),
		m_currentCameraForward(0, 0, 1),
		m_prevRStick(0, 0),
		m_baseYaw(0.0f),
		m_offsetYaw(0.0f),
		m_cameraAngleState(Center),
		On(true)
	{}
	PlayerCamera::~PlayerCamera() {}

	void PlayerCamera::OnCreate()
	{
		//auto gameStage = m_gameStage.lock();
		//if (!gameStage) return;
		//auto player = gameStage->GetSharedGameObject<Player>(L"Player");
		//if (!player) return;
		auto gameStage = m_gameStage.lock();
		if (!gameStage) return;
		for (int i = 0; i < gameStage->count; i++)
		{
			auto tp = gameStage->GetSharedGameObject<TurningPoint>(L"TurningPoint_" + std::to_wstring(i));
			if (!tp)return;
			m_tp.push_back(tp);
		}

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

		if (!gameStage->GetIsActive()) return; //ポーズ中は動作をしない

		if (m_tp.empty()) //空の場合に追加する
		{
			for (int i = 0; i < gameStage->count; i++)
			{
				auto tp = gameStage->GetSharedGameObject<TurningPoint>(L"TurningPoint_" + std::to_wstring(i));
				if (tp) m_tp.push_back(tp);
			}
		}


		for (int i = 0; i < m_tp.size(); i++)
		{
			if (m_tp[i]->GetTrigger()) //TurningPointの場所にきたら角度を変える動作を行う
			{
				//SetNextCameraAngle(newEye, newAt);
				tpCount = i;
				m_changeAngle = true;
				On = false;
			}
		}

		if (!On)
		{
			switch (tpCount)
			{
			case 0:
				m_nextEye = Vec3(playerPos.x + 60.0f, playerPos.y + 40.0f, playerPos.z - 30.0f);
				m_nextAt = Vec3(playerPos.x, playerPos.y, playerPos.z + 40);
				break;
			case 1:
				m_nextEye = Vec3(playerPos.x + 20.0f, playerPos.y + 40.0f, playerPos.z - 30.0f);
				m_nextAt = Vec3(playerPos.x + 20.0f, playerPos.y, playerPos.z + 20.0f);
				break;
			case 2:
				m_nextEye = Vec3(playerPos.x + 20.0f, playerPos.y + 40.0f, playerPos.z - 30.0f);
				m_nextAt = Vec3(playerPos.x + 20.0f, playerPos.y, playerPos.z + 20.0f);
				break;

			}
		}


		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();

		// 前回からの経過時間：デルタタイムを取得する
		float delta = app->GetElapsedTime();

		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
		// 右スティックの値取得
		Vec2 RStick(pad.fThumbRX, pad.fThumbRY);


		switch (m_cameraAngleState)
		{
		case Center:
			if (m_prevRStick.x <= 0.25f && RStick.x > 0.25f) // 通常時に右に倒した場合
			{
				m_offsetYaw = 45.0f; // 右に向く角度を決める
				SetCameraAngleState(Right);
			}
			if (m_prevRStick.x >= -0.25f && RStick.x < -0.25f) //通常時に左に倒した場合
			{
				m_offsetYaw = -45.0f;
				SetCameraAngleState(Left);
			}
			break;
		case Right:
			if (m_prevRStick.x >= -0.25f && RStick.x < -0.25f) // 右を向いているときに左に倒した場合
			{
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
		m_prevRStick = RStick;


		if (On)
			SetCameraToPlayerPos(); //通常時は普通のプレイヤー追従カメラ
		else
			ChangeAngle();          //切り替えオブジェクトに触れた場合はアングルを変える

	}

	void PlayerCamera::SetCameraToPlayerPos()
	{
		auto gameStage = m_gameStage.lock();
		if (!gameStage) return;
		auto player = gameStage->GetSharedGameObject<Player>(L"Player");
		if (!player) return;
		auto playerTrans = player->GetComponent<Transform>();
		Vec3 playerPos = playerTrans->GetPosition();
		Vec3 up = Vec3(0, 2, -0.5);

		float distance = 40.0f; // プレイヤーとの距離
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

		if (m_isFirstFrame)
		{

			SetEye(eye);
			SetAt(at);
			m_isFirstFrame = false;
		}
		else
		{
			//補間
			float speed = (std::min)(delta * 6.0f, 1.0f); //目標を通り過ぎないために1.0fを超えないよう調整
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
		//float yaw = m_baseYaw + m_offsetYaw;
		//float rad = XMConvertToRadians(yaw);

		//Vec3 rotatedForward = Vec3(sin(rad), 0, cos(rad));

		if (m_changeAngle)
		{
			SetEye(m_nextEye);
			SetAt(m_nextAt);
			m_changeAngle = false;
		}
		else
		{
			//補間
			float speed = (std::min)(delta * 6.0f, 1.0f); //目標を通り過ぎないために1.0fを超えないよう調整
			SetEye(GetEye() + (m_nextEye - GetEye()) * speed);
			SetAt(GetAt() + (m_nextAt - GetAt()) * speed * 2.0f);
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