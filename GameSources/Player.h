/*!
@file Player.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"
#include "JoltRigidBody.h"
#include <deque>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>


namespace basecross {

	// 軌跡ノード
	struct TrackNode
	{
		Vec3 position;
		float roadWidth;
	};

	// プレイヤーの移動の軌跡を管理するクラス
	class PlayerTrackManager
	{
	private:
		std::deque<TrackNode> m_track;
		size_t m_maxNode = 200; // ノードの最大数
		float m_interval = 2.0f; // ノードの間隔(2.0f)
	public:
		void UpdateTrack(const Vec3& playerPos, float width);
		TrackNode GetNearTrackNode(const Vec3& pos);
	};

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
		virtual void EffectRangeDraw(const Vec3& position, const Vec3& rotation){}
		bool GetActive() { return m_isActive; }
		void SetPlayer(const shared_ptr<GameObject>& player) { m_player = player; }
		int GetCharacterNum() { return m_characterNum; }
	};

	// 隊列オブジェクトの管理クラス
	class FormationManager
	{
	private:
		weak_ptr<CharacterFormation> m_formation[4];
		int m_formationNum;

	public:
		void Init(const shared_ptr<Stage>& stage, const shared_ptr<GameObject> player);
		void StartFormation(const Vec3& pos, const Vec3& rot);
		void FinishFormation();
		void DrawFormationRange(const Vec3& pos, const Vec3& rot);
		void ResetDraw();
		bool GetFormationActive();
		void SetFormationNumber(int num)
		{ 
			m_formationNum = num; 
			if (m_formationNum < 0)m_formationNum = 0;
			if (m_formationNum >= 4)m_formationNum = 3;
		}
		int GetFormationNumber() { return m_formationNum; }
		int GetFormationCharacterNum();
	};

	// 群れのキャラクター
	class SubPlayer : public GameObject
	{
		Vec3 m_targetPos;
		Vec3 m_playerPos;
		shared_ptr<Transform> m_transComp;
		shared_ptr<PNTBoneModelDraw> m_drawComp;
		//float m_rotate;
		float m_dif;
		float m_stay;
		bool m_follow;
		//float m_rad;
		//float m_len;
		unique_ptr<StateMachine<SubPlayer>> m_state;
		weak_ptr<GameObject> m_player;
		Vec3 m_velocity;
		float m_maxSpeed;
		int m_randam;
		float m_dis;
		Vec3 CalculateSteering(const TrackNode& targetNode, const vector<shared_ptr<GameObject>> subPlayers, float seekBase = 2.0f, float sepBase = 1.5f);
		bool m_isReadyFormation;
		float m_velocityY;
	public:
		// コンストラクタ
		SubPlayer(const std::shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_targetPos(Vec3(0)),
			m_playerPos(Vec3(0)),
			//m_rotate(0),
			m_dif(0),
			m_stay(0),
			m_follow(false),
			m_velocity(Vec3(0)),
			m_maxSpeed(9.0f),
			m_isReadyFormation(false),
			m_velocityY(0)
		{
		}

		void OnCreate() override; // 初期化
		void OnUpdate() override; // 更新
		void SetTargetPos(const Vec3& pos) { m_targetPos = pos; }
		void SetPlayerPos(const Vec3& pos) { m_playerPos = pos; }
		//void SetRotate(float rotate) { m_rotate = rotate; }
		void SetAlive(bool isAlive); // 生きているかどうかのセッター
		bool GetAlive(); // 生きているかどうかのゲッター
		void SetFollow(bool follow) { m_follow = follow; }
		bool Stay();
		bool FollowPlayer();
		bool MoveToTargetPosition();
		bool Stray();
		void SetPlayer(const shared_ptr<GameObject>& player) { m_player = player; }
		shared_ptr<GameObject> GetPlayer();
		Vec3 GetVelocity() { return m_velocity; }
		void SetPosition(const Vec3& pos);
		//ステートマシンのゲッター
		const unique_ptr<StateMachine<SubPlayer>>& GetStateMachine() {
			return m_state;
		}
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		void SetReadyFormation(bool b) { m_isReadyFormation = b; }
		bool GetReadyFormation() { return m_isReadyFormation; }
	};



	// 群れの隊列の管理クラス
	class SubPlayerManager
	{
	private:
		const static int MAX_CHARACTER_NUM = 100;
		int m_activeNum;
		bool m_allMove;
		Vec3 m_targetPos;
		vector<shared_ptr<SubPlayer>> m_subPlayers; // 群れのキャラクター
		vector<weak_ptr<SubPlayer>> m_formationMenber; // 隊列を組むために動くメンバー
	public:
		void Init(const shared_ptr<Stage>& stage, const shared_ptr<GameObject>& player);
		void Add(int num, const Vec3& pos);
		bool Erase(int num);
		bool Erase();
		bool Erase(const shared_ptr<SubPlayer>& subPlayer);
		void SetAllMove(bool allMove) { m_allMove = allMove; }
		bool GetAllMove() { return m_allMove; }
		vector<shared_ptr<GameObject>> GetActiveSubPlayer();
		void AllCharacterMove();
		void SetPlayerPos(const Vec3& pos);
		bool StartForamtionMove(int num, const Vec3& pos);
		int GetActiveNum() { return m_activeNum; }
		bool CheckFormationReady();
		Vec3 GetTargetPos() { return m_targetPos; }
	};

	// GameObjectクラスを継承した「Player」クラスを定義
	class Player : public GameObject // GameObjectクラスの継承【必須】
	{
		std::shared_ptr<Transform> m_transform; // トランスフォームはよく使うのでメンバにしておく
		//std::shared_ptr<PNTDXModelDraw> m_draw; // ドローコンポーネント
		std::shared_ptr<PNTStaticDraw> m_draw; // ドローコンポーネント

		Vec3 m_position; // プレイヤーの位置
		Vec3 m_rotation; // プレイヤーの回転
		Vec3 m_scale;    // プレイヤーのスケーリング
		Vec3 m_velocity; // プレイヤーの移動ベクトル
		float m_roadWidth = 10;
		Vec3 m_formationRot;
		bool m_isStartedFormation;

		std::unique_ptr<JPH::CharacterVirtual> m_character;
		JPH::PhysicsSystem* m_pPhysicsSystem = nullptr;
		Vec3 m_desiredVelocity;
		JPH::ObjectLayer m_objectLayer;

		shared_ptr<PlayerTrackManager> m_trackMng;
		shared_ptr<FormationManager> m_formationMng;
		shared_ptr<SubPlayerManager> m_subPlayerMng;

		void InitializeCharacter();
		void UpdateCharacter(float deltaTime);

	public:
		// ステージを引数にしたコンストラクタ【必須】
		Player(const std::shared_ptr<Stage>& stage) :
			GameObject(stage), // ステージをGameObjectに渡す【必須】
			m_position(0.0f, 0.0f, 0.0f), // プレイヤーの初期位置を設定
			m_rotation(0.0f, 0.0f, 0.0f), // プレイヤーの初期回転を設定
			m_scale(1.0f)     // プレイヤーの初期スケーリングを設定
		{
		}
		Player(const std::shared_ptr<Stage>& stage, Vec3 scale) :
			GameObject(stage), // ステージをGameObjectに渡す【必須】
			m_position(0.0f, 0.0f, 0.0f), // プレイヤーの初期位置を設定
			m_rotation(0.0f, 0.0f, 0.0f), // プレイヤーの初期回転を設定
			m_scale(scale)    // プレイヤーの初期スケーリングを設定
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

		shared_ptr<PlayerTrackManager> GetTrackManager() const { return m_trackMng; }
		shared_ptr<SubPlayerManager> GetSunbPlayerManager() const { return m_subPlayerMng; }


		virtual void OnCollisionEnter(shared_ptr<GameObject>& other) override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
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
		Vec3 m_position;
		bool m_onPlayer;
	public:
		BridgeFormation(const std::shared_ptr<Stage>& stage) :
			CharacterFormation(stage),
			m_position(Vec3(0)),
			m_onPlayer(false)
		{

		}

		void OnCreate() override; // 初期化
		void OnUpdate() override; // 更新
		void SetOnPlayer(bool b) { m_onPlayer = b; }
		void Start(const Vec3& position, const Vec3& rotation) override;
		void EffectRangeDraw(const Vec3& position, const Vec3& rotation) override;

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

	class SubPlayerMoveToTargetPositionState : public ObjState<SubPlayer>
	{
	public:
		SubPlayerMoveToTargetPositionState() {}

		static shared_ptr<SubPlayerMoveToTargetPositionState> Instance();
		void Enter(const shared_ptr<SubPlayer>& obj) override;
		void Execute(const shared_ptr<SubPlayer>& obj) override;
		void Exit(const shared_ptr<SubPlayer>& obj) override;

	};

	class SubPlayerStrayState : public ObjState<SubPlayer>
	{
	public:
		SubPlayerStrayState() {}

		static shared_ptr<SubPlayerStrayState> Instance();
		void Enter(const shared_ptr<SubPlayer>& obj) override;
		void Execute(const shared_ptr<SubPlayer>& obj) override;
		void Exit(const shared_ptr<SubPlayer>& obj) override;

	};

}
//end basecross

