/*!
@file SubPlayerManager.h
@brief 群れの管理クラス
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {


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
		void ResetFormationMenber();
		int GetActiveNum() { return m_activeNum; }
		int GetFollowNum();
		bool CheckFormationReady();
		Vec3 GetTargetPos() { return m_targetPos; }
	};

}
//end basecross
#pragma once
