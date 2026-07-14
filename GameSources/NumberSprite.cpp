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
		float texW = 102.4f / 1024.0f; //テクスチャの幅＝1024、1つの数字の幅＝102.4
		float texH = 256.0f / 256.0f;
		float left = texW * m_number;  //テクスチャのどの位置から切り取るか
		float right = left + texW;
		float top = 0.0f;
		float bottom = top + texH;
		//頂点データを設定

		m_vertices =
		{
			{Vec3(0,0,0),m_color,Vec2(left,top)}, //左上
			{Vec3(102.4f,0,0),m_color,Vec2(right,top)}, //右上
			{Vec3(0,-256,0),m_color,Vec2(left,bottom)}, //左下
			{Vec3(102.4f,-256,0),m_color,Vec2(right,bottom)}, //右下
		};

		//インデックスデータを設定
		indices = 
		{
			0,1,2,
			2,1,3
		};

		m_drawComp = AddComponent<PCTSpriteDraw>(m_vertices, indices);
		//m_drawComp->SetTextureResource(L"TEX_NUMBER");
		m_drawComp->SetSamplerState(SamplerState::LinearWrap);//テクスチャを繰り返すように張り付ける設定
		//SetDrawLayer();
		SetAlphaActive(true);	//透過処理を有効にする
		//m_drawComp->SetBlendState(BlendState::Additive);

		m_transComp = GetComponent<Transform>();
		m_transComp->SetPosition(0, 0, 0);//画面の中心を原点として表示
		m_transComp->SetRotation(0, 0, 0);
		//m_transComp->SetScale(Vec3(0.4f));
	}

	void NumberSprite::OnUpdate()
	{
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
		float right = left + texW;

		float gap = 0.00f; //隣の数字が映らないための隙間
		float fixLeft = left + gap;
		float fixRight = right - gap;

		m_vertices[0].textureCoordinate.x = (fixLeft);
		m_vertices[1].textureCoordinate.x = (fixRight);
		m_vertices[2].textureCoordinate.x = (fixLeft);
		m_vertices[3].textureCoordinate.x = (fixRight);

		m_drawComp->UpdateVertices(m_vertices);
	}
}
//end basecross
