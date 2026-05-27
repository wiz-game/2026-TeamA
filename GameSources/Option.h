#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross
{
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

	class SoundTest : public GameObject
	{
		float m_BGMVolume;
		float m_SEVolume;
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