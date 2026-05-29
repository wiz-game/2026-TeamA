#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// Enemyの徘徊中ステート
	shared_ptr<WonderingState> WonderingState::Instance() 
	{
		static shared_ptr<WonderingState> instance(new WonderingState);
		return instance;
	}
	void WonderingState::Enter(const shared_ptr<Enemy>& Obj) 
	{
		//
	}
	void WonderingState::Execute(const shared_ptr<Enemy>& Obj) 
	{
		// 徘徊中の毎フレーム処理
		Obj->Wondering();
	}
	void WonderingState::Exit(const shared_ptr<Enemy>& Obj) 
	{
		// wonderingから抜けるときの処理

	}

	// EnemyがPlayerを追跡中ステート
	shared_ptr<ChasingState> ChasingState::Instance() 
	{
		static shared_ptr<ChasingState> instance(new ChasingState);
		return instance;
	}
	void ChasingState::Enter(const shared_ptr<Enemy>& Obj) 
	{
		
	}
	void ChasingState::Execute(const shared_ptr<Enemy>& Obj) 
	{
		// 追跡中のマイフレーム処理
		Obj->Chasing();
	}
	void ChasingState::Exit(const shared_ptr<Enemy>& Obj) 
	{
		
	}


	void Enemy::OnCreate()
	{
		InitTransComp();
		InitDrawComp();

		AddTag(L"Enemy");

		auto col = AddComponent<CollisionObb>();

		m_stateMachine.reset(new StateMachine<Enemy>(GetThis<Enemy>())); // ステートマシンの初期化
		m_stateMachine->ChangeState(WonderingState::Instance()); //最初に徘徊中のステートを設定

		m_player = GetStage()->GetSharedGameObject<Player>(L"Player");

		damage = 2; //ダメージ値

		timer = 0.0f;

		isHit = false;
		BrokenObjs::OnCreate();
		BrokenObjs::SetHP(hp); //hpを設定
	}

	void Enemy::InitTransComp()
	{
		auto stage = GetStage();

		m_transComp = GetComponent<Transform>();
		m_transComp->SetPosition(Vec3(-4, 2, 3));
		m_transComp->SetScale(Vec3(1, 1, 1));
		m_transComp->SetRotation(Vec3(0));


	}

	void Enemy::InitDrawComp()
	{
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
		m_drawComp->SetDiffuse(Col4(0, 1, 1, 0));

	}

	void Enemy::OnUpdate()
	{
		if (!isHit)
		{
			auto& app = App::GetApp();
			auto scene = app->GetScene<Scene>();
			float elapsedTime = app->GetElapsedTime();
			wstringstream wss(L"");

			enemyScale = m_transComp->GetScale();
			enemyPos = m_transComp->GetPosition();

			if (auto player = m_player.lock())
			{
				playerScale = player->GetScale();
				playerPos = player->GetPosition();
			}


			Vec3 diff = playerPos - enemyPos;
			float distance = diff.length();

			float followRange = 5.0f; //追尾を開始する距離
			float speed = 1.0f; //速度
			float followSpeed = 1.5f;//追尾時の速度

			////追尾していない場合の移動範囲(-10～10)
			//float rx = -10.0f + (float)rand() / RAND_MAX * 20.0f;
			//float rz = -10.0f + (float)rand() / RAND_MAX * 20.0f;

			//Vec3 dir = Vec3(rx, 0, rz).normalize();

			if (distance < followRange)//一定距離以内
			{
				dir = diff.normalize();

				enemyPos += dir * elapsedTime * followSpeed;
				m_transComp->SetPosition(enemyPos);

			}
			else
			{
				timer += elapsedTime;

				if (timer >= 0.0f)
				{
					SetValue();
					dir = Vec3(rx, 0, rz).normalize();
					timer = -1.0f + (float)rand() / RAND_MAX * -4.0f;
				}

				auto ground = GetStage()->GetSharedGameObject<Ground>(L"Ground");
				Vec3 groundPos = ground->GetComponent<Transform>()->GetPosition();

				if (enemyPos.x < groundPos.x)
				{
					enemyPos += dir * elapsedTime * speed;
					m_transComp->SetPosition(enemyPos);
				}
				else if (enemyPos.z < groundPos.z)
				{
					enemyPos += dir * elapsedTime * speed;
					m_transComp->SetPosition(enemyPos);
				}
			}


			wss << L"EnemyPos.x:" << enemyPos.x
				<< L"\nEnemyPos.y:" << enemyPos.y
				<< L"\nEnemyPos.z" << enemyPos.z
				<< L"\nEnemyLife" << BrokenObjs::GetHP() << endl;
			scene->SetDebugString(wss.str());

			BrokenObjs::OnUpdate();

		}

		diff = playerPos - enemyPos;
		distance = diff.length();

		if (distance < followRange && m_stateMachine->GetCurrentState() != ChasingState::Instance())
		{
			m_stateMachine->ChangeState(ChasingState::Instance()); //player発見時に追跡中のステートに移行
		}
		else if(distance >= followRange && m_stateMachine->GetCurrentState() != WonderingState::Instance())
		{
			m_stateMachine->ChangeState(WonderingState::Instance()); //追跡範囲から出たら徘徊中ステートに戻る
		}

	}

	void Enemy::Wondering()
	{
		float speed = 1.0f; //速度
		timer += elapsedTime;

		if (timer >= 0.0f)
		{
			SetValue();
			dir = Vec3(rx, 0, rz).normalize();
			timer = -1.0f + (float)rand() / RAND_MAX * -4.0f;
		}

		auto ground = GetStage()->GetSharedGameObject<Ground>(L"Ground");
		Vec3 groundPos = ground->GetComponent<Transform>()->GetPosition();

		if (enemyPos.x < groundPos.x)
		{
			enemyPos += dir * elapsedTime * speed;
			m_transComp->SetPosition(enemyPos);
		}
		else if (enemyPos.z < groundPos.z)
		{
			enemyPos += dir * elapsedTime * speed;
			m_transComp->SetPosition(enemyPos);
		}
	}

	void Enemy::Chasing()
	{
		//Vec3 diff = playerPos - enemyPos;
		//float distance = diff.length();

		//float followRange = 5.0f; //追尾を開始する距離
		//float followSpeed = 1.5f;//追尾時の速度

		//if (distance < followRange)//一定距離以内
		//{
			dir = diff.normalize();

			enemyPos += dir * elapsedTime * followSpeed;
			m_transComp->SetPosition(enemyPos);

		//}


	}

	void Enemy::SetValue()
	{
		//追尾していない場合の移動範囲(-10～10)
		rx = -10.0f + (float)rand() / RAND_MAX * 20.0f;
		rz = -10.0f + (float)rand() / RAND_MAX * 20.0f;


	}

	void Enemy::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Attack"))
		{
			isHit = true;
			BrokenObjs::SetDamage(damage); //ダメージを設定
			BrokenObjs::takeDamage();
		}
	}
}