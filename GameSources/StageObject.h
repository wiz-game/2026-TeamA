/*!
@file Foo.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	//　ステージオブジェクトの基底クラス
	class StageObject : public GameObject
	{
	protected:
		shared_ptr<Transform> m_transComp;
		shared_ptr<Stage> m_stage;

		Vec3 m_position; // 位置
		Vec3 m_rotation; // 回転
		Vec3 m_scale;    // スケーリング

	public:
		StageObject(const std::shared_ptr<Stage>& stage);
		virtual ~StageObject() {}

		void OnCreate()override;
		void OnUpdate()override;

		virtual void SetPosition(const Vec3& pos);
		virtual void SetRotation(const Vec3& rot);
		virtual void SetScale(const Vec3& scale);

	};

	// ゴールクラス
	class Goal : public StageObject
	{
		shared_ptr<PNTStaticDraw> m_drawComp;
		void OnCollisionEnter(shared_ptr<GameObject>& other)override;

	public:

		Goal(const shared_ptr<Stage>& stage) :
			StageObject(stage)
		{
		}
		void OnCreate()override;
		void OnUpdate()override;
	};


	// ブロッククラス
	class Block : public StageObject
	{
		shared_ptr<PNTStaticDraw> m_drawComp;

		//void OnCollisionEnter(shared_ptr<GameObject>& other)override;

	public:
		Block(const shared_ptr<Stage>& stage) :
			StageObject(stage)
		{

		}

		void OnCreate()override;
		void OnUpdate()override;

		void SetCollar(Col4 collar);

		void SetModel(const wstring& modelKey);

	};

	// スロープクラス
	class Slope : public StageObject
	{
		shared_ptr<PNTStaticDraw> m_drawComp;
		//void OnCollisionEnter(shared_ptr<GameObject>& other)override;
	public:
		Slope(const shared_ptr<Stage>& stage) :
			StageObject(stage)
		{

		}

		void OnCreate()override;
		void OnUpdate()override;

		void SetCollar(Col4 collar);
	};

	class SlopeCollisionObject : public StageObject
	{

		public:
			SlopeCollisionObject(const shared_ptr<Stage>& stage) :
			StageObject(stage)
		{
		}

		void OnCreate()override;
		void OnUpdate()override;
	};


	// 足場クラス
	class Foothold : public StageObject
	{
		shared_ptr<PNTStaticDraw> m_drawComp;
		//void OnCollisionEnter(shared_ptr<GameObject>& other)override;
	public:
		Foothold(const shared_ptr<Stage>& stage) :
			StageObject(stage)
		{
		}

		void OnCreate()override;
		void OnUpdate()override;

		void SetCollar(Col4 collar);

	};

	// 板クラス
	class Board : public StageObject
	{
		shared_ptr<PNTStaticDraw> m_drawComp;
		//void OnCollisionEnter(shared_ptr<GameObject>& other)override;
	public:
		Board(const shared_ptr<Stage>& stage) :
			StageObject(stage)
		{
		}

		void OnCreate()override;
		void OnUpdate()override;
	};


	// 木クラス
	class FirTree : public StageObject
	{
		shared_ptr<PNTStaticDraw> m_drawComp;
	public:
		FirTree(const shared_ptr<Stage>& stage) :
			StageObject(stage)
		{
		}

		void OnCreate()override;

		void OnUpdate()override;

		void SetCollar(Col4 collar);
	};

	// 倒木クラス
	class FallenTree : public StageObject
	{
		shared_ptr<PNTStaticDraw> m_drawComp;

	public:
		FallenTree(const shared_ptr<Stage>& stage) :
			StageObject(stage)
		{
		}

		void OnCreate()override;

		void OnUpdate()override;

		void SetCollar(Col4 collar);
	};

	// 石クラス
	class Stone : public StageObject
	{
		shared_ptr<PNTStaticDraw> m_drawComp;
	public:
		Stone(const shared_ptr<Stage>& stage) :
			StageObject(stage)
		{
		}
		void OnCreate()override;
		void OnUpdate()override;
		void SetCollar(Col4 collar);
	};

	// キノコクラス
	class Mushroom : public StageObject
	{
		shared_ptr<PNTStaticDraw> m_drawComp;
	public:
		Mushroom(const shared_ptr<Stage>& stage) :
			StageObject(stage)
		{
		}
		void OnCreate()override;
		void OnUpdate()override;
		void SetCollar(Col4 collar);

		void SetModel(const wstring& modelKey);
		void SetModelTexture(const wstring& textureKey);
		void SetModelMatrix(const Mat4x4& matrix);
	};

}
//end basecross
