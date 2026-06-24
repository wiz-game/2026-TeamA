/*!
@file Player.cpp
@brief プレイヤーの実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


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
		m_drawComp = AddComponent<PNTBoneModelDraw>();
		m_drawComp->SetMultiMeshResource(L"MODEL_PON");
		m_drawComp->AddAnimation(L"ANIM_IDLE", 0, 120, true);
		m_drawComp->AddAnimation(L"ANIM_WALK", 140, 120, true);
		m_drawComp->ChangeCurrentAnimation(L"ANIM_IDLE");

		//Mat4x4 spanMat;

		//spanMat.affineTransformation(
		//	Vec3(1.0f, 1.0f, 1.0f), // scale
		//	Vec3(0.0f, 0.0f, 0.0f), // rotation
		//	Vec3(0.0f, 0.0f, 0.0f), // rotation
		//	Vec3(0.0f, -0.3f, 0.0f)  // position
		//);

		//m_drawComp->SetMeshToTransformMatrix(spanMat);

		//m_draw->SetTextureResource(L"TEX_PLAYER");
		//m_draw->SetDiffuse(Col4(1, 0, 0, 1));
		//AddComponent<Gravity>();

		// 群れのキャラクターの生成
		m_subPlayerMng = shared_ptr<SubPlayerManager>(new SubPlayerManager());
		m_subPlayerMng->Init(GetStage(), GetThis<GameObject>());
		m_subPlayerMng->Add(20, m_position);

		// 隊列の生成
		m_formationMng = shared_ptr<FormationManager>(new FormationManager());
		m_formationMng->Init(GetStage(), GetThis<Player>());

		// ノード管理クラスの生成
		m_trackMng = shared_ptr<PlayerTrackManager>(new PlayerTrackManager());

		//InitializeCharacter();

		auto col = AddComponent<CollisionSphere>();
		//col->SetMakedDiameter(0.5f);
		//col->SetMakedHeight(1.0f);
		//col->SetDrawActive(true);
		//AddComponent<Gravity>();

		// 当たり判定用のタグの追加
		AddTag(L"Player");

		// デバッグ用文字列のレイヤーを上げる
		//GetStage()->GetSharedGameObject<GameObject>(L"DebugString")->SetDrawLayer(5);
	}

	// プレイヤーの更新処理
	void Player::OnUpdate()
	{
		// デバッグ用文字列のレイヤーを上げる
		GetStage()->GetSharedGameObject<GameObject>(L"DebugString")->SetDrawLayer(5);

		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();

		// 前回からの経過時間：デルタタイムを取得する
		float delta = app->GetElapsedTime();

		// 入力デバイスを取得する
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];

		// 左スティックの入力を取得する
		Vec2 LStick(pad.fThumbLX, pad.fThumbLY);

		// カメラを取得
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto at = camera->GetAt();
		auto eye = camera->GetEye();
		auto dir = eye - at;
		dir.normalize();
		auto rad = atan2f(dir.z, dir.x) + XM_PIDIV2;

		// 左スティックの入力に応じてプレイヤーを移動させる
		float moveSpeed = 9.0f; // 移動速度
		Vec3 moveVec(LStick.x, 0.0f, LStick.y); // 移動ベクトル
		auto x = moveVec.x * cosf(rad) - moveVec.z * sinf(rad);
		auto z = moveVec.x * sinf(rad) + moveVec.z * cosf(rad);
		moveVec.x = x;
		moveVec.z = z;
		m_velocityY -= delta * 9.8f;
		//m_velocity *= 0.95f;
		m_velocity = moveVec * moveSpeed;
		if (m_isStartedFormation)
		{
			m_velocity = { 0 };
		}
		if (m_velocity.length() > moveSpeed)
		{
			m_velocity = m_velocity.normalize() * moveSpeed;
		}

		// トランポリンによるバウンド移動処理
		if (m_accelerationForTrampolineBound > 0.0f)
		{
			m_velocityY += m_accelerationForTrampolineBound * delta;

			// バウンド加速度を重力分減退させる
			m_accelerationForTrampolineBound -= 9.8f * delta;
			if (m_accelerationForTrampolineBound < 0.0f) m_accelerationForTrampolineBound = 0.0f;
		}

		// 移動量確定
		m_position = m_transform->GetPosition();
		//m_position += moveVec * moveSpeed * delta; // 移動ベクトルに速度とデルタタイムを掛ける
		m_position += m_velocity * delta;
		m_position.y += m_velocityY * delta;
		m_transform->SetPosition(m_position); // プレイヤーを移動させる
		m_desiredVelocity = moveVec * moveSpeed;

		// 回転処理
		if (m_velocity.length() > 0.1f)
		{
			m_rotation.y = -atan2f(m_velocity.z, m_velocity.x) + XM_PI;
			m_transform->SetRotation(m_rotation);
		}

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
		m_subPlayerMng->SetPlayerPos(m_position);

		// 群れの移動を管理
		if ((LStick.length() > 0.1f) && m_subPlayerMng->GetAllMove())
		{
			m_subPlayerMng->AllCharacterMove();
		}
		else if (m_desiredVelocity.length() < 0.1f)
		{
			//m_allMove = false;
			m_subPlayerMng->SetAllMove(false);
		}

		// ボタンで群れの数を変更
		if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			//AddSubPlayer(1);
			m_subPlayerMng->Add(5, m_position);
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			//EraseSubPlayer(1);
			m_subPlayerMng->Erase(5);
		}
		// 隊列に関する処理
		if (m_subPlayerMng->CheckFormationReady())
		{
			auto rotate = m_formationRot;
			rotate.y += XM_PI;
			auto pos = m_subPlayerMng->GetTargetPos();
			m_formationMng->StartFormation(pos, rotate);
			m_isStartedFormation = false;

		}

		// 一定時間経過しても隊列が組まれなかった時の処理
		if (m_isStartedFormation)
		{
			m_frmWaitTime += delta;
			if (m_frmWaitTime >= 5.0f)
			{
				m_formationMng->ResetDraw();
				m_subPlayerMng->ResetFormationMenber();
				m_isStartedFormation = false;
			}
		}
		else
		{
			m_frmWaitTime = 0.0f;
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_B && !m_formationMng->GetFormationActive())
		{
			m_formationMng->ResetDraw();
			m_subPlayerMng->ResetFormationMenber();
			m_isStartedFormation = false;
			m_frmWaitTime = 0.0f;
		}
		if (pad.wReleasedButtons & XINPUT_GAMEPAD_B)
		{
			if (!m_formationMng->GetFormationActive() && !m_isStartedFormation)
			{
				m_formationMng->ResetDraw();
			}

			if (!m_formationMng->GetFormationActive())
			{
				auto rotate = m_rotation;
				rotate.y += XM_PIDIV2;
				//m_formationMng->StartFormation(m_position, rotate);
				int num = m_formationMng->GetFormationCharacterNum();
				Vec3 pos = m_position;
				Vec3 rot = m_rotation;
				rot.y += XM_PI;
				pos.x += cosf(-rot.y) * 5.5f;
				pos.z += sinf(-rot.y) * 5.5f;
				m_formationRot = m_rotation;
				bool b = m_subPlayerMng->StartForamtionMove(num, pos);
				if (b)
				{
					m_formationMng->DrawFormationRange(pos, rot);
					m_isStartedFormation = true;
				}
			}
			else
			{
				m_formationMng->FinishFormation();
			}
		}


		// 隊列の範囲を表示
		if (pad.wPressedButtons & XINPUT_GAMEPAD_X)
		{
			if (!m_formationMng->GetFormationActive() && !m_isStartedFormation)
			{
				//int num = m_formationMng->GetFormationCharacterNum();
				//int followNum = m_subPlayerMng->GetFollowNum();
				//if (followNum >= num)
				//{
				//	Vec3 pos = m_position;
				//	Vec3 rot = m_rotation;
				//	rot.y += XM_PI;
				//	pos.x += cosf(-rot.y) * 5.5f;
				//	pos.z += sinf(-rot.y) * 5.5f;

				//	m_formationMng->DrawFormationRange(pos, rot);
				//}
				m_drawFormationRange = !m_drawFormationRange;
			}

			if (!m_drawFormationRange && !m_formationMng->GetFormationActive() && !m_isStartedFormation)
			{
				m_formationMng->ResetDraw();
			}
		}
		//if (pad.wReleasedButtons & XINPUT_GAMEPAD_X && !m_formationMng->GetFormationActive() && !m_isStartedFormation)
		//{
		//	m_formationMng->ResetDraw();
		//}

		if (m_drawFormationRange && !m_formationMng->GetFormationActive() && !m_isStartedFormation)
		{
			int num = m_formationMng->GetFormationCharacterNum();
			int followNum = m_subPlayerMng->GetFollowNum();
			if (followNum >= num)
			{
				Vec3 pos = m_position;
				Vec3 rot = m_rotation;
				rot.y += XM_PI;
				pos.x += cosf(-rot.y) * 5.5f;
				pos.z += sinf(-rot.y) * 5.5f;

				m_formationMng->DrawFormationRange(pos, rot);
			}
			else
			{
				m_formationMng->ResetDraw();
			}
		}


		if (pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			if (!m_formationMng->GetFormationActive())
			{
				m_formationMng->ResetDraw();
			}
			m_formationMng->SetFormationNumber(m_formationMng->GetFormationNumber() - 1);
		}
		if (pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			if (!m_formationMng->GetFormationActive())
			{
				m_formationMng->ResetDraw();
			}
			m_formationMng->SetFormationNumber(m_formationMng->GetFormationNumber() + 1);

		}

		// ゲームオーバーの処理
		if (m_position.y < -50.0f)
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), app->GetScene<Scene>(), L"ToGameOverStage");
		}

		// 軌跡ノードの更新
		//auto rotate = m_rotation.y + XM_PIDIV2;
		//auto playerBack = Vec3(-cosf(rotate), 0, sinf(rotate)) * 2.5f;
		//m_trackMng->UpdateTrack(m_position + playerBack, m_roadWidth);
		m_trackMng->UpdateTrack(m_position, m_roadWidth);

		// アニメーションの更新
		m_drawComp->UpdateAnimation(delta * 2.0f);
		if (abs(m_velocity.x + m_velocity.z) < 0.1f)
		{
			if (m_drawComp->GetCurrentAnimation() != L"ANIM_IDLE")
			{
				m_drawComp->ChangeCurrentAnimation(L"ANIM_IDLE");
			}
		}
		else
		{
			if (m_drawComp->GetCurrentAnimation() != L"ANIM_WALK")
			{
				m_drawComp->ChangeCurrentAnimation(L"ANIM_WALK");
			}
		}

		// Debug用文字列
		//app->GetScene<Scene>()->GetDebugString();
		wstringstream ss;
		ss << L"FPS : " << app->GetStepTimer().GetFramesPerSecond() << endl;
		ss << L"Formation : " << m_formationMng->GetFormationNumber() << endl;
		ss << L"ActiveNum : " << m_subPlayerMng->GetActiveNum() << endl;
		ss << L"RoadWidth : " << m_roadWidth << endl;
		ss << L"CollisionObject : " << m_debugStr << endl;
		//for (auto& obj : m_subPlayerMng->GetActiveSubPlayer())
		//{
		//	auto sub = dynamic_pointer_cast<SubPlayer>(obj);
		//	if (sub)
		//	{
		//		ss << sub->GetComponent<PNTBoneModelDraw>()->GetCurrentAnimation() << " ";
		//	}
		//}
		//ss << endl;
		app->GetScene<Scene>()->SetDebugString(ss.str());
	}


	void Player::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Ground"))
		{
		}

		if (other->FindTag(L"Bridge"))
		{
			auto bridge = dynamic_pointer_cast<BridgeFormation>(other);
			if (bridge)
			{
				bridge->SetOnPlayer(true);
			}
		}
		if (other->FindTag(L"Cube"))
		{
			auto cube = dynamic_pointer_cast<CubeFormation>(other);
			if (cube)
			{
				cube->SetOnPlayer(true);
			}
		}
		m_debugStr = L"Check";

		// Trampolineに乗ったとき跳ねるため、上向きの加速度が設定される
		// ゲームの違和感を軽減するため、上向きの初速も加える
		auto& ptrTrampoline = dynamic_pointer_cast<Trampoline>(other);
		if (ptrTrampoline)
		{
			auto trampolineTrans = ptrTrampoline->GetComponent<Transform>();
			auto trampolinePos = trampolineTrans->GetPosition();
			auto trampolineScale = trampolineTrans->GetScale();

			// 衝突時トランポリンより高い位置にいるときだけ跳ねる
			if (m_position.y - m_scale.y * 0.5f > trampolinePos.y + trampolineScale.y * 0.5f)
			{
				m_accelerationForTrampolineBound = ptrTrampoline->GetBoundPower();
				m_velocityY = m_velocityY < 0.0f ? 0.0f : m_velocityY;
				m_velocityY = m_accelerationForTrampolineBound * 0.5f;
			}
		}
	}

	void Player::OnCollisionExcute(shared_ptr<GameObject>& Other)
	{
		auto otherTrans = Other->GetComponent<Transform>();
		auto otherPos = otherTrans->GetPosition();
		auto otherScale = otherTrans->GetScale();
		auto otherRot = otherTrans->GetQuaternion().toRotVec();
		auto pos = m_transform->GetPosition();
		auto scale = m_transform->GetScale();
		auto dis = pos - otherPos;
		auto scaleSum = scale + otherScale;
		bool isRotX = otherRot.x != 0.0f;
		bool isRotY = otherRot.y != 0.0f;
		bool isRotZ = otherRot.x != 0.0f;
		if (isRotX /*|| isRotY*/ || isRotZ)
		{
			m_velocityY = 0;
			//Vec3 cPos = dis;
			//if (isRotX)
			//{
			//	auto y = cPos.y * cosf(-otherRot.x) - cPos.z * sinf(-otherRot.x);
			//	auto z = cPos.y * sinf(-otherRot.x) + cPos.z * cosf(-otherRot.x);
			//	cPos.y = y;
			//	cPos.z = z;
			//}
			//if (isRotY)
			//{
			//	auto x =  cPos.x * cosf(-otherRot.y) + cPos.z * sinf(-otherRot.y);
			//	auto z = -cPos.x * sinf(-otherRot.y) + cPos.z * cosf(-otherRot.y);
			//	cPos.x = x;
			//	cPos.z = z;
			//}
			//if (isRotZ)
			//{
			//	auto x = cPos.x * cosf(-otherRot.z) - cPos.y * sinf(-otherRot.z);
			//	auto y = cPos.x * sinf(-otherRot.z) + cPos.y * cosf(-otherRot.z);
			//	cPos.x = x;
			//	cPos.y = y;
			//}
			//dis.y = cPos.y;
		}

		if ((scaleSum.y / 2.0f) - dis.y <= scale.y)
		{
			m_velocityY = 0;
			pos.y += 0.01f;
			//m_position = pos;
			m_transform->SetPosition(pos);
		}

		auto foothold = dynamic_pointer_cast<Foothold>(Other);
		auto board = dynamic_pointer_cast<Board>(Other);
		if (Other->FindTag(L"Cube"))
		{
			m_roadWidth = 8.0f;
			m_velocityY = 0;
		}
		else if (Other->FindTag(L"Bridge"))
		{
			m_roadWidth = 7.0f;
		}
		else if (foothold)
		{
			m_roadWidth = 8.0f;
		}
		else if (board)
		{
			m_roadWidth = 5.0f;
		}
		else
		{
			m_roadWidth = 10.0f;
		}

		auto obj = dynamic_pointer_cast<StageObject>(Other);
		if (obj)
		{
			// Debug用文字列
			m_debugStr = L"Object";
			auto block = dynamic_pointer_cast<Block>(obj);
			if (block) m_debugStr = L"Block";
			auto slope = dynamic_pointer_cast<Slope>(obj);
			if (slope) m_debugStr = L"Slope";
			auto slopeCol = dynamic_pointer_cast<SlopeCollisionObject>(obj);
			if (slopeCol) m_debugStr = L"SlopeCollision";
			auto foothold = dynamic_pointer_cast<Foothold>(obj);
			if (foothold) m_debugStr = L"Foothold";
			auto board = dynamic_pointer_cast<Board>(obj);
			if (board) m_debugStr = L"Board";
			auto firtree = dynamic_pointer_cast<FirTree>(obj);
			if (firtree) m_debugStr = L"FirTree";
			auto fallentree = dynamic_pointer_cast<FallenTree>(obj);
			if (fallentree) m_debugStr = L"FallenTree";
			auto stone = dynamic_pointer_cast<Stone>(obj);
			if (stone) m_debugStr = L"Stone";
			auto mushroom = dynamic_pointer_cast<Mushroom>(obj);
			if (mushroom) m_debugStr = L"Mushroom";
		}
		else
		{
			m_debugStr = L"NULL";
		}
	}

	void Player::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
		//m_roadWidth = 10.0f;
		if (Other->FindTag(L"Bridge"))
		{
			auto bridge = dynamic_pointer_cast<BridgeFormation>(Other);
			if (bridge)
			{
				bridge->SetOnPlayer(false);
			}
		}
		if (Other->FindTag(L"Cube"))
		{
			auto cube = dynamic_pointer_cast<CubeFormation>(Other);
			if (cube)
			{
				cube->SetOnPlayer(false);
			}
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



}
//end basecross

