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
#include "Player.h"

class Beam {
  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Beam();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Beam();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	void BeamBorn();    // 敵発生
	void BeamUpdate(); // 敵更新
	void BeamMove();    // 敵移動

	float GetBeamX() { return worldTransformBeam_.translation_.x; }
	float GetBeamZ() { return worldTransformBeam_.translation_.z; }
	void Hit() { beamFlag_ = 0; }
	private:
	Input* input_ = nullptr;
	// ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;

	int beamFlag_ = 0; // 存在フラグ
	Player* player_;
};