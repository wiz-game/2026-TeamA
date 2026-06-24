/*!
@file Player.h
@brief プレイヤー
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


	// GameObjectクラスを継承した「Player」クラスを定義
	class Player : public GameObject // GameObjectクラスの継承【必須】
	{
		std::shared_ptr<Transform> m_transform; // トランスフォームはよく使うのでメンバにしておく
		//std::shared_ptr<PNTDXModelDraw> m_draw; // ドローコンポーネント
		std::shared_ptr<PNTBoneModelDraw> m_drawComp; // ドローコンポーネント

		Vec3 m_position; // プレイヤーの位置
		Vec3 m_rotation; // プレイヤーの回転
		Vec3 m_scale;    // プレイヤーのスケーリング
		Vec3 m_velocity; // プレイヤーの移動ベクトル
		float m_velocityY; // Y方向の移動量
		float m_roadWidth = 10; // トラックに送る道幅
		Vec3 m_formationRot; // 隊列に送る回転
		bool m_isStartedFormation; // 隊列が始まっているかどうか
		float m_frmWaitTime; // フォーメーションが出来上がるまでの時間を数える
		wstring m_debugStr; // デバッグ用文字列
		bool m_drawFormationRange; // 隊列の範囲を表示するかどうか

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
		shared_ptr<SubPlayerManager> GetSubPlayerManager() const { return m_subPlayerMng; }


		virtual void OnCollisionEnter(shared_ptr<GameObject>& other) override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
	};




}
//end basecross

