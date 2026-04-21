/*!
@file Foo.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class StageManager
	{
	private:
		struct StageManagerDeleter
		{
			void operator()(StageManager* p) { delete p; }
		};
		static unique_ptr<StageManager, StageManagerDeleter> m_StageManager;

		StageManager();
		virtual ~StageManager(){}

		shared_ptr<Stage> m_stage;
	public:
		/*!
		@brief ステージマネージャーを取得
		@return ステージマネージャー
		*/
		static unique_ptr<StageManager, StageManagerDeleter>& CreateStageManager();

		/*!
		@brief ステージマネージャーを取得
		@return ステージマネージャー
		*/
		static unique_ptr<StageManager, StageManagerDeleter>& GetStageManager();

		/*!
		@brief ステージマネージャーを削除
		@return なし
		*/
		static void DeleteStageManager();

		/*!
		@brief ステージマネージャーを初期化
		@return なし
		*/
		void Initialize();

		/*!
		@brief ステージマネージャーを更新
		@return なし
		*/
		void Update();

		/*!
		@brief ステージマネージャーを破棄
		@return なし
		*/
		void Discard();

		/*!
		@brief CSVファイルからステージデータを読み込む
		@param[in] filePath読み込むCSVファイルのパス
		@return なし
		*/
		void LoadStageFromCSV(const std::wstring& filePath);


	private:
		/*!
		@brief CSVの1行を解析する
		@param[in] lineCSVの1行分の文字列
		@return なし
		*/
		void ParseCSVLine(const std::string& line);

		/*!
		@brief CSVの情報からステージオブジェクトを生成する
		@param[in] name オブジェクト名
		@param[in] pos 生成位置（Vec3）
		@return なし
		*/
		void CreateObjectFromCSV(const std::wstring& name, const Vec3& pos, const Vec3& rot, const Vec3& scale);

		//コピー禁止
		StageManager(const StageManager&) = delete;
		StageManager& operator=(const StageManager&) = delete;
		//ムーブ禁止
		StageManager(const StageManager&&) = delete;
		StageManager& operator=(const StageManager&&) = delete;
	};

}
//end basecross
