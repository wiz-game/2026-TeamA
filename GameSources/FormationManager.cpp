/*!
@file FormationManager.cpp
@brief 隊列の管理クラスの実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void FormationManager::Init(const shared_ptr<Stage>& stage, const shared_ptr<GameObject> player)
	{
		m_formation[0] = stage->AddGameObject<HammerFormation>();
		m_formation[1] = stage->AddGameObject<CubeFormation>();
		m_formation[2] = stage->AddGameObject<SpearFormation>();
		m_formation[3] = stage->AddGameObject<BridgeFormation>();

		for (int i = 0; i < 4; i++)
		{
			auto formation = m_formation[i].lock();
			if (formation)
			{
				formation->SetUpdateActive(false);
				formation->SetDrawActive(false);
				formation->SetPlayer(player);
			}
		}

	}

	void FormationManager::StartFormation(const Vec3& pos, const Vec3& rot)
	{
		auto formation = m_formation[m_formationNum].lock();
		if (formation)
		{
			formation->Start(pos, rot);
		}
	}

	void FormationManager::FinishFormation()
	{
		auto formation = m_formation[m_formationNum].lock();
		if (formation)
		{
			formation->Finish();
		}
	}

	void FormationManager::DrawFormationRange(const Vec3& pos, const Vec3& rot)
	{
		auto formation = m_formation[m_formationNum].lock();
		if (formation)
		{
			formation->EffectRangeDraw(pos, rot);
		}
	}

	void FormationManager::ResetDraw()
	{
		auto formation = m_formation[m_formationNum].lock();
		if (formation)
		{
			formation->SetDrawActive(false);
		}
	}

	bool FormationManager::GetFormationActive()
	{
		auto formation = m_formation[m_formationNum].lock();
		if (formation)
		{
			return formation->GetActive();
		}

		return false;
	}

	int FormationManager::GetFormationCharacterNum()
	{
		auto formation = m_formation[m_formationNum].lock();
		if (formation)
		{
			return formation->GetCharacterNum();
		}

		return -1;
	}

}
//end basecross
