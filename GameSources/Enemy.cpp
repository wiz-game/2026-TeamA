#include "stdafx.h"
#include "Project.h"

namespace basecross
{

	void Enemy::OnCreate()
	{
		InitTransComp();
		InitDrawComp();

		AddTag(L"Enemy");

		auto col = AddComponent<CollisionObb>();

		m_player = GetStage()->GetSharedGameObject<Player>(L"Player");

		damage = 2;

		timer = 0.0f;

		isHit = false;
		BrokenObjs::OnCreate();
		BrokenObjs::SetHP(hp);
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
	}

	void Enemy::SetValue()
	{
		//追尾していない場合の移動範囲(-10～10)
		rx = -10.0f + (float)rand() / RAND_MAX * 20.0f;
		rz = -10.0f + (float)rand() / RAND_MAX * 20.0f;


	}

	void Enemy::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Player"))
		{
			isHit = true;
			BrokenObjs::SetDamage(damage);
			BrokenObjs::takeDamage();
		}
	}
}