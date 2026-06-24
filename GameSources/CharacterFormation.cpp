/*!
@file CharacterFormation.cpp
@brief 群れの隊列の実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

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
				player->GetSubPlayerManager()->Add(m_characterNum, m_transComp->GetPosition() + Vec3(0.0f, 1.0f, 0.0f));
			}
		}

	}

	void HammerFormation::OnCreate()
	{
		// ドローコンポーネントを追加
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
		m_drawComp->SetDiffuse(Col4(0, 1, 0, 1));

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
				b = player->GetSubPlayerManager()->Erase();
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
		auto col = m_drawComp->GetDiffuse();
		col.w = 1.0f;
		m_drawComp->SetDiffuse(col);
		SetAlphaActive(false);

	}

	void HammerFormation::EffectRangeDraw(const Vec3& position, const Vec3& rotation)
	{
		m_rotation = rotation;
		m_rotation.y += XM_PIDIV2;
		m_transComp->SetPosition(position);
		m_transComp->SetRotation(m_rotation);

		SetDrawActive(true);
		auto col = m_drawComp->GetDiffuse();
		col.w = 0.5f;
		m_drawComp->SetDiffuse(col);
		SetAlphaActive(true);
	}

	void CubeFormation::OnCreate()
	{
		// ドローコンポーネントを追加
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
		m_drawComp->SetDiffuse(Col4(0, 1, 0, 1));

		m_transComp = GetComponent<Transform>();
		Vec3 pos = m_transComp->GetPosition();
		pos.x = 1;
		m_transComp->SetPosition(pos);
		m_transComp->SetRotation(Vec3(XM_PIDIV2 / 3, 0, 0));
		m_transComp->SetScale(Vec3(10.0f, 1.0f, 20.0f));
		Vec3 scale = m_transComp->GetScale();

		m_characterNum = 15;

		auto col = AddComponent<CollisionObb>();
		//col->SetMakedSize(0.5f);
		//col->SetDrawActive(true);
		col->SetFixed(true);
		AddTag(L"Cube");
	}

	void CubeFormation::OnUpdate()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		if (!m_onPlayer)
		{
			m_time += delta;
		}
		else
		{
			m_time = 0;
		}
		if (m_time > 3.0f)
		{
			//m_isActive = false;
			//SetDrawActive(m_isActive);
			//SetUpdateActive(m_isActive);
			////GetComponent<CollisionObb>()->SetUpdateActive(m_isActive);
			////RemoveComponent<JoltRigidBody>();
			////m_rigidBody.reset();
			//auto player = m_player.lock();
			//if (player)
			//{
			//	player->AddSubPlayer(15);
			//}
			Finish();
		}
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
				b = player->GetSubPlayerManager()->Erase();
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
		pos.x += cosf(-m_rotation.y) * 8.0f;
		pos.z += sinf(-m_rotation.y) * 8.0f;
		pos.y += 4.0f;
		m_transComp->SetPosition(pos);
		m_transComp->SetRotation(rot);
		m_isActive = true;
		SetDrawActive(m_isActive);
		SetUpdateActive(m_isActive);

		auto col = m_drawComp->GetDiffuse();
		col.w = 1.0f;
		m_drawComp->SetDiffuse(col);
		SetAlphaActive(false);

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

	void CubeFormation::EffectRangeDraw(const Vec3& position, const Vec3& rotation)
	{
		if (m_isActive)
		{
			return;
		}
		m_rotation = rotation;
		auto rot = rotation;
		rot.x = XM_PIDIV2 / 3;
		rot.y += -XM_PIDIV2;
		auto pos = position;
		pos.x += cosf(-m_rotation.y) * 8.0f;
		pos.z += sinf(-m_rotation.y) * 8.0f;
		pos.y += 4.0f;
		m_transComp->SetPosition(pos);
		m_transComp->SetRotation(rot);

		SetDrawActive(true);
		auto col = m_drawComp->GetDiffuse();
		col.w = 0.5f;
		m_drawComp->SetDiffuse(col);
		SetAlphaActive(true);
	}


	void SpearFormation::OnCreate()
	{
		// ドローコンポーネントを追加
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
		m_drawComp->SetDiffuse(Col4(0, 1, 0, 1));

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
				b = player->GetSubPlayerManager()->Erase();
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

		auto col = m_drawComp->GetDiffuse();
		col.w = 1.0f;
		m_drawComp->SetDiffuse(col);
		SetAlphaActive(false);

	}

	void SpearFormation::EffectRangeDraw(const Vec3& position, const Vec3& rotation)
	{
		m_rotation = rotation;
		m_rotation.y += XM_PIDIV2;
		m_position = position;
		m_transComp->SetPosition(position);
		m_transComp->SetRotation(m_rotation);

		SetDrawActive(true);
		auto col = m_drawComp->GetDiffuse();
		col.w = 0.5f;
		m_drawComp->SetDiffuse(col);
		SetAlphaActive(true);
	}


	void BridgeFormation::OnCreate()
	{
		// ドローコンポーネントを追加
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
		m_drawComp->SetDiffuse(Col4(0, 1, 0, 1));

		m_transComp = GetComponent<Transform>();
		Vec3 pos = m_transComp->GetPosition();
		pos.x = 1;
		m_transComp->SetPosition(pos);
		//m_transComp->SetRotation(Vec3(XM_PIDIV2 / 3, 0, 0));
		m_transComp->SetScale(m_baseScale);
		Vec3 scale = m_transComp->GetScale();

		m_characterNum = 15;

		auto col = AddComponent<CollisionObb>();
		//col->SetMakedSize(0.5f);
		//col->SetDrawActive(true);
		col->SetFixed(true);
		AddTag(L"Bridge");
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
		auto delta = App::GetApp()->GetElapsedTime();
		if (!m_onPlayer)
		{
			m_time += delta;
		}
		else
		{
			m_time = 0;
		}
		if (m_time > 3.0f)
		{
			//m_isActive = false;
			//SetDrawActive(m_isActive);
			//SetUpdateActive(m_isActive);
			////GetComponent<CollisionObb>()->SetUpdateActive(m_isActive);
			////RemoveComponent<JoltRigidBody>();
			////m_rigidBody.reset();
			//auto player = m_player.lock();
			//if (player)
			//{
			//	player->AddSubPlayer(15);
			//}
			Finish();
		}
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
				b = player->GetSubPlayerManager()->Erase();
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
		pos.x += cosf(-m_rotation.y) * 20.0f * ((float)m_characterNum / 15.0f);
		pos.z += sinf(-m_rotation.y) * 20.0f * ((float)m_characterNum / 15.0f);
		pos.y -= 0.99f;
		auto scale = m_baseScale;
		scale.z *= (float)m_characterNum / 15.0f;
		m_transComp->SetScale(scale);
		m_transComp->SetPosition(pos);
		m_transComp->SetRotation(rot);
		m_isActive = true;
		SetDrawActive(m_isActive);
		SetUpdateActive(m_isActive);

		auto col = m_drawComp->GetDiffuse();
		col.w = 1.0f;
		m_drawComp->SetDiffuse(col);
		SetAlphaActive(false);

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

	void BridgeFormation::EffectRangeDraw(const Vec3& position, const Vec3& rotation)
	{
		m_rotation = rotation;
		auto rot = rotation;
		//rot.x = XM_PIDIV2 / 3;
		rot.y += -XM_PIDIV2;
		auto pos = position;
		pos.x += cosf(-m_rotation.y) * 20.0f * ((float)m_characterNum / 15.0f);
		pos.z += sinf(-m_rotation.y) * 20.0f * ((float)m_characterNum / 15.0f);
		pos.y -= 0.99f;
		auto scale = m_baseScale;
		scale.z *= (float)m_characterNum / 15.0f;
		m_transComp->SetScale(scale);
		m_transComp->SetPosition(pos);
		m_transComp->SetRotation(rot);

		SetDrawActive(true);
		auto col = m_drawComp->GetDiffuse();
		col.w = 0.5f;
		m_drawComp->SetDiffuse(col);
		SetAlphaActive(true);

	}

	int BridgeFormation::GetCharacterNum()
	{
		auto obj = m_player.lock();
		if (!obj) return -1;

		auto player = dynamic_pointer_cast<Player>(obj);
		if (!player) return -1;

		m_characterNum = player->GetSubPlayerManager()->GetFollowNum();

		if (m_characterNum < 15)
		{
			m_characterNum = 15;
		}
		if (m_characterNum > 30)
		{
			m_characterNum = 30;
		}
		return m_characterNum;

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
