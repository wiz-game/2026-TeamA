/*!
@file Player.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross {
	// GameObjectクラスを継承した「Player」クラスを定義
	class Player : public GameObject // GameObjectクラスの継承【必須】
	{
		std::shared_ptr<Transform> m_transform; // トランスフォームはよく使うのでメンバにしておく
		//std::shared_ptr<PNTDXModelDraw> m_draw; // ドローコンポーネント
		std::shared_ptr<PNTStaticDraw> m_draw; // ドローコンポーネント

		Vec3 m_position; // プレイヤーの位置
		Vec3 m_rotation; // プレイヤーの回転
		Vec3 m_scale;    // プレイヤーのスケーリング
	public :
		// ステージを引数にしたコンストラクタ【必須】
		Player(const std::shared_ptr<Stage>& stage) :
			GameObject(stage), // ステージをGameObjectに渡す【必須】
			m_position(0.0f, 0.0f, 0.0f), // プレイヤーの初期位置を設定
			m_rotation(0.0f, 0.0f, 0.0f), // プレイヤーの初期回転を設定
			m_scale(1.0f)     // プレイヤーの初期スケーリングを設定
		{
		}
		Player(const std::shared_ptr<Stage>& stage,Vec3 scale) :
			GameObject(stage), // ステージをGameObjectに渡す【必須】
			m_position(0.0f, 0.0f, 0.0f), // プレイヤーの初期位置を設定
			m_rotation(0.0f, 0.0f, 0.0f), // プレイヤーの初期回転を設定
			m_scale(scale)     // プレイヤーの初期スケーリングを設定
		{
		}

		void OnCreate() override; // 初期設定用の関数(UnityのStartメソッドに相当)
		void OnUpdate() override; // 毎フレーム実行される関数(UnityのUpdateメソッドに相当)

		Vec3 GetPosition()
		{
			return m_position;
		}
		void SetPosition(Vec3 pos)
		{
			m_position = pos;
		}

		Vec3 GetRotation()
		{
			return m_rotation;
		}
		void SetRotation(Vec3 rot)
		{
			m_rotation = rot;
		}

		Vec3 GetScale()
		{
			return m_scale;
		}
		void SetScale(Vec3 scale)
		{
			m_scale = scale;
		}
	};
}
//end basecross

