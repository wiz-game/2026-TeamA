/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void PlayerTrackManager::UpdateTrack(const Vec3& playerPos, float width)
	{
		// 軌跡がないとき
		if (m_track.empty())
		{
			m_track.push_back({ playerPos, width });
			return;
		}

		// 最後の軌跡と今の場所を比較し、移動してたら軌跡を追加
		auto lastPos = m_track.back().position;
		auto dis = playerPos - lastPos;
		dis.y = 0;
		if (dis.length() > m_interval)
		{
			m_track.push_back({ playerPos, width });
			// 軌跡のノードの最大数を超えたら最初のノードを破棄
			if (m_track.size() > m_maxNode)
			{
				m_track.pop_front();
			}
		}
	}

	TrackNode PlayerTrackManager::GetNearTrackNode(const Vec3& pos)
	{
		// 軌跡がないとき
		if (m_track.empty())
		{
			return {pos, 10.0f};
		}

		// 最も近い軌跡ノードを探す
		int id = 0;
		float min = FLT_MAX;
		for (int i = 0; i < m_track.size(); i++)
		{
			auto trackPos = m_track.back().position;
			auto dis = pos - trackPos;
			if (dis.length() < min)
			{
				id = i;
			}
		}
		id += 1;
		if (id >= m_track.size())
		{
			id = m_track.size() - 1;
		}
		return m_track[id];
	}

	void FormationManager::Init(const shared_ptr<Stage>& stage, const shared_ptr<GameObject> player)
	{
		m_formation[0] = stage->AddGameObject<HammerFormation>();
		m_formation[1] = stage->AddGameObject<CubeFormation>();
		m_formation[2] = stage->AddGameObject<SpearFormation>();
		m_formation[3] = stage->AddGameObject<BridgeFormation>();

		for (int i = 0; i < 4; i++)
		{
			auto formation = m_formation[i].lock();
			if (formation)
			{
				formation->SetUpdateActive(false);
				formation->SetDrawActive(false);
				formation->SetPlayer(player);
			}
		}

	}

	void FormationManager::StartFormation(int num, const Vec3& pos, const Vec3& rot)
	{
		auto formation = m_formation[num].lock();
		if (formation)
		{
			formation->Start(pos, rot);
		}
	}
	
	void FormationManager::FinishFormation(int num)
	{
		auto formation = m_formation[num].lock();
		if (formation)
		{
			formation->Finish();
		}
	}

	bool FormationManager::GetFormationActive(int num)
	{
		auto formation = m_formation[num].lock();
		if (formation)
		{
			return formation->GetActive();
		}

		return false;
	}


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
		m_draw->SetMeshResource(L"MODEL_PLAYER");
		//m_draw->SetTextureResource(L"TEX_PLAYER");
		//m_draw->SetDiffuse(Col4(1, 0, 0, 1));
		//AddComponent<Gravity>();

		// 群れのキャラクターの生成
		int num[12] = { 10, 12, 12, 14, 14, 14, 14, 14, 12, 12, 12, 10 };
		int n = 0;;
		int sum = 0;
		sum += num[n];
		for (int i = 0; i < MAX_CHARACTER_NUM; i++)
		{
			if (i >= sum)
			{
				n++;
				sum += num[n];
			}
			float x = ((float)((i % num[n]) / 2) + 0.5f) * (float)(i % 2 == 0 ? 1 : -1) * 1.0f;
			float z = n * 1.0f + 1;
			m_characterPositions[i] = Vec3(x, 0, z);
			auto subPlayer = GetStage()->AddGameObject<SubPlayer>(m_characterPositions[i]);
			subPlayer->SetAlive(false);
			subPlayer->SetPlayer(GetThis<GameObject>());
			m_subPlayers.push_back(subPlayer);
		}
		AddSubPlayer(30);

		//m_formation[0] = GetStage()->AddGameObject<HammerFormation>();
		//m_formation[1] = GetStage()->AddGameObject<CubeFormation>();
		//m_formation[2] = GetStage()->AddGameObject<SpearFormation>();
		//m_formation[3] = GetStage()->AddGameObject<BridgeFormation>();

		//for (int i = 0; i < 4; i++)
		//{
		//	auto formation = dynamic_pointer_cast<CharacterFormation>(m_formation[i]);
		//	if (formation)
		//	{
		//		formation->SetUpdateActive(false);
		//		formation->SetDrawActive(false);
		//		formation->SetPlayer(GetThis<Player>());
		//	}
		//}

		m_formationMng.Init(GetStage(), GetThis<Player>());

		//InitializeCharacter();

		auto col = AddComponent<CollisionSphere>();
		//col->SetMakedDiameter(0.5f);
		//col->SetMakedHeight(1.0f);
		col->SetDrawActive(true);
		//AddComponent<Gravity>();

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
			m_rotation.y = -atan2f(LStick.y, LStick.x) - XM_PIDIV2;
			m_transform->SetRotation(m_rotation);
		}

		// 左スティックの入力に応じてプレイヤーを移動させる
		float moveSpeed = 9.0f; // 移動速度
		Vec3 moveVec(LStick.x, m_velocity.y, LStick.y); // 移動ベクトル
		m_velocity.y -= delta;
		m_position = m_transform->GetPosition();
		m_position += moveVec * moveSpeed * delta; // 移動ベクトルに速度とデルタタイムを掛ける
		m_transform->SetPosition(m_position); // プレイヤーを移動させる
		m_desiredVelocity = moveVec * moveSpeed;

		//if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
		//{
		//	// 直接CharacterVirtualの速度を設定
		//	auto velocity = JPH::Vec3(m_desiredVelocity.x, 5.0f, m_desiredVelocity.z);
		//	m_character->SetLinearVelocity(velocity);
		//}

		//if (!m_pPhysicsSystem || !m_character) return;

		//UpdateCharacter(delta);

		//// Transformを更新
		//if (m_transform) {
		//	JPH::RVec3 joltPos = m_character->GetPosition();
		//	m_transform->SetPosition(
		//		static_cast<float>(joltPos.GetX()),
		//		static_cast<float>(joltPos.GetY()),
		//		static_cast<float>(joltPos.GetZ())
		//	);
		//}
		//m_position = m_transform->GetPosition();

		auto aabb = AABB(m_position, 100, 100, 100);
		GetStage()->GetCollisionManager()->SetRootAABB(aabb);

		// 群れに移動用の座標を送る
		for (auto& obj : m_subPlayers)
		{
			auto subPlayer = dynamic_pointer_cast<SubPlayer>(obj);
			if (subPlayer)
			{
				if (subPlayer->GetAlive())
				{
					//subPlayer->SetTargetPos(m_position - Vec3(cosf(m_rotation.y), 0, -sinf(m_rotation.y)) * 7.0f);
					subPlayer->SetPlayerPos(m_position);
					//subPlayer->SetRotate(m_rotation.y);
				}
			}
		}

		// 群れの移動を管理
		if ((m_desiredVelocity.length() > 0.1f || LStick.length()) && m_allMove)
		{
			AllCharacterMove();
		}
		else if (m_desiredVelocity.length() < 0.1f)
		{
			m_allMove = false;
		}

		// ボタンで群れの数を変更
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			AddSubPlayer(1);
		}

		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			EraseSubPlayer(1);
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_B)
		{
			//auto formation = dynamic_pointer_cast<CharacterFormation>(m_formation[m_formationNumber]);
			//if (formation)
			//{
			//	auto rotate = m_rotation;
			//	rotate.y += XM_PIDIV2;
			//	formation->Start(m_position, rotate);
			//}
			if (!m_formationMng.GetFormationActive(m_formationNumber))
			{
				auto rotate = m_rotation;
				rotate.y += XM_PIDIV2;
				m_formationMng.StartFormation(m_formationNumber, m_position, rotate);
			}
			else
			{
				m_formationMng.FinishFormation(m_formationNumber);

			}
		}
		if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			m_formationNumber -= 1;
			if (m_formationNumber < 0)
			{
				m_formationNumber = 0;
			}
		}
		if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			m_formationNumber += 1;
			if (m_formationNumber >= 4)
			{
				m_formationNumber = 3;
			}

		}

		// 軌跡ノードの更新
		auto rotate = m_rotation.y + XM_PIDIV2;
		auto playerBack = Vec3(-cosf(rotate), 0, sinf(rotate)) * 2.5f;
		m_trackMng.UpdateTrack(m_position + playerBack, 10.0f);

		// Debug用文字列
		//app->GetScene<Scene>()->GetDebugString();
		wstringstream ss;
		ss << L"FPS : " << app->GetStepTimer().GetFramesPerSecond() << endl;
		ss << L"Formation : " << m_formationNumber << endl;
		ss << L"ActiveNum : " << m_activeNum << endl;
		app->GetScene<Scene>()->SetDebugString(ss.str());
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
				int x = -rand() % 9 + 5;
				int z = -rand() % 9 + 5;
				Vec3 v = { (float)x, 0, (float)z };
				subPlayer->SetPosition(v + m_position);
				//subPlayer->SetTargetPos(m_characterPositions[m_activeNum]);
				m_activeNum++;
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
				m_activeNum--;
				num--;
			}
		}

		return true;
	}

	vector<shared_ptr<GameObject>> Player::GetActiveSubPlayer()
	{
		vector<shared_ptr<GameObject>> objs;
		for (auto& obj : m_subPlayers)
		{
			auto subPlayer = dynamic_pointer_cast<SubPlayer>(obj);
			if (subPlayer)
			{
				if (subPlayer->GetAlive())
				{
					objs.push_back(obj);
				}
			}
		}

		return objs;
	}

	void Player::OnCollisionEnter(const shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Ground"))
		{
		}
	}

	void Player::OnCollisionExcute(shared_ptr<GameObject>& Other)
	{
		auto otherTrans = Other->GetComponent<Transform>();
		auto otherPos = otherTrans->GetPosition();
		auto otherScale = otherTrans->GetScale();
		auto pos = m_transform->GetPosition();
		auto scale = m_transform->GetScale();
		auto dis = pos - otherPos;
		auto scaleSum = scale + otherScale;
		if ((dis.y / (scaleSum.y / 2.0f)) >= 0.9f || Other->FindTag(L"Cube"))
		{
			m_velocity.y = 0;
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
		characterSettings.mSupportingVolume = JPH::Plane(JPH::Vec3::sAxisY(), -radius);

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

	void Player::AllCharacterMove()
	{
		for (auto& obj : m_subPlayers)
		{
			auto subPlayer = dynamic_pointer_cast<SubPlayer>(obj);
			if (subPlayer->GetAlive())
			{
				subPlayer->SetFollow(true);
			}
		}

	}

	// 群れのキャラクターの初期化
	void SubPlayer::OnCreate()
	{
		// ドローコンポーネントを追加
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"MODEL_PLAYER");
		//draw->SetDiffuse(Col4(1, 0, 0, 1));

		m_transComp = GetComponent<Transform>();
		//m_transComp->SetPosition(m_targetPos);
		m_transComp->SetScale(Vec3(0.7f));

		m_state.reset(new StateMachine<SubPlayer>(GetThis<SubPlayer>()));
		m_state->ChangeState(SubPlayerFollowState::Instance());

		//m_rad = static_cast<float>(rand() % 6282) / 1000.0f;
		//m_len = static_cast<float>(rand() % 10) / 10.0f * 7.0f;

		m_dif = (float)(rand() % 10) * 0.06f + 0.05f;
		m_randam = rand() % 5;
		auto col = AddComponent<CollisionSphere>();
		col->SetDrawActive(true);

	}

	// 群れのキャラクターの更新
	void SubPlayer::OnUpdate()
	{
		m_state->Update();

		auto pos = m_transComp->GetPosition();

		if (pos.y < -100)
		{
			pos.y = m_playerPos.y;
			m_velocity.y = 0;
			m_transComp->SetPosition(pos);
		}
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

	bool SubPlayer::Stay()
	{
		auto delta = App::GetApp()->GetElapsedTime();

		auto pos = m_transComp->GetPosition();
		auto dis = m_playerPos - pos;
		//auto distance = Vec3(m_targetPos).length() + 1.0f;
		if (dis.length() > m_dis * 2.5f)
		{
			m_follow = true;
		}
		if (m_follow)
		{
			m_stay += delta;
			if (m_stay > m_dif)
			{
				m_stay = 0;
				m_follow = false;
				return true;
			}
		}
		return false;
	}
	bool SubPlayer::FollowPlayer()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		auto frame = App::GetApp()->GetStepTimer().GetFrameCount();

		Vec3 playerVec = Vec3(0);
		if (m_player.expired()) return false;
		auto obj = m_player.lock();
		if (!obj) return false;
		auto player = dynamic_pointer_cast<Player>(obj);
		if (!player) return false;
		playerVec = player->GetMoveVelocity();
		

		//auto pos = m_transComp->GetPosition();
		//auto rotate = -m_rotate + XM_PIDIV2;
		//auto subPos = Vec3(m_targetPos.x * cosf(rotate) - m_targetPos.z * sinf(rotate), 0, m_targetPos.x * sinf(rotate) + m_targetPos.z * cosf(rotate)) + m_playerPos;
		//Vec3 moveVec = Vec3(subPos - pos);
		//moveVec.normalize();
		//float speed = playerVec.length() < 0.1f ? 2.0f : playerVec.length();
		//pos += moveVec * delta * speed;
		//pos.y = 1.0f;
		//m_transComp->SetPosition(pos);

		auto pos = m_transComp->GetPosition();
		//auto rotate = m_rotate + XM_PIDIV2;
		//auto subPos = Vec3(cosf(m_rad - rotate) * m_len, 0, sinf(m_rad - rotate) * m_len);
		//auto playerBack = Vec3(-cosf(rotate), 0, sinf(rotate)) * 7.5f;
		//Vec3 moveVec = Vec3(m_playerPos + subPos + playerBack - pos);
		//moveVec.y = 0;
		//moveVec.normalize();
		//m_velocity += moveVec * 0.5f;
		//auto y = m_velocity.y;
		//m_velocity.y = 0;
		//m_velocity.normalize();
		 //重力
		//float y = m_velocity.y - 9.8f * delta;

		if (frame % 5 == m_randam)
		{
			auto trackMng = player->GetTrackManager();
			auto node = trackMng.GetNearTrackNode(pos);
			auto others = player->GetActiveSubPlayer();
			auto steeringForce = CalculateSteering(node, others);
			m_velocity += steeringForce * delta * 5;
			m_velocity.y = 0;
			if (m_velocity.length() > m_maxSpeed)
			{
				m_velocity = m_velocity.normalize() * m_maxSpeed;
			}
		}
		//m_velocity.y = y;
		pos += m_velocity * delta;
		//pos.y = m_playerPos.y;
		m_transComp->SetPosition(pos);

		// 回転処理
		auto rad = atan2f(m_velocity.x, m_velocity.z) + XM_PI;
		m_transComp->SetRotation(Vec3(0, rad, 0));


		auto dis = m_playerPos - pos;
		auto others = player->GetActiveSubPlayer();
		if (playerVec.length() < 0.1f)
		{
			if (dis.length() < 1.0f)
			{
				m_dis = dis.length();
				return true;
			}
			else
			{
				for (auto& other : others)
				{
					auto sub = dynamic_pointer_cast<SubPlayer>(other);
					if (sub)
					{
						if (sub->GetStateMachine()->IsInState(SubPlayerStayState::Instance()))
						{
							auto otherPos = sub->GetComponent<Transform>()->GetPosition();
							auto otherDis = otherPos - pos;
							if (otherDis.length() < 1.3f)
							{
								m_dis = dis.length();
								return true;
							}
						}
					}
				}
			}
		}


		return false;
	}

	void SubPlayer::OnCollisionExcute(shared_ptr<GameObject>& Other)
	{
		auto otherTrans = Other->GetComponent<Transform>();
		auto otherPos = otherTrans->GetPosition();
		auto otherScale = otherTrans->GetScale();
		auto pos = m_transComp->GetPosition();
		auto scale = m_transComp->GetScale();
		auto dis = pos - otherPos;
		auto scaleSum = scale + otherScale;
		if ((dis.y / (scaleSum.y / 2.0f)) >= 0.9f)
		{
			m_velocity.y = 0;
		}
	}

	shared_ptr<GameObject> SubPlayer::GetPlayer()
	{
		if (!m_player.expired())
		{
			auto player = m_player.lock();
			if (player)
			{
				return player;
			}
		}
		return nullptr;
	}

	void SubPlayer::SetPosition(const Vec3& pos)
	{
		m_transComp->SetPosition(pos);
	}

	Vec3 SubPlayer::CalculateSteering(const TrackNode& targetNode, const vector<shared_ptr<GameObject>> subPlayers)
	{
		Vec3 force = Vec3(0);

		auto pos = m_transComp->GetPosition();
		// プレイヤーとの距離をもとに分離の値を変更
		// 群れの数も参照する
		float playerDis = 0;
		if (!m_player.expired())
		{
			auto obj = m_player.lock();
			if (obj)
			{
				auto playerPos = obj->GetComponent<Transform>()->GetPosition();
				playerDis = Vec3(playerPos - pos).length();
			}
		}
		float separateRatio = (7.0f / playerDis);
		if (separateRatio < 0.2f)
		{
			separateRatio = 0.2f;
		}
		else if (separateRatio > 1.f)
		{
			separateRatio = 1.0f;
		}
		int num = subPlayers.size();
		float sepRatio = (float)num / 100.0f;
		if (sepRatio < 0.5f)
		{
			sepRatio = 0.5f;
		}
		else if (sepRatio > 2.0f)
		{
			sepRatio = 2;
		}


		// 道幅をもとに追従、分散の大きさの割合を決める
		// 基準を10.0fとして比率を計算
		float spreadRatio = targetNode.roadWidth / 10.0f;
		if (spreadRatio > 1.0f)
		{
			spreadRatio = 1.0f;
		}
		else if (spreadRatio < 0.1f)
		{
			spreadRatio = 0.1f;
		}

		// Seekは狭いと強くなる
		float seekWeight = (1.0f + (1.0f - spreadRatio) * 2.0f) * 2.0f;
		// Separationは狭いと弱くなる
		float sepWeight = spreadRatio * 1.5f * separateRatio + sepRatio;

		// Seek(軌跡への追従)の処理
		{
			Vec3 disVec = targetNode.position - pos;
			disVec.y = 0;
			float dis = disVec.length();

			// ノードの範囲内にいるときはスピードを緩める
			float speedScale = ((dis * dis) < (targetNode.roadWidth / 2.0f) * (targetNode.roadWidth / 2.0f)) ? 0.6f : 1.0f;

			disVec *= m_maxSpeed * speedScale;
			auto seekForce = disVec - m_velocity;

			// 力の制限
			if (seekForce.length() > 10.0f)
			{
				seekForce = seekForce.normalize() * 10.0f;
			}

			force += seekForce * seekWeight;
		}

		// Separation(衝突回避の反発)の処理
		{
			Vec3 sepForce = Vec3(0);
			int sepCount = 0;

			for (auto& obj : subPlayers)
			{
				auto other = dynamic_pointer_cast<SubPlayer>(obj);
				if (!other) continue;
				if (other == GetThis<SubPlayer>()) continue;

				auto otherPos = other->GetComponent<Transform>()->GetPosition();
				auto disVec = pos - otherPos;
				disVec.y = 0;
				float dis = disVec.length();
				if (dis > 0.001f && dis < 3.0f)
				{
					auto push = disVec.normalize() * (1.0f / dis);
					sepForce += push;
					sepCount++;
				}
			}

			if (sepCount > 0)
			{
				sepForce /= sepCount;
				// 現在の速度を引いてステアリング力に変換
				sepForce = sepForce.normalize() * m_maxSpeed - m_velocity;

				// 力の制限
				if (sepForce.length() > 10.0f)
				{
					sepForce = sepForce.normalize() * 10.0f;
				}

				force += sepForce * sepWeight;
			}
		}

		// Aligment(群れの進行方向を合わせる)処理
		{
			Vec3 aliForce = Vec3(0);
			int count = 0;

			for (auto& obj : subPlayers)
			{
				auto other = dynamic_pointer_cast<SubPlayer>(obj);
				if (!other) continue;
				if (other == GetThis<SubPlayer>()) continue;

				auto otherPos = other->GetComponent<Transform>()->GetPosition();
				auto disVec = pos - otherPos;
				disVec.y = 0;
				float dis = disVec.length();
				if (dis > 0.001f && dis < 5.0f)
				{
					aliForce += other->GetVelocity();
					count++;
				}
			}

			if (count > 0)
			{
				aliForce /= count;
				aliForce = (aliForce.normalize() * m_maxSpeed) - m_velocity;

				force += aliForce;
			}
		}
		return force;
	}


	void CharacterFormation::Finish()
	{
		m_isActive = false;
		SetDrawActive(m_isActive);
		SetUpdateActive(m_isActive);
		auto obj = m_player.lock();
		if (obj)
		{
			auto player = dynamic_pointer_cast<Player>(obj);
			if (player)
			{
				player->AddSubPlayer(m_characterNum);
			}
		}

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

		m_characterNum = 20;
	}

	void HammerFormation::OnUpdate()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		m_rotation.x += delta;
		if (m_rotation.x > XM_PIDIV2)
		{
			//m_isActive = false;
			//SetDrawActive(m_isActive);
			//SetUpdateActive(m_isActive);
			//auto player = m_player.lock();
			//if (player)
			//{
			//	player->AddSubPlayer(20);
			//}
			Finish();

			GetStage()->AddGameObject<AttackCollisionObj>(m_transComp->GetPosition(), m_rotation.y);
		}
		m_transComp->SetRotation(m_rotation);
	}

	void HammerFormation::Start(const Vec3& position, const Vec3& rotation)
	{
		bool b = true;
		auto obj = m_player.lock();
		if (obj)
		{
			auto player = dynamic_pointer_cast<Player>(obj);
			if (player)
			{
				b = player->EraseSubPlayer(m_characterNum);
			}
		}
		if (!b)
		{
			return;
		}
		m_rotation = rotation;
		m_rotation.y += XM_PIDIV2;
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
		m_transComp->SetRotation(Vec3(XM_PIDIV2 / 3, 0, 0));
		m_transComp->SetScale(Vec3(4.0f, 1.0f, 10.0f));
		Vec3 scale = m_transComp->GetScale();

		m_characterNum = 15;

		auto col = AddComponent<CollisionObb>();
		//col->SetMakedSize(0.5f);
		col->SetDrawActive(true);
		col->SetFixed(true);
		AddTag(L"Cube");
	}

	void CubeFormation::OnUpdate()
	{
		//auto delta = App::GetApp()->GetElapsedTime();
		//m_time += delta;
		//if (m_time > 3.0f)
		//{
		//	//m_isActive = false;
		//	//SetDrawActive(m_isActive);
		//	//SetUpdateActive(m_isActive);
		//	////GetComponent<CollisionObb>()->SetUpdateActive(m_isActive);
		//	////RemoveComponent<JoltRigidBody>();
		//	////m_rigidBody.reset();
		//	//auto player = m_player.lock();
		//	//if (player)
		//	//{
		//	//	player->AddSubPlayer(15);
		//	//}
		//	Finish();
		//}
	}

	void CubeFormation::Start(const Vec3& position, const Vec3& rotation)
	{
		if (m_isActive)
		{
			return;
		}
		bool b = true;
		auto obj = m_player.lock();
		if (obj)
		{
			auto player = dynamic_pointer_cast<Player>(obj);
			if (player)
			{
				b = player->EraseSubPlayer(m_characterNum);
			}
		}
		if (!b)
		{
			return;
		}
		m_time = 0.0f;
		m_rotation = rotation;
		auto rot = rotation;
		rot.x = XM_PIDIV2 / 3;
		rot.y += -XM_PIDIV2;
		auto pos = position;
		pos.x += cosf(-m_rotation.y) * 5.0f;
		pos.z += sinf(-m_rotation.y) * 5.0f;
		pos.y += 1.5f;
		m_transComp->SetPosition(pos);
		m_transComp->SetRotation(rot);
		m_isActive = true;
		SetDrawActive(m_isActive);
		SetUpdateActive(m_isActive);
		//GetComponent<CollisionObb>()->SetUpdateActive(m_isActive);

		// 箱形の当たり判定を再設定
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

	void SpearFormation::OnCreate()
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
		scale.z = 2.0f;
		m_transComp->SetScale(scale);

		m_characterNum = 20;
	}

	void SpearFormation::OnUpdate()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		m_position.x += delta * 10;
		m_time += delta;
		if (m_time > 0.2f)
		{
			Finish();
			m_time = 0;
			GetStage()->AddGameObject<AttackCollisionObj>(m_transComp->GetPosition(), m_rotation.y);
		}
		m_transComp->SetPosition(m_position);
	}

	void SpearFormation::Start(const Vec3& position, const Vec3& rotation)
	{
		bool b = true;
		auto obj = m_player.lock();
		if (obj)
		{
			auto player = dynamic_pointer_cast<Player>(obj);
			if (player)
			{
				b = player->EraseSubPlayer(m_characterNum);
			}
		}
		if (!b)
		{
			return;
		}
		m_rotation = rotation;
		m_rotation.y += XM_PIDIV2;
		m_position = position;
		m_transComp->SetPosition(position);
		m_transComp->SetRotation(m_rotation);
		m_isActive = true;
		SetDrawActive(m_isActive);
		SetUpdateActive(m_isActive);
	}

	void BridgeFormation::OnCreate()
	{
		// ドローコンポーネントを追加
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDiffuse(Col4(0, 1, 0, 1));

		m_transComp = GetComponent<Transform>();
		Vec3 pos = m_transComp->GetPosition();
		pos.x = 1;
		m_transComp->SetPosition(pos);
		//m_transComp->SetRotation(Vec3(XM_PIDIV2 / 3, 0, 0));
		m_transComp->SetScale(Vec3(4.0f, 1.0f, 20.0f));
		Vec3 scale = m_transComp->GetScale();

		m_characterNum = 15;

		auto col = AddComponent<CollisionObb>();
		//col->SetMakedSize(0.5f);
		col->SetDrawActive(true);
		col->SetFixed(true);
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

	void BridgeFormation::OnUpdate()
	{
		//auto delta = App::GetApp()->GetElapsedTime();
		//m_time += delta;
		//if (m_time > 3.0f)
		//{
		//	//m_isActive = false;
		//	//SetDrawActive(m_isActive);
		//	//SetUpdateActive(m_isActive);
		//	////GetComponent<CollisionObb>()->SetUpdateActive(m_isActive);
		//	////RemoveComponent<JoltRigidBody>();
		//	////m_rigidBody.reset();
		//	//auto player = m_player.lock();
		//	//if (player)
		//	//{
		//	//	player->AddSubPlayer(15);
		//	//}
		//	Finish();
		//}
	}

	void BridgeFormation::Start(const Vec3& position, const Vec3& rotation)
	{
		if (m_isActive)
		{
			return;
		}
		bool b = true;
		auto obj = m_player.lock();
		if (obj)
		{
			auto player = dynamic_pointer_cast<Player>(obj);
			if (player)
			{
				b = player->EraseSubPlayer(m_characterNum);
			}
		}
		if (!b)
		{
			return;
		}
		m_time = 0.0f;
		m_rotation = rotation;
		auto rot = rotation;
		//rot.x = XM_PIDIV2 / 3;
		rot.y += -XM_PIDIV2;
		auto pos = position;
		pos.x += cosf(-m_rotation.y) * 10.5f;
		pos.z += sinf(-m_rotation.y) * 10.5f;
		pos.y -= 1.0f;
		m_transComp->SetPosition(pos);
		m_transComp->SetRotation(rot);
		m_isActive = true;
		SetDrawActive(m_isActive);
		SetUpdateActive(m_isActive);
		//GetComponent<CollisionObb>()->SetUpdateActive(m_isActive);

		// 箱形の当たり判定を再設定
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

	shared_ptr<SubPlayerStayState> SubPlayerStayState::Instance()
	{
		static shared_ptr<SubPlayerStayState> instance(new SubPlayerStayState);
		return instance;
	}

	void SubPlayerStayState::Enter(const shared_ptr<SubPlayer>& obj)
	{

	}
	void SubPlayerStayState::Execute(const shared_ptr<SubPlayer>& obj)
	{
		if (obj->Stay())
		{
			obj->GetStateMachine()->ChangeState(SubPlayerFollowState::Instance());
		}
	}
	void SubPlayerStayState::Exit(const shared_ptr<SubPlayer>& obj)
	{

	}

	shared_ptr<SubPlayerFollowState> SubPlayerFollowState::Instance()
	{
		static shared_ptr<SubPlayerFollowState> instance(new SubPlayerFollowState);
		return instance;
	}

	void SubPlayerFollowState::Enter(const shared_ptr<SubPlayer>& obj)
	{
		auto gameObj = obj->GetPlayer();
		auto player = dynamic_pointer_cast<Player>(gameObj);
		if (player)
		{
			player->SetAllMove(true);
		}
	}
	void SubPlayerFollowState::Execute(const shared_ptr<SubPlayer>& obj)
	{
		if (obj->FollowPlayer())
		{
			obj->GetStateMachine()->ChangeState(SubPlayerStayState::Instance());
		}
	}
	void SubPlayerFollowState::Exit(const shared_ptr<SubPlayer>& obj)
	{

	}

}
//end basecross

