/*!
@file Foo.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{

	//初期化
	void NumberSprite::OnCreate()
	{
		float texW = 102.4f / 1024.0f;
		float texH = 256.0f / 256.0f;
		float left = texW * m_number;
		float right = left + texW;
		float top = 10.0f;
		float bottom = top + texH;
		//頂点データを設定

		m_vertices =
		{
			{Vec3(0,0,0),m_color,Vec2(left,top)},//①
			{Vec3(102.4f,0,0),m_color,Vec2(right,top)},//②
			{Vec3(0,-256,0),m_color,Vec2(left,bottom)},//③
			{Vec3(102.4f,-256,0),m_color,Vec2(right,bottom)},//④
		};

		//インデックスデータを設定
		indices = {
			0,1,2,
			2,1,3
		};

		m_drawComp = AddComponent<PCTSpriteDraw>(m_vertices, indices);
		m_drawComp->SetTextureResource(L"TEX_NUMBER");
		m_drawComp->SetSamplerState(SamplerState::LinearWrap);//テクスチャを繰り返すように張り付ける設定
		//SetDrawLayer();
		SetAlphaActive(true);	//透過処理を有効にする
		//m_drawComp->SetBlendState(BlendState::Additive);

		m_transComp = GetComponent<Transform>();
		m_transComp->SetPosition(0, 0, 0);//画面の中心を原点として表示
		m_transComp->SetRotation(0, 0, 0);
		m_transComp->SetScale(Vec3(1));
	}
	void NumberSprite::OnUpdate()
	{
		auto& app = App::GetApp();
		float delta = app->GetElapsedTime();
		auto scene = app->GetScene<Scene>();

		wstring log = scene->GetDebugString();
		wstringstream wss;
		wss << log;
		//wss << L"生成されました" << L"\n";

		scene->SetDebugString(wss.str());

		//数値を更新する
		//SetNumber(9);
	}

	void NumberSprite::SetNumber(int num) // 0 - 9
	{
		//エラーチェック（条件が失敗したらプログラムを止める）
		assert(num >= 0 && num <= 9);

		//管理している数値を変更する
		m_number = num;

		//新しい数値に合わせて頂点データを更新する
		float texW = 102.4f / 1024.0f;
		float left = texW * m_number;//引数で指定した数字の左側を表す座標


		m_vertices[0].textureCoordinate.x = (left);
		m_vertices[1].textureCoordinate.x = (left + texW);
		m_vertices[2].textureCoordinate.x = (left);
		m_vertices[3].textureCoordinate.x = (left + texW);

		m_drawComp->UpdateVertices(m_vertices);
	}
}
//end basecross
