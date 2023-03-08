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

class Enemy3 {
  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Enemy3();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy3();

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

	void Enemy3Born();   // 敵発生
	void Enemy3Update(); // 敵更新
	void Enemy3Move();   // 敵移動

	int GetEnemy3Flag() { return enemy3Flag_; }
	float GetEnemy3X() { return worldTransformEnemy3_.translation_.x; }
	float GetEnemy3Z() { return worldTransformEnemy3_.translation_.z; }
	void Hit() { enemy3Flag_ = 0; }

  private:
	// 敵
	uint32_t textureHandleEnemy3_ = 0;
	Model* modelEnemy3_ = 0;
	WorldTransform worldTransformEnemy3_;

	float enemy3Speed_ = 0;
	int enemy3Flag_ = 0; // 存在フラグ
};
