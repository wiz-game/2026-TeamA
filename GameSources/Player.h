/*!
@file Player.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"
#include "JoltRigidBody.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>


namespace basecross {
	// GameObjectクラスを継承した「Player」クラスを定義
	class Player : public GameObject // GameObjectクラスの継承【必須】
	{
		std::shared_ptr<Transform> m_transform; // トランスフォームはよく使うのでメンバにしておく
		//std::shared_ptr<PNTDXModelDraw> m_draw; // ドローコンポーネント
		std::shared_ptr<PNTStaticDraw> m_draw; // ドローコンポーネント

		Vec3 m_position; // プレイヤーの位置
		Vec3 m_rotation; // プレイヤーの回転
		Vec3 m_scale;    // プレイヤーのスケーリング

		const static int MAX_CHARACTER_NUM = 150;
		vector<shared_ptr<GameObject>> m_subPlayers; // 群れのキャラクター
		shared_ptr<GameObject> m_hammer; // ハンマーのオブジェクト
		shared_ptr<GameObject> m_cube; // キューブのオブジェクト
		Vec3 m_characterPositions[MAX_CHARACTER_NUM]; // 
		int m_activeNum; // 
		bool m_allMove; // すべての群れを動かすためのフラグ

		std::unique_ptr<JPH::CharacterVirtual> m_character;
		JPH::PhysicsSystem* m_pPhysicsSystem = nullptr;
		Vec3 m_desiredVelocity;
		JPH::ObjectLayer m_objectLayer;

		void InitializeCharacter();
		void UpdateCharacter(float deltaTime);
		void AllCharacterMove();

	public:
		// ステージを引数にしたコンストラクタ【必須】
		Player(const std::shared_ptr<Stage>& stage) :
			GameObject(stage), // ステージをGameObjectに渡す【必須】
			m_position(0.0f, 1.0f, 0.0f), // プレイヤーの初期位置を設定
			m_rotation(0.0f, 0.0f, 0.0f), // プレイヤーの初期回転を設定
			m_scale(1.0f),     // プレイヤーの初期スケーリングを設定
			m_activeNum(0)
		{
		}
		Player(const std::shared_ptr<Stage>& stage, Vec3 scale) :
			GameObject(stage), // ステージをGameObjectに渡す【必須】
			m_position(0.0f, 1.0f, 0.0f), // プレイヤーの初期位置を設定
			m_rotation(0.0f, 0.0f, 0.0f), // プレイヤーの初期回転を設定
			m_scale(scale),     // プレイヤーの初期スケーリングを設定
			m_activeNum(0)     // プレイヤーの初期スケーリングを設定
		{
		}

		void OnCreate() override; // 初期設定用の関数(UnityのStartメソッドに相当)
		void OnUpdate() override; // 毎フレーム実行される関数(UnityのUpdateメソッドに相当)
		//void OnDraw() override;

		Vec3 GetPosition() { return m_position; }
		void SetPosition(const Vec3& pos) { m_position = pos; }
		Vec3 GetRotation() { return m_rotation; }
		void SetRotation(const Vec3& rot) { m_rotation = rot; }
		Vec3 GetScale() { return m_scale; }
		void SetScale(const Vec3& scale) { m_scale = scale; }
		Vec3 GetMoveVelocity() { return m_desiredVelocity; }

		void SetAllMove(bool allMove) { m_allMove = allMove; }
		void AddSubPlayer(int num);
		bool EraseSubPlayer(int num);

		void OnCollisionEnter(const shared_ptr<GameObject>& other);
	};

	// 群れのキャラクター
	class SubPlayer : public GameObject
	{
		Vec3 m_targetPos;
		Vec3 m_playerPos;
		shared_ptr<Transform> m_transComp;
		float m_rotate;
		float m_dif;
		float m_stay;
		bool m_follow;
		unique_ptr<StateMachine<SubPlayer>> m_state;
		weak_ptr<GameObject> m_player;

	public:
		// コンストラクタ
		SubPlayer(const std::shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_targetPos(Vec3(0)),
			m_playerPos(Vec3(0)),
			m_rotate(0),
			m_dif(0),
			m_stay(0),
			m_follow(false)
		{
		}
		SubPlayer(const std::shared_ptr<Stage>& stage, const Vec3& pos) :
			GameObject(stage),
			m_targetPos(pos),
			m_playerPos(Vec3(0)),
			m_rotate(0),
			m_dif(0),
			m_stay(0),
			m_follow(false)
		{
		}


		void OnCreate() override; // 初期化
		void OnUpdate() override; // 更新
		void SetTargetPos(const Vec3& pos) { m_targetPos = pos; }
		void SetPlayerPos(const Vec3& pos) { m_playerPos = pos; }
		void SetRotate(float rotate) { m_rotate = rotate; }
		void SetAlive(bool isAlive); // 生きているかどうかのセッター
		bool GetAlive(); // 生きているかどうかのゲッター
		void SetFollow(bool follow) { m_follow = follow; }
		bool Stay();
		bool FollowPlayer();
		void SetPlayer(const shared_ptr<GameObject>& player) { m_player = player; }
		shared_ptr<GameObject> GetPlayer();
		//ステートマシンのゲッター
		const unique_ptr<StateMachine<SubPlayer>>& GetStateMachine() {
			return m_state;
		}
	};

	class HammerFormation : public GameObject
	{
		shared_ptr<Transform> m_transComp;
		weak_ptr<Player> m_player;
		Vec3 m_rotation;
		bool m_isActive;
	public:
		HammerFormation(const std::shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_isActive(true)
		{

		}

		void OnCreate() override; // 初期化
		void OnUpdate() override; // 更新

		void Start(const Vec3& position, const Vec3& rotation);
		void SetPlayer(const shared_ptr<Player>& player) { m_player = player; }
	};

	class CubeFormation : public GameObject
	{
		shared_ptr<Transform> m_transComp;
		weak_ptr<Player> m_player;
		Vec3 m_rotation;
		bool m_isActive;
		float m_time;

		shared_ptr<JoltRigidBody> m_rigidBody;
	public:
		CubeFormation(const std::shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_isActive(false)
		{
		}

		void OnCreate() override; // 初期化
		void OnUpdate() override; // 更新

		void Start(const Vec3& position, const Vec3& rotation);
		void SetPlayer(const shared_ptr<Player>& player) { m_player = player; }

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

	class SubPlayerStayState : public ObjState<SubPlayer>
	{
	public:
		SubPlayerStayState() {}

		static shared_ptr<SubPlayerStayState> Instance();
		void Enter(const shared_ptr<SubPlayer>& obj) override;
		void Execute(const shared_ptr<SubPlayer>& obj) override;
		void Exit(const shared_ptr<SubPlayer>& obj) override;

	};

	class SubPlayerFollowState : public ObjState<SubPlayer>
	{
	public:
		SubPlayerFollowState() {}

		static shared_ptr<SubPlayerFollowState> Instance();
		void Enter(const shared_ptr<SubPlayer>& obj) override;
		void Execute(const shared_ptr<SubPlayer>& obj) override;
		void Exit(const shared_ptr<SubPlayer>& obj) override;

	};

}
//end basecross

