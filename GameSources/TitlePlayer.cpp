/*!
@file Foo.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"
#include "TitlePlayer.h"

namespace basecross
{
	WalkNormal::WalkNormal(const shared_ptr<TitlePlayer>& ptrOwner, const Vec3& direction)
		: Walk(ptrOwner)
		, m_Direction(direction)
	{
	}

	void WalkNormal::OnCreate()
	{
		auto ptrDraw = m_Owner.lock()->GetComponent<PNTBoneModelDraw>();
		ptrDraw->ChangeCurrentAnimation(L"ANIM_WALK");
	}
	
	void WalkNormal::OnUpdate()
	{
		auto& app = App::GetApp();
		auto delta = app->GetElapsedTime();

		auto ptrTrans = m_Owner.lock()->GetComponent<Transform>();

		auto pos = ptrTrans->GetPosition();
		pos += m_Direction.normalize() * m_Speed * delta;
		ptrTrans->SetPosition(pos);

		auto quat = Quat();
		auto defaultForward = Vec3(0.0f, 0.0f, 1.0f);
		auto dire = m_Direction;
		dire.normalize();
		auto dot = defaultForward.dot(dire);
		auto rad = acosf(dot / defaultForward.length() * dire.length());
		if (m_Direction.x < 0.0f) rad = -rad;
		quat = Quat(Vec3(0.0f, 1.0f, 0.0f), rad);
		ptrTrans->SetQuaternion(quat);

		m_TimeCount += delta;
		if (m_TimeCount > m_Time)
		{
			m_Owner.lock()->WalkCompOperation();
			m_TimeCount - m_Time;
		}
	}

	WalkJamp::WalkJamp(const shared_ptr<TitlePlayer>& ptrOwner, const Vec3& direction)
		: WalkNormal(ptrOwner, direction)
	{
	}

	void WalkJamp::OnUpdate()
	{
		WalkNormal::OnUpdate();

		auto& app = App::GetApp();
		auto delta = app->GetElapsedTime();

		auto ptrTrans = m_Owner.lock()->GetComponent<Transform>();

		auto pos = ptrTrans->GetPosition();
		m_yPos = pos.y;
		m_yPos += m_yAcceleration * delta;
		if (m_yPos < 0.0f)
		{
			m_yPos = 0.0f;
			m_yVelocity = 0.0f;
			m_yAcceleration = m_jampPower;
		}
		pos.y = m_yPos;
		ptrTrans->SetPosition(pos);

		m_yVelocity += m_yAcceleration * delta;
		m_yAcceleration -= m_gravity;
	}

	WalkDiagonal::WalkDiagonal(const shared_ptr<TitlePlayer>& ptrOwner, const Vec3& direction)
		: WalkNormal(ptrOwner, direction)
	{
	}

	void WalkDiagonal::OnCreate()
	{
		WalkNormal::OnCreate();
		m_Direction = Vec3(m_Direction.x, 0.0f, 1.0f).normalize();
		m_Time *= 1.414f;
		m_flipCoolTimeCount = m_flipCoolTime * 0.5f;
	}

	void WalkDiagonal::OnUpdate()
	{
		WalkNormal::OnUpdate();

		auto& app = App::GetApp();
		auto delta = app->GetElapsedTime();

		m_flipCoolTimeCount += delta;
		if (m_flipCoolTimeCount > m_flipCoolTime)
		{
			m_Direction.z = -m_Direction.z;
			m_flipCoolTimeCount -= m_flipCoolTime;
		}
	}

	TitlePlayer::TitlePlayer(const shared_ptr<Stage>& ptrStage, bool isGenerator, float time)
		: GameObject(ptrStage)
		, m_IsGenerator(isGenerator)
		, m_freezTime(time)
	{
	}

	void basecross::TitlePlayer::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();

		if (m_IsGenerator)
		{
			ptrTrans->SetPosition(Vec3(-17.0f, 0.0f, 0.0f)); // 初期位置を設定
			ptrTrans->SetScale(Vec3(2.0f)); // スケーリングを設定
		}

		auto ptrDraw = AddComponent<PNTBoneModelDraw>();
		ptrDraw->SetMultiMeshResource(L"MODEL_PON");
		
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.7f),
			Vec3(0.0f, 0.0f, 0.0f),
			Quat(Vec3(0.0f, 1.0f, 0.0f), XM_PIDIV2),
			Vec3(0.0f, -0.5f, 0.0f)
		);
		ptrDraw->SetMeshToTransformMatrix(spanMat);

		ptrDraw->AddAnimation(L"ANIM_IDLE", 0, 120, true);
		ptrDraw->AddAnimation(L"ANIM_WALK", 140, 120, true);
		ptrDraw->ChangeCurrentAnimation(L"ANIM_IDLE");

		if (m_IsGenerator)
		{
			m_walkType = make_shared<WalkNormal>(GetThis<TitlePlayer>(), Vec3(1.0f, 0.0f, 0.0f));
			m_walkType->OnCreate();
		}
	}

	void TitlePlayer::OnUpdate()
	{
		auto& app = App::GetApp();
		auto delta = app->GetElapsedTime();

		if (!m_IsGenerator)
		{
			m_freezTimeCount += delta;
			if (m_freezTimeCount < m_freezTime) return;
		}

		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->UpdateAnimation(delta);

		if (m_walkType != nullptr) m_walkType->OnUpdate();
	}

	void TitlePlayer::WalkCompOperation()
	{
		if (!m_IsGenerator)
		{
			GetStage()->RemoveGameObject<TitlePlayer>(GetThis<TitlePlayer>());
			return;
		}

		auto ptrTrans = GetComponent<Transform>();
		auto pos = ptrTrans->GetPosition();
		auto direction = Vec3();
		if (pos.x < 0.0f)
		{
			pos.x = -17.0f;
			direction = Vec3(1.0f, 0.0f, 0.0f);
		}
		else
		{
			pos.x = 17.0f;
			direction = Vec3(-1.0f, 0.0f, 0.0f);
		}
		pos = Vec3(pos.x, 0.0f, 0.0f);
		ptrTrans->SetPosition(pos);

		auto type = 0;
		auto ptrNormal = dynamic_pointer_cast<WalkNormal>(m_walkType);
		if (ptrNormal != nullptr)
		{
			type = 0;
		}

		auto ptrJamp = dynamic_pointer_cast<WalkJamp>(m_walkType);
		if (ptrJamp != nullptr)
		{
			type = 1;
		}

		auto ptrDiagonal = dynamic_pointer_cast<WalkDiagonal>(m_walkType);
		if (ptrDiagonal != nullptr)
		{
			type = 2;
		}
		m_walkType = nullptr;

		switch (type)
		{
		case 0:
			m_walkType = make_shared<WalkJamp>(GetThis<TitlePlayer>(), direction);
			break;
		case 1:
			m_walkType = make_shared<WalkDiagonal>(GetThis<TitlePlayer>(), direction);
			break;
		case 2:
			m_walkType = make_shared<WalkNormal>(GetThis<TitlePlayer>(), direction);
			break;
		default:
			m_walkType = make_shared<WalkNormal>(GetThis<TitlePlayer>(), direction);
			break;
		}
		if(m_walkType != nullptr) m_walkType->OnCreate();

		int loop = rand() % 30 - 10;
		if (loop < 0) loop = 0;
		for (int i = 0; i < 10; i++)
		{
			auto freezTime = float(rand() % 150) / 100;
			auto ptrNewPlayer = GetStage()->AddGameObject<TitlePlayer>(false, freezTime);
			switch (type)
			{
			case 0:
				ptrNewPlayer->SetWalkType(make_shared<WalkJamp>(ptrNewPlayer, direction));
				break;
			case 1:
				ptrNewPlayer->SetWalkType(make_shared<WalkDiagonal>(ptrNewPlayer, direction));
				break;
			case 2:
				ptrNewPlayer->SetWalkType(make_shared<WalkNormal>(ptrNewPlayer, direction));
				break;
			default:
				ptrNewPlayer->SetWalkType(make_shared<WalkNormal>(ptrNewPlayer, direction));
				break;
			}

			auto ptrNewPlayerTrans = ptrNewPlayer->GetComponent<Transform>();

			float xPos = float(rand() % 400 - 200) / 100;
			float zPos = float(rand() % 400 - 200) / 100;
			ptrNewPlayerTrans->SetPosition(Vec3(pos.x + xPos, 0.0f, zPos));
		}
	}
}
//end basecross
