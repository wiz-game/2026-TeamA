/*!
@file Foo.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

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

}
//end basecross
#pragma once
