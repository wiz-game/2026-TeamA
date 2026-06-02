#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	class Enemy : public StageObject
	{

		shared_ptr<Transform> m_transComp;
		shared_ptr<PNTStaticDraw> m_drawComp;
		void InitTransComp();
		void InitDrawComp();
		int hp;

		int damage;
		weak_ptr<Player> m_player;
		Vec3 enemyScale;
		Vec3 enemyPos;
		Vec3 playerScale;
		Vec3 playerPos;

		void SetValue();
		void OnCollisionEnter(shared_ptr<GameObject>& other)override;

		bool isHit;
		float timer;
		float elapsedTime;

		float rx;
		float rz;
		Vec3 dir;

		Vec3 diff;
		float distance;

		float followRange = 5.0f; //追尾を開始する距離
		float followSpeed = 1.5f;//追尾時の速度

		BrokenObjs m_broken;

		unique_ptr<StateMachine<Enemy>> m_stateMachine;
	public:
		Enemy(shared_ptr<Stage>& stage);
		~Enemy();

		void OnCreate()override;
		void OnUpdate()override;

		void Wondering();
		void Chasing();


	};

	class WonderingState : public ObjState<Enemy>
	{
		WonderingState() {}
	public:
		static shared_ptr<WonderingState> Instance();
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;

	};

	class ChasingState : public ObjState<Enemy>
	{
		ChasingState(){}
	public:
		static shared_ptr<ChasingState> Instance();
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;

	};

}