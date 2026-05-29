#include "stdafx.h"
#include "Project.h"


namespace basecross
{
	void Block::OnCreate()
	{
		StageObject::OnCreate();
		m_stage = GetStage();
		//auto grv = AddComponent<Gravity>();

		Mat4x4 spanMat;

		spanMat.affineTransformation(
			Vec3(0.5f, 0.5f, 0.5), // scale
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
}
