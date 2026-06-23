/*!
@file Foo.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	shared_ptr<SubPlayerStayState> SubPlayerStayState::Instance()
	{
		static shared_ptr<SubPlayerStayState> instance(new SubPlayerStayState);
		return instance;
	}

	void SubPlayerStayState::Enter(const shared_ptr<SubPlayer>& obj)
	{
		obj->GetComponent<PNTBoneModelDraw>()->ChangeCurrentAnimation(L"ANIM_IDLE");
	}
	void SubPlayerStayState::Execute(const shared_ptr<SubPlayer>& obj)
	{
		if (obj->Stay())
		{
			obj->GetStateMachine()->ChangeState(SubPlayerFollowState::Instance());
		}
	}
	void SubPlayerStayState::Exit(const shared_ptr<SubPlayer>& obj)
	{

	}

	shared_ptr<SubPlayerFollowState> SubPlayerFollowState::Instance()
	{
		static shared_ptr<SubPlayerFollowState> instance(new SubPlayerFollowState);
		return instance;
	}

	void SubPlayerFollowState::Enter(const shared_ptr<SubPlayer>& obj)
	{
		auto gameObj = obj->GetPlayer();
		auto player = dynamic_pointer_cast<Player>(gameObj);
		if (player)
		{
			player->GetSubPlayerManager()->SetAllMove(true);
			obj->SetReadyFormation(false);
		}
		obj->GetComponent<PNTBoneModelDraw>()->ChangeCurrentAnimation(L"ANIM_WALK");
	}
	void SubPlayerFollowState::Execute(const shared_ptr<SubPlayer>& obj)
	{
		if (obj->FollowPlayer())
		{
			obj->GetStateMachine()->ChangeState(SubPlayerStayState::Instance());
		}
	}
	void SubPlayerFollowState::Exit(const shared_ptr<SubPlayer>& obj)
	{

	}

	shared_ptr<SubPlayerMoveToTargetPositionState> SubPlayerMoveToTargetPositionState::Instance()
	{
		static shared_ptr<SubPlayerMoveToTargetPositionState> instance(new SubPlayerMoveToTargetPositionState);
		return instance;
	}

	void SubPlayerMoveToTargetPositionState::Enter(const shared_ptr<SubPlayer>& obj)
	{
		//obj->GetComponent<PNTBoneModelDraw>()->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));
		obj->GetComponent<PNTBoneModelDraw>()->ChangeCurrentAnimation(L"ANIM_WALK");
	}
	void SubPlayerMoveToTargetPositionState::Execute(const shared_ptr<SubPlayer>& obj)
	{
		if (obj->MoveToTargetPosition())
		{
			obj->GetStateMachine()->ChangeState(SubPlayerStayState::Instance());
		}
	}
	void SubPlayerMoveToTargetPositionState::Exit(const shared_ptr<SubPlayer>& obj)
	{
		obj->SetReadyFormation(true);
		//obj->GetComponent<PNTBoneModelDraw>()->SetDiffuse(Col4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	shared_ptr<SubPlayerStrayState> SubPlayerStrayState::Instance()
	{
		static shared_ptr<SubPlayerStrayState> instance(new SubPlayerStrayState);
		return instance;
	}

	void SubPlayerStrayState::Enter(const shared_ptr<SubPlayer>& obj)
	{
		obj->GetComponent<PNTBoneModelDraw>()->ChangeCurrentAnimation(L"ANIM_IDLE");
	}
	void SubPlayerStrayState::Execute(const shared_ptr<SubPlayer>& obj)
	{
		if (obj->Stray())
		{
			obj->GetStateMachine()->ChangeState(SubPlayerFollowState::Instance());
		}
	}
	void SubPlayerStrayState::Exit(const shared_ptr<SubPlayer>& obj)
	{

	}

}
//end basecross
