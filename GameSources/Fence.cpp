#include "stdafx.h"
#include "Project.h"


namespace basecross
{
	void Fence::OnCreate()
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

	void Fence::OnUpdate()
	{
		StageObject::OnUpdate();

	}

	void Fence::SetCollar(Col4 collar)
	{
		m_drawComp->SetDiffuse(collar);
	}

	void Fence::SetModel(const wstring& modelKey)
	{
		m_drawComp->SetMeshResource(modelKey);
	}
}
