/*!
 @file SkyBox.cpp
 @brief 背景のテクスチャ
 @authors Kato Yuki
 */

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Plane::OnCreate()
	{
		// 頂点データ
		vector<VertexPositionTexture> vertices = {
			{Vec3(-0.5f, +0.5f, 0.0f), Vec2(0.0f, 0.0f)},
			{Vec3(+0.5f, +0.5f, 0.0f), Vec2(1.0f, 0.0f)},
			{Vec3(-0.5f, -0.5f, 0.0f), Vec2(0.0f, 1.0f)},
			{Vec3(+0.5f, -0.5f, 0.0f), Vec2(1.0f, 1.0f)}
		};

		// 頂点インデックス
		vector<uint16_t> indices = {
			0, 1, 2, 2, 1, 3
		};

		// 上記データを適用
		auto drawComp = AddComponent<PTStaticDraw>();
		drawComp->SetDiffuse(Col4(0.5f, 0.5f, 0.5f, 1.0f));
		drawComp->SetOriginalMeshUse(true);
		drawComp->CreateOriginalMesh(vertices, indices);
	}

	void SkyBox::OnCreate()
	{
		map<wstring, Vec3> pairs = {
			{L"000_000_000", Vec3(0   ,  0,   +0.5f)},
			{L"000_090_000", Vec3(+0.5f, 0,    0)},
			{L"000_180_000", Vec3(0   ,  0,   -0.5f)},
			{L"000_270_000", Vec3(-0.5f, 0,    0)},
			{L"090_000_000", Vec3(0   , -0.5f, 0)},
			{L"270_000_000", Vec3(0   , +0.5f, 0)},
		};

		for (const auto& pair : pairs)
		{
			auto plane = ObjectFactory::Create<Plane>(GetStage());

			auto planeDraw = plane->GetComponent<PTStaticDraw>();
			auto wstr = pair.first.substr(0, 3);
			if (wstr == L"000")
			{
				planeDraw->SetTextureResource(L"SkyBoxHorizontal");
			}
			else if (wstr == L"090")
			{
				planeDraw->SetTextureResource(L"SkyBoxBelow");
			}
			else if (wstr == L"270")
			{
				planeDraw->SetTextureResource(L"SkyBoxTop");
			}

			planeDraw->SetDepthStencilState(DepthStencilState::None);
			planeDraw->SetSamplerState(SamplerState::LinearClamp);

			auto planeTrans = plane->GetComponent<Transform>();

			// テクスチャキーの文字列を角度に変換する
			float rotX = XMConvertToRadians(stof(pair.first.substr(0, 3)));
			float rotY = XMConvertToRadians(stof(pair.first.substr(4, 3)));
			float rotZ = XMConvertToRadians(stof(pair.first.substr(8, 3)));

			const float scale = 707.0f;
			planeTrans->SetScale(Vec3(scale));
			planeTrans->SetRotation(rotX, rotY, rotZ);
			planeTrans->SetPosition(pair.second * scale);
			planeTrans->SetParent(GetThis<GameObject>());

			m_Planes.push_back(plane);
		}
	}

	void SkyBox::OnUpdate()
	{
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(GetStage()->GetView()->GetTargetCamera()->GetEye());
	}

	void SkyBox::OnDraw()
	{
		for (auto& plane : m_Planes)
		{
			plane->OnDraw();
		}
	}
}
//end basecross
