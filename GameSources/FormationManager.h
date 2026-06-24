/*!
@file FormationManager.h
@brief 隊列の管理クラス
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {


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

}
//end basecross
#pragma once
