/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"
#include "JoltRigidBody.h"

namespace basecross{
	// プレイヤーの初期設定
	void Player::OnCreate()
	{
		// トランスフォームコンポーネントを取得しておく
		m_transform = GetComponent<Transform>();
		m_transform->SetPosition(m_position); // 初期位置を設定
		m_transform->SetRotation(m_rotation); // 初期回転を設定
		m_transform->SetScale(m_scale); // スケーリングを設定
		// ドローコンポーネントを追加
		//m_draw = AddComponent<PNTDXModelDraw>();
		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"DEFAULT_CUBE");
		m_draw->SetDiffuse(Col4(1,0,0,1));
		//AddComponent<Gravity>();

		// 群れのキャラクターの生成
		for (int i = 0; i < 150; i++)
		{
			auto subPlayer = GetStage()->AddGameObject<SubPlayer>(Vec3(i % 8, 0, i / 10));
			if (i < 50)
			{
				subPlayer->SetAlive(true);
			}
			else
			{
				subPlayer->SetAlive(false);
			}
			m_subPlayers.push_back(subPlayer);
		}

		// ハンマーのオブジェクトの作成
		m_hammer = GetStage()->AddGameObject<HammerFormation>();
		auto hammer = dynamic_pointer_cast<HammerFormation>(m_hammer);
		if (hammer)
		{
			hammer->SetPlayer(GetThis<Player>());
		}
	}

	// プレイヤーの更新処理
	void Player::OnUpdate()
	{

		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();

		// 前回からの経過時間：デルタタイムを取得する
		float delta = app->GetElapsedTime();

		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];

		// 左スティックの入力を取得する
		Vec2 LStick(pad.fThumbLX, pad.fThumbLY);

		// プレイヤーの回転
		if (LStick.length() > 0.1f)
		{
			m_rotation.y = -atan2f(LStick.y, LStick.x);
			m_transform->SetRotation(m_rotation);
		}

		// 左スティックの入力に応じてプレイヤーを移動させる
		float moveSpeed = 2.0f; // 移動速度
		Vec3 moveVec(LStick.x, 0.0f, LStick.y); // 移動ベクトル
		m_position += moveVec * moveSpeed * delta; // 移動ベクトルに速度とデルタタイムを掛ける
		m_transform->SetPosition(m_position); // プレイヤーを移動させる

		// 群れに移動用の座標を送る
		for (auto& obj : m_subPlayers)
		{
			auto subPlayer = dynamic_pointer_cast<SubPlayer>(obj);
			if (subPlayer)
			{
				{
					subPlayer->SetTargetPos(m_position - Vec3(cosf(m_rotation.y), 0, -sinf(m_rotation.y)) * 7.0f);
					subPlayer->SetRotate(m_rotation.y);
				}
			}
		}

		// ボタンで群れの数を変更
		if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
		{
			AddSubPlayer(1);
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_B)
		{
			EraseSubPlayer(1);
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_X)
		{
			auto hammerFormation = dynamic_pointer_cast<HammerFormation>(m_hammer);
			if (hammerFormation)
			{
				hammerFormation->Start(m_position, Vec3(0, m_rotation.y + XM_PIDIV2, 0));
			}
		}

	}

	void Player::AddSubPlayer(int num)
	{
		for (auto& obj : m_subPlayers)
		{
			if (num <= 0)
			{
				break;
			}
			auto subPlayer = dynamic_pointer_cast<SubPlayer>(obj);
			if (!subPlayer->GetAlive())
			{
				subPlayer->SetAlive(true);
				num--;
			}
		}
	}

	void Player::EraseSubPlayer(int num)
	{
		for (auto& obj : m_subPlayers)
		{
			if (num <= 0)
			{
				break;
			}
			auto subPlayer = dynamic_pointer_cast<SubPlayer>(obj);
			if (subPlayer->GetAlive())
			{
				subPlayer->SetAlive(false);
				num--;
			}
		}

	}



	void Player::OnCollisionEnter(const shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Ground"))
		{
		}
	}

	//void Player::OnDraw()
	//{
	//	GameObject::OnDraw();
	//	for (auto& subPlayer : m_subPlayers)
	//	{
	//		subPlayer->OnDraw();
	//	}
	//}

	// 群れのキャラクターの初期化
	void SubPlayer::OnCreate()
	{
		// ドローコンポーネントを追加
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_SPHERE");
		draw->SetDiffuse(Col4(1, 0, 0, 1));

		m_transComp = GetComponent<Transform>();
		m_transComp->SetPosition(m_targetPos);

		m_rad = static_cast<float>(rand() % 6282) / 1000.0f;
		m_len = static_cast<float>(rand() % 10) / 10.0f * 6.0f;
		m_subPos = Vec3(cosf(m_rad) * m_len, 0, sinf(m_rad) * m_len);
	}

	// 群れのキャラクターの更新
	void SubPlayer::OnUpdate()
	{
		auto delta = App::GetApp()->GetElapsedTime();

		auto pos = m_transComp->GetPosition();
		m_subPos = Vec3(cosf(m_rad - m_rotate) * m_len, 0, sinf(m_rad - m_rotate) * m_len);
		Vec3 moveVec = Vec3(m_targetPos + m_subPos - pos);
		moveVec.normalize();
		pos += moveVec * delta * 3.0f;
		pos.y = 1.0f;
		m_transComp->SetPosition(pos);
	}

	void SubPlayer::SetAlive(bool isAlive)
	{
		SetUpdateActive(isAlive);
		SetDrawActive(isAlive);
	}
	bool SubPlayer::GetAlive()
	{
		return GetUpdateActive() && GetDrawActive();
	}


	void HammerFormation::OnCreate()
	{
		// ドローコンポーネントを追加
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDiffuse(Col4(0, 1, 0, 1));

		m_transComp = GetComponent<Transform>();
		Vec3 pos = m_transComp->GetPosition();
		pos.y = 1;
		m_transComp->SetPosition(pos);
		Vec3 scale = m_transComp->GetScale();
		scale.y = 2.0f;
		m_transComp->SetScale(scale);
	}

	void HammerFormation::OnUpdate()
	{
		m_rotation.x += 0.01;
		if (m_rotation.x > XM_PIDIV2)
		{
			m_isActive = false;
			SetDrawActive(m_isActive);
			SetUpdateActive(m_isActive);
			auto player = m_player.lock();
			if(player)
			{
				player->AddSubPlayer(20);
			}
		}
		m_transComp->SetRotation(m_rotation);
	}

	void HammerFormation::Start(const Vec3& position, const Vec3& rotation)
	{
		m_rotation = rotation;
		m_transComp->SetPosition(position);
		m_transComp->SetRotation(m_rotation);
		m_isActive = true;
		SetDrawActive(m_isActive);
		SetUpdateActive(m_isActive);
		auto player = m_player.lock();
		if (player)
		{
			player->EraseSubPlayer(20);
		}

	}

}
//end basecross

