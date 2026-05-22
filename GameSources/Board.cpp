#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Board::OnCreate()
	{
		StageObject::OnCreate();
		m_stage = GetStage();
		//auto grv = AddComponent<Gravity>();

		Mat4x4 spanMat;

		spanMat.affineTransformation(
			Vec3(0.25f, 2.0f, 1.0f), // scale
			Vec3(0.0f, 0.0f, 0.0f), // rotation
			Vec3(0.0f, XMConvertToRadians(90), 0.0f), // rotation
			Vec3(0.0f, -0.5f, 0.0f)  // position
		);

		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshResource(L"MODEL_BOARD");
		m_drawComp->SetMeshToTransformMatrix(spanMat);

	}

	void Board::OnUpdate()
	{
		StageObject::OnUpdate();

	}

	void Board::SetCollar(Col4 collar)
	{
		m_drawComp->SetDiffuse(collar);
	}


}