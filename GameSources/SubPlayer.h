/*!
@file Foo.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {


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

}
//end basecross
#pragma once
