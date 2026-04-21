/*!
@file Foo.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	unique_ptr<StageManager, StageManager::StageManagerDeleter> StageManager::m_StageManager;

	StageManager::StageManager()
	{
	}

	// シングルトン構築
	unique_ptr<StageManager, StageManager::StageManagerDeleter>& StageManager::CreateStageManager()
	{
		if (!m_StageManager)
		{
			//自分自身の構築
			m_StageManager.reset(new StageManager());
			// 初期化
			m_StageManager->Initialize();
		}
		return m_StageManager;
	}

	// シングルトン取得
	unique_ptr<StageManager, StageManager::StageManagerDeleter>& StageManager::GetStageManager()
	{
		try
		{
			if (m_StageManager.get() == 0)
			{
				throw BaseException
				(
					L"StageManagerがまだ作成されてません",
					L"if (m_StageManager.get() == 0)",
					L"StageManager::GetStageManager()"
				);
			}
			return m_StageManager;
		}
		catch (...)
		{
			throw;
		}
	}

	// シングルトン破棄
	void StageManager::DeleteStageManager()
	{
		if (m_StageManager)
		{
			// 削除処理
			m_StageManager->Discard();
			// 自分自身の破棄
			m_StageManager.reset();
		}
	}

	// 初期化
	void StageManager::Initialize()
	{

	}

	// 更新
	void StageManager::Update()
	{

	}

	// 破棄
	void StageManager::Discard()
	{

	}

	// CSV ファイルからステージデータを読み込む
	void StageManager::LoadStageFromCSV(const std::wstring& filePath)
	{
		try
		{
			ifstream file(filePath);
			if (!file.is_open())
			{
				throw BaseException
				(
					L"CSVファイルが開けませんでした",
					L"ifstream file(filePath); if (!file.is_open())",
					L"StageManager::LoadStageFromCSV()"
				);
			}

			string line;

			// 1行目（ヘッダー）を読み飛ばす
			if (getline(file, line))
			{
				// ここは何もしない（ヘッダー無視）
			}

			// 2行目以降を処理
			while (getline(file, line))
			{
				ParseCSVLine(line);
			}

			file.close();
		}
		catch (...)
		{
			throw;
		}
	}

	// CSVの1行を解析する
	void StageManager::ParseCSVLine(const std::string& line)
	{
		try
		{
			stringstream ss(line);
			string name;
			float x, y, z;
			if (getline(ss, name, ',') && ss >> x && ss.get() && ss >> y && ss.get() && ss >> z)
			{
				CreateObjectFromCSV(wstring(name.begin(), name.end()), Vec3(x, y, z));
			}
			else
			{
				throw BaseException
				(
					L"CSVの行の形式が不正です",
					L"if (getline(ss, name, ',') && ss >> x && ss.get() && ss >> y && ss.get() && ss >> z)",
					L"StageManager::ParseCSVLine()"
				);
			}
		}
		catch (...)
		{
			throw;
		}
	}

	// CSVの情報からステージオブジェクトを生成する
	void StageManager::CreateObjectFromCSV(const std::wstring& name, const Vec3& pos)
	{
		try
		{
			auto scene = App::GetApp()->GetScene<Scene>();
			if (!scene)
			{
				throw BaseException
				(
					L"Scene が取得できません",
					L"auto scene = App::GetApp()->GetScene<Scene>()",
					L"StageManager::CreateObjectFromCSV()"
				);
			}

			// --- オブジェクト生成 ---
			if (name == L"Wall")
			{
				auto wall = scene->AddGameObject<Wall>();
				wall->SetPosition(pos);
			}
			else if (name == L"Goal")
			{
				//auto goal = scene->AddGameObject<Goal>();
				//goal->SetPosition(pos);
			}
			if (name == L"Floor")
			{
				auto floor = scene->AddGameObject<Wall>();
				floor->SetPosition(pos);

			}
			else
			{
				throw BaseException
				(
					L"CSVのオブジェクト名が不正です",
					L"不明なオブジェクト名: " + name,
					L"StageManager::CreateObjectFromCSV()"
				);
			}
		}
		catch (...)
		{
			throw;
		}
	}
}
//end basecross
