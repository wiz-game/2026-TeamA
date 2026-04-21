/*!
@file Foo.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class StageManager
	{
	private:
		struct StageManagerDeleter
		{
			void operator()(StageManager* p) { delete p; }
		};
		static unique_ptr<StageManager, StageManagerDeleter> m_StageManager;

		StageManager();
		virtual ~StageManager(){}

	public:
		/*!
		@brief ステージマネージャーを作成
		@return 作成したステージマネージャー
		*/
	};

}
//end basecross
