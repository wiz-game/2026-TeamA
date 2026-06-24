/*!
@file CharacterFormation.h
@brief 群れの隊列
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "JoltRigidBody.h"
#include <deque>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>

namespace basecross {


	// 隊列オブジェクトのベース
	class CharacterFormation : public GameObject
	{
	protected:
		shared_ptr<Transform> m_transComp;
		shared_ptr<PNTStaticDraw> m_drawComp;
		weak_ptr<GameObject> m_player;
		Vec3 m_rotation;
		float m_time;
		bool m_isActive;
		int m_characterNum;
	public:
		CharacterFormation(const std::shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_time(0),
			m_isActive(false),
			m_characterNum(0)
		{

		}

		virtual void Start(const Vec3& position, const Vec3& rotation) {}
		virtual void Finish();
		virtual void EffectRangeDraw(const Vec3& position, const Vec3& rotation) {}
		bool GetActive() { return m_isActive; }
		void SetPlayer(const shared_ptr<GameObject>& player) { m_player = player; }
		virtual int GetCharacterNum() { return m_characterNum; }
	};

	class HammerFormation : public CharacterFormation
	{
	public:
		HammerFormation(const std::shared_ptr<Stage>& stage) :
			CharacterFormation(stage)
		{

		}

		void OnCreate() override; // 初期化
		void OnUpdate() override; // 更新

		void Start(const Vec3& position, const Vec3& rotation) override;
		void EffectRangeDraw(const Vec3& position, const Vec3& rotation) override;

	};

	class CubeFormation : public CharacterFormation
	{
		shared_ptr<JoltRigidBody> m_rigidBody;
		bool m_onPlayer;

	public:
		CubeFormation(const std::shared_ptr<Stage>& stage) :
			CharacterFormation(stage),
			m_onPlayer(false)
		{
		}

		void OnCreate() override; // 初期化
		void OnUpdate() override; // 更新
		void SetOnPlayer(bool b) { m_onPlayer = b; }
		void Start(const Vec3& position, const Vec3& rotation);
		void EffectRangeDraw(const Vec3& position, const Vec3& rotation) override;

	};

	class SpearFormation : public CharacterFormation
	{
		Vec3 m_position;
	public:
		SpearFormation(const std::shared_ptr<Stage>& stage) :
			CharacterFormation(stage)
		{

		}

		void OnCreate() override; // 初期化
		void OnUpdate() override; // 更新

		void Start(const Vec3& position, const Vec3& rotation) override;
		void EffectRangeDraw(const Vec3& position, const Vec3& rotation) override;

	};

	class BridgeFormation : public CharacterFormation
	{
		Vec3 m_baseScale;
		Vec3 m_position;
		bool m_onPlayer;
	public:
		BridgeFormation(const std::shared_ptr<Stage>& stage) :
			CharacterFormation(stage),
			m_baseScale(Vec3(10.0f, 1.0f, 40.0f)),
			m_position(Vec3(0)),
			m_onPlayer(false)
		{

		}

		void OnCreate() override; // 初期化
		void OnUpdate() override; // 更新
		void SetOnPlayer(bool b) { m_onPlayer = b; }
		void Start(const Vec3& position, const Vec3& rotation) override;
		void EffectRangeDraw(const Vec3& position, const Vec3& rotation) override;
		int GetCharacterNum() override;
	};

	class AttackCollisionObj : public GameObject
	{
		float m_time;
		Vec3 m_position;
		float m_rotation;
	public:
		AttackCollisionObj(const std::shared_ptr<Stage>& stage, const Vec3& pos, float rot) :
			GameObject(stage),
			m_time(0.0f),
			m_position(pos),
			m_rotation(rot)
		{

		}

		void OnCreate() override; // 初期化
		void OnUpdate() override; // 更新
	};

}
//end basecross
#pragma once
