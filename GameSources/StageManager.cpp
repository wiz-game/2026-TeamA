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
		auto& app = App::GetApp();
		wstring path = app->GetDataDirWString();
		wstring levelPath = path + L"Levels/";
		wstring wstrPath = levelPath + filePath;

		// wstring → string に変換
		std::string strPath(wstrPath.begin(), wstrPath.end());

		try
		{
			ifstream file(strPath);
			if (!file.is_open())
			{
				throw BaseException(
					L"CSVファイルが開けませんでした",
					L"ifstream file(strPath); if (!file.is_open())",
					L"StageManager::LoadStageFromCSV()"
				);
			}

			string line;

			// ヘッダーをスキップ
			getline(file, line);

			// 本体を読む
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
			float px, py, pz;
			float rx, ry, rz;
			float sx, sy, sz;

			if (getline(ss, name, ',') &&
				ss >> px && ss.get() &&
				ss >> py && ss.get() &&
				ss >> pz && ss.get() &&
				ss >> rx && ss.get() &&
				ss >> ry && ss.get() &&
				ss >> rz && ss.get() &&
				ss >> sx && ss.get() &&
				ss >> sy && ss.get() &&
				ss >> sz)
			{
				CreateObjectFromCSV(wstring(name.begin(), name.end()), Vec3(px, py, pz), Vec3(XMConvertToRadians(rx), XMConvertToRadians(ry), XMConvertToRadians(rz)), Vec3(sx, sy, sz));
			}
			else
			{
				throw BaseException(
					L"CSVの行の形式が不正です",
					L"ParseCSVLine のパース失敗",
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
	void StageManager::CreateObjectFromCSV(const std::wstring& name, const Vec3& pos, const Vec3& rot, const Vec3& scale)
	{
		try
		{
			auto scene = App::GetApp()->GetScene<Scene>();
			if (!scene)
			{
				throw BaseException(
					L"Scene が取得できません",
					L"auto scene = App::GetApp()->GetScene<Scene>()",
					L"StageManager::CreateObjectFromCSV()"
				);
			}

			auto stage = scene->GetActiveTypeStage<GameStage>();
			if (!stage)
			{
				throw BaseException(
					L"アクティブなステージがありません",
					L"scene->GetActiveTypeStage<GameStage>()",
					L"StageManager::CreateObjectFromCSV()"
				);
			}

			// --- オブジェクト生成 ---
			if (name == L"Wall")
			{
				auto wall = stage->AddGameObject<Wall>();
				wall->SetPosition(pos);
				wall->SetRotation(rot);
				wall->SetScale(scale);
				return;
			}
			//else if (name == L"Goal")
			//{
			//	auto goal = stage->AddGameObject<Goal>();
			//	goal->SetPosition(pos);
			//	return;
			//}
			else if (name == L" Floor")
			{
				auto floor = stage->AddGameObject<Wall>();
				floor->SetPosition(pos);
				floor->SetRotation(rot);
				floor->SetScale(scale);	
				return;
			}
			else if (name == L"GameObject")
			{
				return;
			}
			else
			{
				throw BaseException(
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
