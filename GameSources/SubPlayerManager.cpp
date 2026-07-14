/*!
@file SubPlayerManager.cpp
@brief 群れの管理クラスの実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void SubPlayerManager::Init(const shared_ptr<Stage>& stage, const shared_ptr<GameObject>& player)
	{
		for (int i = 0; i < MAX_CHARACTER_NUM; i++)
		{
			auto subPlayer = stage->AddGameObject<SubPlayer>();
			subPlayer->SetAlive(false);
			subPlayer->SetPlayer(player);
			m_subPlayers.push_back(subPlayer);
		}

	}

	void SubPlayerManager::Add(int num, const Vec3& pos)
	{
		for (auto& subPlayer : m_subPlayers)
		{
			if (num <= 0)
			{
				break;
			}
			if (!subPlayer->GetAlive())
			{
				subPlayer->SetAlive(true);
				int ran = rand() % 6 + 1;
				int x = ran * ((ran % 2 == 0) ? 1 : -1);
				ran = rand() % 6 + 1;
				int z = ran * ((ran % 2 == 0) ? 1 : -1);
				Vec3 v = { (float)x, 0.5f, (float)z };
				subPlayer->SetPosition(v + pos);
				//subPlayer->SetTargetPos(m_characterPositions[m_activeNum]);
				subPlayer->GetStateMachine()->ChangeState(SubPlayerFollowState::Instance());
				m_activeNum++;
				num--;
			}
		}

	}

	bool SubPlayerManager::Erase(int num)
	{
		int rest = 0;
		for (auto& subPlayer : m_subPlayers)
		{
			if (subPlayer->GetAlive())
			{
				rest++;
			}
		}
		if (rest < num)
		{
			return false;
		}
		for (auto& subPlayer : m_subPlayers)
		{
			if (num <= 0)
			{
				break;
			}
			if (subPlayer->GetAlive())
			{
				subPlayer->SetAlive(false);
				m_activeNum--;
				num--;
			}
		}

		return true;

	}

	bool SubPlayerManager::Erase()
	{
		for (auto& suba : m_formationMenber)
		{
			auto sub = suba.lock();
			for (auto& obj : GetActiveSubPlayer())
			{
				auto active = dynamic_pointer_cast<SubPlayer>(obj);
				if (sub == active)
				{
					sub->SetAlive(false);
					m_activeNum--;
					break;
				}
			}
		}
		m_formationMenber.clear();
		return true;

	}
	bool SubPlayerManager::Erase(const shared_ptr<SubPlayer>& subPlayer)
	{
		for (auto& obj : GetActiveSubPlayer())
		{
			auto active = dynamic_pointer_cast<SubPlayer>(obj);
			if (subPlayer == active)
			{
				subPlayer->SetAlive(false);
				m_activeNum--;
				break;
			}
		}
		return true;

	}

	void SubPlayerManager::AllCharacterMove()
	{
		for (auto& subPlayer : m_subPlayers)
		{
			if (subPlayer->GetAlive())
			{
				subPlayer->SetFollow(true);
			}
		}
	}

	vector<shared_ptr<GameObject>> SubPlayerManager::GetActiveSubPlayer()
	{
		vector<shared_ptr<GameObject>> objs;
		for (auto& obj : m_subPlayers)
		{
			auto subPlayer = dynamic_pointer_cast<SubPlayer>(obj);
			if (subPlayer)
			{
				if (subPlayer->GetAlive())
				{
					objs.push_back(obj);
				}
			}
		}

		return objs;
	}


	void SubPlayerManager::SetPlayerPos(const Vec3& pos)
	{
		// 群れに移動用の座標を送る
		for (auto& subPlayer : m_subPlayers)
		{
			if (subPlayer)
			{
				if (subPlayer->GetAlive())
				{
					//subPlayer->SetTargetPos(m_position - Vec3(cosf(m_rotation.y), 0, -sinf(m_rotation.y)) * 7.0f);
					subPlayer->SetPlayerPos(pos);
					//subPlayer->SetRotate(m_rotation.y);
				}
			}
		}
	}

	bool SubPlayerManager::StartForamtionMove(int num, const Vec3& pos)
	{
		if (m_activeNum < num)
		{
			return false;
		}

		int followNum = GetFollowNum();
		//for (auto& subPlayer : m_subPlayers)
		//{
		//	if (subPlayer->GetAlive() && !subPlayer->GetStateMachine()->IsInState(SubPlayerStrayState::Instance()))
		//	{
		//		followNum++;
		//	}
		//}
		if (followNum < num) return false;

		m_formationMenber.clear();
		vector<weak_ptr<SubPlayer>> nears;
		for (auto& subPlayer : m_subPlayers)
		{
			if (subPlayer->GetAlive() && !subPlayer->GetStateMachine()->IsInState(SubPlayerStrayState::Instance()))
			{
				if (num > 0)
				{
					//subPlayer->GetStateMachine()->ChangeState(SubPlayerMoveToTargetPositionState::Instance());
					//subPlayer->SetTargetPos(pos);
					//subPlayer->SetReadyFormation(true);
					nears.push_back(subPlayer);
					num--;
				}
				else
				{
					Vec3 dis = pos - subPlayer->GetComponent<Transform>()->GetPosition();
					float max = 0;
					int maxNum = 0;
					for (int i = 0; i < nears.size(); i++)
					{
						auto nearSub = nears[i].lock();
						if (!nearSub) continue;
						Vec3 com = pos - nearSub->GetComponent<Transform>()->GetPosition();
						if (max < com.length())
						{
							max = com.length();
							maxNum = i;
						}
					}
					if (dis.length() < max)
					{
						nears[maxNum] = subPlayer;
					}
				}
			}
		}
		m_formationMenber = nears;
		for (auto& menber : m_formationMenber)
		{
			auto sub = menber.lock();
			if (sub)
			{
				sub->GetStateMachine()->ChangeState(SubPlayerMoveToTargetPositionState::Instance());
				sub->SetTargetPos(pos);

			}
		}
		m_targetPos = pos;
		return true;
	}

	void SubPlayerManager::ResetFormationMenber()
	{
		for (auto& menber : m_formationMenber)
		{
			auto sub = menber.lock();
			if (sub)
			{
				sub->GetStateMachine()->ChangeState(SubPlayerFollowState::Instance());
			}
		}
		m_formationMenber.clear();
	}

	int SubPlayerManager::GetFollowNum()
	{
		int followNum = 0;
		for (auto& subPlayer : m_subPlayers)
		{
			if (subPlayer->GetAlive() && !subPlayer->GetStateMachine()->IsInState(SubPlayerStrayState::Instance()))
			{
				followNum++;
			}
		}

		return followNum;
	}

	bool SubPlayerManager::CheckFormationReady()
	{
		bool isCheck = true;
		if (m_formationMenber.empty()) return false;
		for (auto& obj : m_formationMenber)
		{
			auto sub = obj.lock();
			if (sub)
			{
				isCheck = isCheck && sub->GetStateMachine()->IsInState(SubPlayerStayState::Instance()) && sub->GetReadyFormation();
			}
		}

		return isCheck;
	}

}
//end basecross
