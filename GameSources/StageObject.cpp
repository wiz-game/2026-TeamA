/*!
@file Foo.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{

	//　基底クラス
	// コンストラクタ
	StageObject::StageObject(const std::shared_ptr<Stage>& stage) :
		GameObject(stage),
		m_position(0.0f, 0.0f, 0.0f),
		m_rotation(0.0f, 0.0f, 0.0f),
		m_scale(1.0f, 1.0f, 1.0f)
	{
	}


	// 初期化
	void StageObject::OnCreate()
	{
		m_transComp = GetComponent<Transform>();

		m_transComp->SetPosition(m_position);
		m_transComp->SetScale(m_scale);
		m_transComp->SetRotation(m_rotation);

		auto collision = AddComponent<CollisionObb>();
		collision->SetFixed(true);
		collision->SetDrawActive(true);

	}

	void StageObject::OnUpdate()
	{

	}

	void StageObject::SetPosition(const Vec3& pos)
	{
		m_position = pos;
		if (m_transComp)
		{
			m_transComp->SetPosition(m_position);
		}
	}
	void StageObject::SetRotation(const Vec3& rot)
	{
		m_rotation = rot;
		if (m_transComp)
		{
			m_transComp->SetRotation(m_rotation);
		}
	}
	void StageObject::SetScale(const Vec3& scale)
	{
		m_scale = scale;
		if (m_transComp)
		{
			m_transComp->SetScale(m_scale);
		}
	}

	// ゴールクラス
	void Goal::OnCreate()
	{
		StageObject::OnCreate();
		m_stage = GetStage();
		//auto grv = AddComponent<Gravity>();
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"DEFAULT_CUBE");
		m_drawComp->SetDiffuse(Col4(1, 0, 1, 1));
	}

	void Goal::OnUpdate()
	{
		StageObject::OnUpdate();

	}

	void Goal::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		auto stage = GetStage();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (other && player)
		{
			auto& app = App::GetApp();
			auto scene = app->GetScene<Scene>();
			wstringstream wss(L"");

			wss << L"GameClear" << endl;
			scene->SetDebugString(wss.str());

			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"ToGameClearStage");//ゲームシーンを移動する

		}
	}


	// ブロッククラス
	void Block::OnCreate()
	{
		StageObject::OnCreate();
		m_stage = GetStage();
		//auto grv = AddComponent<Gravity>();

		Mat4x4 spanMat;

		spanMat.affineTransformation(
			Vec3(0.5f, 0.5f, 0.5f), // scale
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, -0.5f, 0.0f)  // position
		);
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshToTransformMatrix(spanMat);
	}

	void Block::OnUpdate()
	{
		StageObject::OnUpdate();
	}

	void Block::SetCollar(Col4 collar)
	{
		m_drawComp->SetDiffuse(collar);
	}

	void Block::SetModel(const wstring& modelKey)
	{
		m_drawComp->SetMeshResource(modelKey);

	}

	// スロープクラス
	void Slope::OnCreate()
	{
		StageObject::OnCreate();
		m_stage = GetStage();
		//auto grv = AddComponent<Gravity>();

		Mat4x4 spanMat;

		spanMat.affineTransformation(
			Vec3(0.5f, 0.5f, 0.25), // scale
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, XM_PI, 0.0f), // rotation
			Vec3(0.0f, -0.5f, 0.0f)  // position
		);
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"MODEL_SLOPE");
		m_drawComp->SetMeshToTransformMatrix(spanMat);
	}

	void Slope::OnUpdate()
	{
		StageObject::OnUpdate();

	}

	void Slope::SetCollar(Col4 collar)
	{
		m_drawComp->SetDiffuse(collar);
	}

	// 板クラス
	void Board::OnCreate()
	{
		StageObject::OnCreate();
		m_stage = GetStage();
		//auto grv = AddComponent<Gravity>();

		Mat4x4 spanMat;

		spanMat.affineTransformation(
			Vec3(1.0f, 8.0f, 0.5f), // scale
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, XM_PI, 0.0f), // rotation
			Vec3(0.0f, 0.0f, 0.0f)  // position
		);

		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"MODEL_BOARD");
		m_drawComp->SetMeshToTransformMatrix(spanMat);

	}

	void Board::OnUpdate()
	{
		StageObject::OnUpdate();

	}

	// 足場クラス
	void Foothold::OnCreate()
	{
		StageObject::OnCreate();
		m_stage = GetStage();
		//auto grv = AddComponent<Gravity>();
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.75f, 8.0f, 0.5f), // scale
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, -15.5f, 0.0f)  // position
		);
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"MODEL_FOOTHOLD");
		m_drawComp->SetMeshToTransformMatrix(spanMat);
	}





	void Foothold::OnUpdate()
	{
		StageObject::OnUpdate();

	}

	void Foothold::SetCollar(Col4 collar)
	{
		m_drawComp->SetDiffuse(collar);
	}

	// 木クラス
	void FirTree::OnCreate()
	{
		StageObject::OnCreate();
		m_stage = GetStage();
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.35f, 0.4f, 0.35f), // scale
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, -0.5f, 0.0f)  // position
		);
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"MODEL_FIRTREE");
		m_drawComp->SetMeshToTransformMatrix(spanMat);
	}

	void FirTree::OnUpdate()
	{
		StageObject::OnUpdate();
	}

	void FirTree::SetCollar(Col4 collar)
	{
		m_drawComp->SetDiffuse(collar);
	}

	// 倒木クラス
	void FallenTree::OnCreate()
	{
		StageObject::OnCreate();
		m_stage = GetStage();
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.3f, 1.0f, 0.3f), // scale
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, XM_PI, 0.0f), // rotation
			Vec3(0.0f, 0.3f, 0.0f)  // position
		);
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"MODEL_FALLENTREE");
		m_drawComp->SetMeshToTransformMatrix(spanMat);
	}

	void FallenTree::OnUpdate()
	{
		StageObject::OnUpdate();
	}

	void FallenTree::SetCollar(Col4 collar)
	{
		m_drawComp->SetDiffuse(collar);
	}

	// 石クラス
	void Stone::OnCreate()
	{
		StageObject::OnCreate();
		m_stage = GetStage();
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.5f, 1.0f, 0.5f), // scale
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, 0.0f, 0.0f)  // position
		);
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"MODEL_STONE");
		m_drawComp->SetMeshToTransformMatrix(spanMat);
	}

	void Stone::OnUpdate()
	{
		StageObject::OnUpdate();
	}

	void Stone::SetCollar(Col4 collar)
	{
		m_drawComp->SetDiffuse(collar);
	}

	// キノコクラス
	void Mushroom::OnCreate()
	{
		StageObject::OnCreate();
		m_stage = GetStage();
		m_drawComp = AddComponent<PNTStaticDraw>();
	}

	void Mushroom::OnUpdate()
	{
		StageObject::OnUpdate();
	}

	void Mushroom::SetCollar(Col4 collar)
	{
		m_drawComp->SetDiffuse(collar);
	}

	void Mushroom::SetModel(const wstring& modelKey)
	{
		m_drawComp->SetMeshResource(modelKey);
	}

	void Mushroom::SetModelTexture(const wstring& textureKey)
	{
		m_drawComp->SetTextureResource(textureKey);
	}

	void Mushroom::SetModelMatrix(const Mat4x4& matrix)
	{
		m_drawComp->SetMeshToTransformMatrix(matrix);
	}
}

//end basecross
