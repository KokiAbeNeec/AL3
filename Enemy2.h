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

class Enemy2 {
  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Enemy2();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy2();

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

	void Enemy2Born();   // 敵発生
	void Enemy2Update(); // 敵更新
	void Enemy2Move(); // 敵移動

	int GetEnemy2Flag() { return enemy2Flag_; }
	float GetEnemy2X() { return worldTransformEnemy2_.translation_.x; }
	float GetEnemy2Z() { return worldTransformEnemy2_.translation_.z; }
	void Hit() { enemy2Flag_ = 0; }

  private:
	// 敵
	uint32_t textureHandleEnemy2_ = 0;
	Model* modelEnemy2_ = 0;
	WorldTransform worldTransformEnemy2_;

	float enemy2Speed_ = 0;	// 移動速度
	int enemy2Flag_ = 0; // 存在フラグ
};