#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
	//オプション
	enum class OptionItem
	{
		Return,
		Restart,
		StageSelect,
		SoundTest,
		Title,
		Count //OptionItem数用
	};
	class Option : public GameObject
	{
		OptionItem m_optionItem;
		int itemCount;
		vector<shared_ptr<GameObject>> m_pauseUI;
		Vec2 m_prevLStick;

		void CreateUI();
		bool m_isActive;
		shared_ptr<GameObject> m_cursol; //カーソル用ポインタ
		Vec3 cursolpositon; //カーソル位置更新用変数

	public:
		Option(const shared_ptr<Stage>& stage);
		~Option();

		void SetVisible(bool visible);

		bool isActive() const 
		{
			return m_isActive; 
		}

		void OnCreate()override;
		void OnUpdate()override;

	};

	//サウンドテスト
	enum class SoundTestItem
	{
		BGM,
		SE
	};

	class SoundTest : public GameObject
	{
		SoundTestItem m_soundTestItem;
		shared_ptr<Option> option;
		shared_ptr<GameObject> m_cursol;
		Vec3 cursolPosition;
		float m_newBGMVolume;
		float m_newSEVolume;
		Vec2 m_prevLStick;
		void CreateUI();

		vector<shared_ptr<GameObject>> m_soundTestUI;
	public:
		SoundTest(const shared_ptr<Stage>& stage);
		~SoundTest();

		void OnCreate() override;
		void OnUpdate() override;

	};
}