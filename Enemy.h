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

class Enemy {
  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Enemy();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

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

	void EnemyBorn(); // 敵発生
	void EnemyUpdate(); // 敵更新
	void EnemyMove();   // 敵移動

	int GetEnemyFlag() { return enemyFlag_; }
	float GetEnemyX() { return worldTransformEnemy_.translation_.x; }
	float GetEnemyZ() { return worldTransformEnemy_.translation_.z; }
	void Hit() { enemyFlag_ = 0; }

	private:
	// 敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = 0;
	WorldTransform worldTransformEnemy_;

	float enemySpeed_ = 0;	// 移動速度
	int enemyFlag_ = 0; // 存在フラグ
};