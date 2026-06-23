/*!
@file Foo.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	class SubPlayerStayState : public ObjState<SubPlayer>
	{
	public:
		SubPlayerStayState() {}

		static shared_ptr<SubPlayerStayState> Instance();
		void Enter(const shared_ptr<SubPlayer>& obj) override;
		void Execute(const shared_ptr<SubPlayer>& obj) override;
		void Exit(const shared_ptr<SubPlayer>& obj) override;

	};

	class SubPlayerFollowState : public ObjState<SubPlayer>
	{
	public:
		SubPlayerFollowState() {}

		static shared_ptr<SubPlayerFollowState> Instance();
		void Enter(const shared_ptr<SubPlayer>& obj) override;
		void Execute(const shared_ptr<SubPlayer>& obj) override;
		void Exit(const shared_ptr<SubPlayer>& obj) override;

	};

	class SubPlayerMoveToTargetPositionState : public ObjState<SubPlayer>
	{
	public:
		SubPlayerMoveToTargetPositionState() {}

		static shared_ptr<SubPlayerMoveToTargetPositionState> Instance();
		void Enter(const shared_ptr<SubPlayer>& obj) override;
		void Execute(const shared_ptr<SubPlayer>& obj) override;
		void Exit(const shared_ptr<SubPlayer>& obj) override;

	};

	class SubPlayerStrayState : public ObjState<SubPlayer>
	{
	public:
		SubPlayerStrayState() {}

		static shared_ptr<SubPlayerStrayState> Instance();
		void Enter(const shared_ptr<SubPlayer>& obj) override;
		void Execute(const shared_ptr<SubPlayer>& obj) override;
		void Exit(const shared_ptr<SubPlayer>& obj) override;

	};

}
//end basecross
#pragma once
