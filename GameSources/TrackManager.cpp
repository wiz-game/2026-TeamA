/*!
@file TrackManager.cpp
@brief 軌跡の管理クラスの実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void PlayerTrackManager::UpdateTrack(const Vec3& playerPos, float width)
	{
		// 軌跡がないとき
		if (m_track.empty())
		{
			m_track.push_back({ playerPos, width });
			return;
		}

		// 最後の軌跡と今の場所を比較し、移動してたら軌跡を追加
		auto lastPos = m_track.back().position;
		auto dis = playerPos - lastPos;
		dis.y = 0;
		if (dis.length() > m_interval)
		{
			m_track.push_back({ playerPos, width });
			// 軌跡のノードの最大数を超えたら最初のノードを破棄
			if (m_track.size() > m_maxNode)
			{
				m_track.pop_front();
			}
		}
	}

	TrackNode PlayerTrackManager::GetNearTrackNode(const Vec3& pos)
	{
		// 軌跡がないとき
		if (m_track.empty())
		{
			return { pos, 10.0f };
		}

		// 最も近い軌跡ノードを探す
		int id = 0;
		float min = FLT_MAX;
		for (int i = 0; i < m_track.size(); i++)
		{
			auto trackPos = m_track.back().position;
			auto dis = pos - trackPos;
			if (dis.length() < min)
			{
				id = i;
			}
		}
		id += 1;
		if (id >= m_track.size())
		{
			id = m_track.size() - 1;
			if (m_track.size() >= 2)
			{
				id -= 1;
			}
		}
		return m_track[id];
	}

}
//end basecross
