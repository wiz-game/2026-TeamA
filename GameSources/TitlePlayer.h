/*!
@file Foo.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class TitlePlayer;

	class Walk
	{
	public:
		Walk(const const shared_ptr<TitlePlayer>& ptrOwner) : m_Owner(ptrOwner) {}

		virtual void OnCreate() = 0;
		virtual void OnUpdate() = 0;

	protected:
		weak_ptr<TitlePlayer> m_Owner;
	};

	class WalkNormal : public Walk
	{
	public:
		WalkNormal(const shared_ptr<TitlePlayer>& ptrOwner, const Vec3& direction);
		virtual ~WalkNormal() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	protected:
		Vec3 m_Direction = Vec3(1.0f, 0.0f, 0.0f);
		float m_Speed = 3.4f;
		float m_TimeCount = 0.0f;
		float m_Time = 10.0f;

	private:
	};

	class WalkJamp : public WalkNormal
	{
	public:
		WalkJamp(const shared_ptr<TitlePlayer>& ptrOwner, const Vec3& direction);
		virtual ~WalkJamp() {}

		void OnUpdate() override;

	private:
		float m_jampPower = 20.0f;
		float m_gravity = 0.7f;
		float m_yAcceleration = 0.0f;
		float m_yVelocity = 0.0f;
		float m_yPos = 0.0f;
	};

	class WalkDiagonal : public WalkNormal
	{
	public:
		WalkDiagonal(const shared_ptr<TitlePlayer>& ptrOwner, const Vec3& direction);
		virtual ~WalkDiagonal() {}

		void OnCreate() override;
		void OnUpdate() override;

	private:
		float m_flipCoolTimeCount = 0.0f;
		float m_flipCoolTime = 3.0f;
	};

	class TitlePlayer : public GameObject
	{
	public:
		TitlePlayer(const shared_ptr<Stage>& ptrStage, bool isGenerator, float time = 0.0f);
		virtual ~TitlePlayer() {}

		void OnCreate() override;
		void OnUpdate() override;

		void SetWalkType(shared_ptr<Walk> walk)
		{
			m_walkType = walk;
			m_walkType->OnCreate();
		}
		void WalkCompOperation();

	private:
		bool m_IsGenerator;
		float m_freezTimeCount = 0.0f;
		float m_freezTime;
		shared_ptr<Walk> m_walkType;
	};
}
//end basecross
