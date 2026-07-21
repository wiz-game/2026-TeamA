/*!
@file SubPlayer.cpp
@brief 群れの実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	// 群れのキャラクターの初期化
	void SubPlayer::OnCreate()
	{
		// ドローコンポーネントを追加
		m_drawComp = AddComponent<PNTBoneModelDraw>();
		m_drawComp->SetMultiMeshResource(L"MODEL_PON");
		//draw->SetDiffuse(Col4(1, 0, 0, 1));
		m_drawComp->AddAnimation(L"ANIM_IDLE", 0, 120, true);
		m_drawComp->AddAnimation(L"ANIM_WALK", 140, 120, true);
		m_drawComp->ChangeCurrentAnimation(L"ANIM_IDLE");

		// モデルが少し浮いているので調整
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f), // scale
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, -0.5f, 0.0f)  // position
		);

		m_drawComp->SetMeshToTransformMatrix(spanMat);


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
		//col->SetDrawActive(true);

		AddTag(L"SubPlayer");
		col->AddExcludeCollisionTag(L"SubPlayer");

	}

	// 群れのキャラクターの更新
	void SubPlayer::OnUpdate()
	{
		m_state->Update();

		auto& app = App::GetApp();
		auto delta = app->GetElapsedTime();
		auto pos = m_transComp->GetPosition();

		// トランポリンによるバウンド移動処理
		if (m_accelerationForTrampolineBound > 0.0f)
		{
			m_velocityY += m_accelerationForTrampolineBound * delta;

			// バウンド加速度を重力分減退させる
			m_accelerationForTrampolineBound -= 9.8f * delta;
			if (m_accelerationForTrampolineBound < 0.0f) m_accelerationForTrampolineBound = 0.0f;
		}

		auto dis = m_playerPos - pos;
		if (dis.length() > 30.0f && !m_state->IsInState(SubPlayerMoveToTargetPositionState::Instance()))
		{
			m_state->ChangeState(SubPlayerStrayState::Instance());
		}

		if (pos.y < -100)
		{
			auto obj = m_player.lock();
			if (!obj) return;
			auto player = dynamic_pointer_cast<Player>(obj);
			if (!player)return;
			player->GetSubPlayerManager()->Erase(GetThis<SubPlayer>());
		}
	}

	void SubPlayer::SetAlive(bool isAlive)
	{
		SetUpdateActive(isAlive);
		SetDrawActive(isAlive);
		SetReadyFormation(false);
		m_velocity = { 0 };
		m_velocityY = 0;

	}
	bool SubPlayer::GetAlive()
	{
		return GetUpdateActive() && GetDrawActive();
	}

	bool SubPlayer::Stay()
	{
		auto delta = App::GetApp()->GetElapsedTime();

		auto pos = m_transComp->GetPosition();
		//重力
		m_velocityY += -9.8f * delta;

		m_velocity *= 0.9f;
		// プレイヤーへの反発
		{
			auto disVec = pos - m_playerPos;
			auto dis = disVec.length();
			if (dis > 0.001f && dis < 2.5f)
			{
				auto pForce = disVec.normalize() * (200.0f / dis);
				if (pForce.length() > 200.0f)
				{
					pForce = pForce.normalize() * 100.0f;
				}
				m_velocity += pForce * delta;
			}

		}

		// アニメーションの更新
		m_drawComp->UpdateAnimation(delta * 2.0f);

		pos += (m_velocity + Vec3(0.0f, m_velocityY, 0.0f)) * delta;
		m_transComp->SetPosition(pos);
		auto dis = m_playerPos - pos;
		//auto distance = Vec3(m_targetPos).length() + 1.0f;
		if (dis.length() > m_dis + 2.5f)
		{
			m_follow = true;
		}
		else
		{
			//m_follow = false;
		}

		if (dis.length() > 15.0f)
		{
			m_isFar = true;
			m_follow = true;
		}
		if (m_follow && !m_isReadyFormation)
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
		m_velocityY += -9.8f * delta;

		// プレイヤーとキャラクターとの距離
		auto dis = m_playerPos - pos;

		if (frame % 5 == m_randam)
		{
			auto trackMng = player->GetTrackManager();
			auto node = trackMng->GetNearTrackNode(pos);
			auto others = player->GetSubPlayerManager()->GetActiveSubPlayer();
			auto steeringForce = CalculateSteering(node, others, 2.0f, 1.8f);
			m_velocity += steeringForce * delta * 5;
			m_velocity.y = 0;
			if (m_velocity.length() > m_maxSpeed)
			{
				m_velocity = m_velocity.normalize() * m_maxSpeed;
			}
		}
		pos += (m_velocity + Vec3(0.0f, m_velocityY, 0.0f)) * delta;
		m_transComp->SetPosition(pos);

		// 回転処理
		auto rad = atan2f(m_velocity.x, m_velocity.z) + XM_PIDIV2;
		m_transComp->SetRotation(Vec3(0, rad, 0));

		// アニメーションの更新
		m_drawComp->UpdateAnimation(delta * 2.0f);

		auto others = player->GetSubPlayerManager()->GetActiveSubPlayer();
		if (playerVec.length() < 0.1f)
		{
			if (dis.length() < 3.0f)
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
						if (sub->GetStateMachine()->IsInState(SubPlayerStayState::Instance()) && !sub->GetIsFar())
						{
							auto otherPos = sub->GetComponent<Transform>()->GetPosition();
							auto otherDis = otherPos - pos;
							if (otherDis.length() < 1.5f)
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

	bool SubPlayer::MoveToTargetPosition()
	{
		auto delta = App::GetApp()->GetElapsedTime();

		if (m_player.expired()) return false;
		auto obj = m_player.lock();
		if (!obj) return false;
		auto player = dynamic_pointer_cast<Player>(obj);
		if (!player) return false;

		//重力
		m_velocityY += -9.8f * delta;

		auto pos = m_transComp->GetPosition();
		{
			TrackNode node{ m_targetPos, 15 };
			auto others = player->GetSubPlayerManager()->GetActiveSubPlayer();
			auto steeringForce = CalculateSteering(node, others, 4.0f, 1.0f, 100.0f);
			m_velocity += steeringForce * delta;
			m_velocity.y = 0;
			if (m_velocity.length() > m_maxSpeed * 1.5f)
			{
				m_velocity = m_velocity.normalize() * m_maxSpeed * 1.5f;
			}
		}
		pos += (m_velocity + Vec3(0.0f, m_velocityY, 0.0f)) * delta;
		m_transComp->SetPosition(pos);

		// 回転処理
		auto rad = atan2f(m_velocity.x, m_velocity.z) + XM_PI;
		m_transComp->SetRotation(Vec3(0, rad, 0));

		// アニメーションの更新
		m_drawComp->UpdateAnimation(delta * 2.0f);

		auto dis = m_targetPos - pos;
		auto others = player->GetSubPlayerManager()->GetActiveSubPlayer();
		if (dis.length() < 3.0f)
		{
			m_dis = dis.length();
			return true;
		}
		else
		{
			//for (auto& other : others)
			//{
			//	auto sub = dynamic_pointer_cast<SubPlayer>(other);
			//	if (sub)
			//	{
			//		if (sub->GetStateMachine()->IsInState(SubPlayerStayState::Instance()))
			//		{
			//			auto otherPos = sub->GetComponent<Transform>()->GetPosition();
			//			auto otherDis = otherPos - pos;
			//			if (otherDis.length() < 1.3f)
			//			{
			//				m_dis = dis.length();
			//				return true;
			//			}
			//		}
			//	}
			//}
		}

		return false;
	}

	bool SubPlayer::Stray()
	{
		auto delta = App::GetApp()->GetElapsedTime();

		auto pos = m_transComp->GetPosition();

		//重力
		m_velocityY += -9.8f * delta;

		m_velocity *= 0.9f;

		pos += (m_velocity + Vec3(0.0f, m_velocityY, 0.0f)) * delta;
		m_transComp->SetPosition(pos);
		auto dis = m_playerPos - pos;

		if (dis.length() < 10.0f)
		{
			return true;
		}

		if (dis.length() > 60.0f)
		{
			auto obj = m_player.lock();
			if (!obj) return false;
			auto player = dynamic_pointer_cast<Player>(obj);
			if (!player)return false;
			player->GetSubPlayerManager()->Erase(GetThis<SubPlayer>());
		}

		return false;
	}

	void SubPlayer::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		// Trampolineに乗ったとき跳ねるため、上向きの加速度が設定される
		// ゲームの違和感を軽減するため、上向きの初速も加える
		auto& ptrTrampoline = dynamic_pointer_cast<Trampoline>(Other);
		if (ptrTrampoline)
		{
			auto trampolineTrans = ptrTrampoline->GetComponent<Transform>();
			auto trampolinePos = trampolineTrans->GetPosition();
			auto trampolineScale = trampolineTrans->GetScale();

			// 衝突時トランポリンより高い位置にいるときだけ跳ねる
			if (m_transComp->GetPosition().y - m_transComp->GetScale().y * 0.5f > trampolinePos.y + trampolineScale.y * 0.5f)
			{
				m_accelerationForTrampolineBound = ptrTrampoline->GetBoundPower();
				m_velocityY = m_velocityY < 0.0f ? 0.0f : m_velocityY;
				m_velocityY = m_accelerationForTrampolineBound * 0.5f;
			}
		}
	}

	void SubPlayer::OnCollisionExcute(shared_ptr<GameObject>& Other)
	{
		auto same = dynamic_pointer_cast<SubPlayer>(Other);
		if (same)return;

		auto otherTrans = Other->GetComponent<Transform>();
		auto otherPos = otherTrans->GetPosition();
		auto otherRot = otherTrans->GetQuaternion().toRotVec();
		auto otherScale = otherTrans->GetScale();
		auto pos = GetComponent<Transform>()->GetPosition();
		auto scale = GetComponent<Transform>()->GetScale();
		auto dis = pos - otherPos;
		auto scaleSum = scale + otherScale;
		bool isRotX = otherRot.x != 0.0f;
		bool isRotY = otherRot.y != 0.0f;
		bool isRotZ = otherRot.x != 0.0f;
		if (isRotX || isRotZ)
		{
			m_velocity.y = 0;
			m_velocityY = 0;
			pos.y += 0.01f;
			m_transComp->SetPosition(pos);
		}

		if ((scaleSum.y / 2.0f) - dis.y <= scale.y)
		{
			m_velocity.y = 0;
			m_velocityY = 0;
			pos.y += 0.01f;
			//m_position = pos;
			m_transComp->SetPosition(pos);
		}
		if (Other->FindTag(L"Cube"))
		{
			m_velocity.y = 0;
			m_velocityY = 0;
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

	Vec3 SubPlayer::CalculateSteering(const TrackNode& targetNode, const vector<shared_ptr<GameObject>> subPlayers, float seekBase, float sepBase, float playerSep)
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
		float seekWeight = (1.0f + (1.0f - spreadRatio) * 2.0f) * seekBase;
		// Separationは狭いと弱くなる
		float sepWeight = spreadRatio * sepBase * separateRatio + sepRatio;

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
				if (dis > 0.001f && dis < 2.5f)
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

		// プレイヤーへの反発
		{
			auto disVec = pos - m_playerPos;
			auto dis = disVec.length();
			if (dis > 0.001f && dis < 2.5f)
			{
				auto pForce = disVec.normalize() * (playerSep / dis);
				if (pForce.length() > playerSep)
				{
					pForce = pForce.normalize() * playerSep;
				}
				force += pForce;
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

}
//end basecross
