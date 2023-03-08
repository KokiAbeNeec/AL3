#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

class Player {
  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	void PlayerUpdate(); // プレイヤー更新

	float GetPlayerX() { return worldTransformPlayer_.translation_.x; }
	float GetPlayerZ() { return worldTransformPlayer_.translation_.z; }
	float PlayerResetX() { return worldTransformPlayer_.translation_.x = 0; }
	float PlayerResetZ() { return worldTransformPlayer_.translation_.z = -1.0f; }

	private:
	Input* input_ = nullptr;
	// プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;
};