/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

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
			hammer->SetUpdateActive(false);
			hammer->SetDrawActive(false);
			hammer->SetPlayer(GetThis<Player>());
		}

		m_cube = GetStage()->AddGameObject<CubeFormation>();
		auto cube = dynamic_pointer_cast<CubeFormation>(m_cube);
		if (cube)
		{
			cube->SetUpdateActive(false);
			cube->SetDrawActive(false);
			cube->SetPlayer(GetThis<Player>());
		}

		InitializeCharacter();

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

		if (LStick.length() > 0.1f)
		{
			m_rotation.y = -atan2f(LStick.y, LStick.x);
			m_transform->SetRotation(m_rotation);
		}

		// 左スティックの入力に応じてプレイヤーを移動させる
		float moveSpeed = 2.0f; // 移動速度
		Vec3 moveVec(LStick.x, 0.0f, LStick.y); // 移動ベクトル
		//m_position += moveVec * moveSpeed * delta; // 移動ベクトルに速度とデルタタイムを掛ける
		//m_transform->SetPosition(m_position); // プレイヤーを移動させる
		m_desiredVelocity = moveVec * moveSpeed;

		if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
		{
			// 直接CharacterVirtualの速度を設定
			auto velocity = JPH::Vec3(m_desiredVelocity.x, 5.0f, m_desiredVelocity.z);
			m_character->SetLinearVelocity(velocity);
		}

		if (!m_pPhysicsSystem || !m_character) return;

		UpdateCharacter(delta);

		// Transformを更新
		if (m_transform) {
			JPH::RVec3 joltPos = m_character->GetPosition();
			m_transform->SetPosition(
				static_cast<float>(joltPos.GetX()),
				static_cast<float>(joltPos.GetY()),
				static_cast<float>(joltPos.GetZ())
			);
		}
		m_position = m_transform->GetPosition();

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

		if (pad.wPressedButtons & XINPUT_GAMEPAD_Y)
		{
			auto cube = dynamic_pointer_cast<CubeFormation>(m_cube);
			if (cube)
			{
				auto pos = m_position;
				pos.x += 1.0f;
				cube->Start(pos, Vec3(0, m_rotation.y + XM_PIDIV2, 0));
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

	bool Player::EraseSubPlayer(int num)
	{
		int rest = 0;
		for (auto& obj : m_subPlayers)
		{
			auto subPlayer = dynamic_pointer_cast<SubPlayer>(obj);
			if (subPlayer->GetAlive())
			{
				rest++;
			}
		}
		if (rest < num)
		{
			return false;
		}
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
		
		return true;
	}



	void Player::OnCollisionEnter(const shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Ground"))
		{
		}
	}

	void Player::InitializeCharacter()
	{
		m_pPhysicsSystem = JoltManager::GetActiveSystem();
		if (!m_pPhysicsSystem) return;

		// Transformから初期位置を取得
		auto transComp = GetComponent<Transform>();
		if (!transComp) return;

		Vec3 pos = transComp->GetPosition();
		Quat q = transComp->GetQuaternion();

		float height = 0.0f;           // キャラクターの高さ
		float radius = 0.5f;           // カプセルの半径
		float mass = 70.0f;            // 質量
		float maxSlopeAngle = 45.0f;   // 登れる最大斜面角度（度）
		float maxStrength = 100.0f;    // 押す力の最大値
		float predictiveContactDistance = 0.1f; // 予測接触距離
		float penetrationRecoverySpeed = 1.0f;  // 貫通回復速度
		m_objectLayer = Layers::MOVING;

		// カプセル形状を作成（半分の高さ - 半径がカプセルの円筒部分）
		float halfHeight = (height * 0.5f) - radius;
		if (halfHeight < 0.0f) halfHeight = 0.0f;

		// 箱形の当たり判定を作成
		JPH::BoxShapeSettings boxShapeSettings(JPH::Vec3(0.5f, 0.5f, 0.5f));
		JPH::ShapeRefC boxShape = boxShapeSettings.Create().Get();

		JPH::RefConst<JPH::Shape> standingShape = JPH::RotatedTranslatedShapeSettings(
			JPH::Vec3(0, height * 0.5f, 0),
			JPH::Quat::sIdentity(),
			boxShape
		).Create().Get();

		// CharacterVirtualの設定
		JPH::CharacterVirtualSettings characterSettings;
		characterSettings.mShape = standingShape;
		characterSettings.mMass = mass;
		characterSettings.mMaxSlopeAngle = JPH::DegreesToRadians(maxSlopeAngle);
		characterSettings.mMaxStrength = maxStrength;
		characterSettings.mPredictiveContactDistance = predictiveContactDistance;
		characterSettings.mPenetrationRecoverySpeed = penetrationRecoverySpeed;
		characterSettings.mUp = JPH::Vec3::sAxisY();
		characterSettings.mSupportingVolume = JPH::Plane(JPH::Vec3::sAxisY(), - radius);

		// CharacterVirtualを作成
		m_character = std::make_unique<JPH::CharacterVirtual>(
			&characterSettings,
			JPH::RVec3(pos.x, pos.y, pos.z),
			JPH::Quat(q.x, q.y, q.z, q.w),
			m_pPhysicsSystem
		);

		// レイヤーの設定
		m_character->SetCharacterVsCharacterCollision(nullptr);

	}

	void Player::UpdateCharacter(float deltaTime)
	{
		if (!m_character) return;

		// 重力を適用
		JPH::Vec3 gravity = m_pPhysicsSystem->GetGravity();
		JPH::Vec3 currentVelocity = m_character->GetLinearVelocity();

		JPH::Vec3 newVelocity;

		// 地面にいる場合
		if (m_character->GetGroundState() == JPH::CharacterVirtual::EGroundState::OnGround || m_position.y < 1.0f) {
			// ジャンプ中（上向きの速度がある）場合はY速度を維持
			if (currentVelocity.GetY() > 0.0f) {
				newVelocity = JPH::Vec3(m_desiredVelocity.x, currentVelocity.GetY(), m_desiredVelocity.z);
			}
			else {
				// 地面に静止している場合はY速度を0に
				newVelocity = JPH::Vec3(m_desiredVelocity.x, 0.0f, m_desiredVelocity.z);
			}
		}
		else {
			// 空中では重力を加算
			newVelocity = currentVelocity + gravity * deltaTime;
			newVelocity.SetX(m_desiredVelocity.x);
			newVelocity.SetZ(m_desiredVelocity.z);
		}
		//newVelocity = JPH::Vec3(m_desiredVelocity.x, m_desiredVelocity.y, m_desiredVelocity.z);

		m_character->SetLinearVelocity(newVelocity);

		// 衝突判定用のフィルター
		JPH::DefaultBroadPhaseLayerFilter broadPhaseFilter(
			m_pPhysicsSystem->GetObjectVsBroadPhaseLayerFilter(),
			m_objectLayer
		);

		JPH::DefaultObjectLayerFilter objectFilter(
			m_pPhysicsSystem->GetObjectLayerPairFilter(),
			m_objectLayer
		);

		JPH::BodyFilter bodyFilter;
		JPH::ShapeFilter shapeFilter;
		JPH::TempAllocatorMalloc tempAllocatorMalloc;

		// 床の速度を反映
		m_character->UpdateGroundVelocity();

		// 更新を実行
		m_character->Update(
			deltaTime,
			gravity,
			broadPhaseFilter,
			objectFilter,
			bodyFilter,
			shapeFilter,
			tempAllocatorMalloc
		);

	}

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
		auto delta = App::GetApp()->GetElapsedTime();
		m_rotation.x += delta;
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

			GetStage()->AddGameObject<AttackCollisionObj>(m_transComp->GetPosition(), m_rotation.y);
		}
		m_transComp->SetRotation(m_rotation);
	}

	void HammerFormation::Start(const Vec3& position, const Vec3& rotation)
	{
		bool b = true;
		auto player = m_player.lock();
		if (player)
		{
			b = player->EraseSubPlayer(20);
		}
		if (!b)
		{
			return;
		}
		m_rotation = rotation;
		m_transComp->SetPosition(position);
		m_transComp->SetRotation(m_rotation);
		m_isActive = true;
		SetDrawActive(m_isActive);
		SetUpdateActive(m_isActive);
	}

	void CubeFormation::OnCreate()
	{
		// ドローコンポーネントを追加
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDiffuse(Col4(0, 1, 0, 1));

		m_transComp = GetComponent<Transform>();
		Vec3 pos = m_transComp->GetPosition();
		pos.x = 1;
		m_transComp->SetPosition(pos);
		Vec3 scale = m_transComp->GetScale();

		// 箱形の当たり判定を作成
		//JPH::BoxShapeSettings boxShapeSettings(JPH::Vec3(0.5f, 0.5f, 0.5f));
		//JPH::ShapeRefC boxShape = boxShapeSettings.Create().Get();

		//JoltRigidBody::Settings rbSettings;
		//rbSettings.shape = boxShape; // 共通の形状を使い回す
		//rbSettings.motionType = JPH::EMotionType::Kinematic; // 物理演算で動く設定
		//rbSettings.objectLayer = Layers::MOVING; // レイヤー 1 (例: MOVING)
		//rbSettings.mass = 10.0f; // 重さ 10kg
		//rbSettings.restitution = 0.5f; // 反発係数 0.5 (弾む)
		//rbSettings.friction = 0.5f;
		//m_rigidBody = AddComponent<JoltRigidBody>();
		//m_rigidBody->Initialize(rbSettings);

	}

	void CubeFormation::OnUpdate()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		m_time += delta;
		if (m_time > 3.0f)
		{
			m_isActive = false;
			SetDrawActive(m_isActive);
			SetUpdateActive(m_isActive);
			RemoveComponent<JoltRigidBody>();
			m_rigidBody.reset();
			auto player = m_player.lock();
			if (player)
			{
				player->AddSubPlayer(15);
			}
		}
	}

	void CubeFormation::Start(const Vec3& position, const Vec3& rotation)
	{
		if (m_isActive)
		{
			return;
		}
		bool b = true;
		auto player = m_player.lock();
		if (player)
		{
			b = player->EraseSubPlayer(15);
		}
		if (!b)
		{
			return;
		}
		m_time = 0.0f;
		m_rotation = rotation;
		m_transComp->SetPosition(position);
		m_transComp->SetRotation(m_rotation);
		m_isActive = true;
		SetDrawActive(m_isActive);
		SetUpdateActive(m_isActive);

		// 箱形の当たり判定を再設定
		JPH::BoxShapeSettings boxShapeSettings(JPH::Vec3(0.5f, 0.5f, 0.5f));
		JPH::ShapeRefC boxShape = boxShapeSettings.Create().Get();

		JoltRigidBody::Settings rbSettings;
		rbSettings.shape = boxShape; // 共通の形状を使い回す
		rbSettings.motionType = JPH::EMotionType::Kinematic; // 物理演算で動く設定
		rbSettings.objectLayer = Layers::MOVING; // レイヤー 1 (例: MOVING)
		rbSettings.mass = 10.0f; // 重さ 10kg
		rbSettings.restitution = 0.5f; // 反発係数 0.5 (弾む)
		rbSettings.friction = 0.5f;
		m_rigidBody = AddComponent<JoltRigidBody>();
		m_rigidBody->Initialize(rbSettings);

	}

	void AttackCollisionObj::OnCreate()
	{
		auto col = AddComponent<CollisionObb>();
		auto transComp = GetComponent<Transform>();
		Vec3 adjust = Vec3(cosf(m_rotation - XM_PIDIV2), 0.0f, -sinf(m_rotation - XM_PIDIV2)) * 2.0f;
		transComp->SetPosition(m_position + adjust);
		transComp->SetRotation(Vec3(0.0f, m_rotation, 0.0f));
		transComp->SetScale(Vec3(2.0f, 1.0f, 2.0f));
		col->SetDrawActive(true);

		AddTag(L"Attack");
	}

	void AttackCollisionObj::OnUpdate()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		m_time += delta;
		if (m_time > 1.0f)
		{
			GetStage()->RemoveGameObject<AttackCollisionObj>(GetThis<AttackCollisionObj>());
		}
	}

}
//end basecross

