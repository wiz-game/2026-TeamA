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

		for (int i = 0; i < 50; i++)
		{
			auto subPlayer = GetStage()->AddGameObject<SubPlayer>(Vec3(i % 4, 0, i / 5));
			m_subPlayers.push_back(subPlayer);
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
					subPlayer->SetTargetPos(m_position - Vec3(cosf(m_rotation.y), 0, -sinf(m_rotation.y)) * 5.0f);
					subPlayer->SetRotate(m_rotation.y);
				}
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
		m_len = static_cast<float>(rand() % 10) / 10.0f * 4.0f;
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
}
//end basecross

