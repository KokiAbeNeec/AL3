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

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// BG（スプライト）
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;

	// ビュープロジェクション（共通）
	ViewProjection viewProjection_;

	// ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTransformStage_;

	// プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;
	void PlayerUpdate();	// プレイヤー更新

	// ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = {};
	WorldTransform worldTransformBeam_[10];
	void BeamUpdate();		// ビーム更新
	void BeamMove();		// ビーム移動
	void BeamBorn();		// ビーム発生
	int beamFlag_[10] = {}; // 存在フラグ
	int beamTimer_ = 0;		// ビーム発射タイマー

	// 敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = 0;
	WorldTransform worldTransformEnemy_[10];
	void EnemyUpdate();		// 敵更新
	void EnemyMove();		// 敵移動
	int enemyFlag_[10] = {}; // 存在フラグ
	void EnemyBorn();		// 敵発生
	float enemySpeed_[10] = {};	// 敵のスピード

	// 衝突判定
	void Collision();		// 衝突判定
	void CollisionPlayerEnemy();	//衝突判定（プレイヤーと敵）
	void CollisionBeamEnemy(); //衝突判定（ビームと敵）

	// 表示
	int gameScore_ = 0;		// ゲームスコア
	int playerLife_ = 1;	// プレイヤーライフ

	void GamePlayUpdate();		// ゲームプレイ更新
	void GamePlayDraw3D();		// ゲームプレイ3D表示
	void GamePlayDraw2DBack();  // ゲームプレイ背景2D表示
	void GamePlayDraw2DNear();	// ゲームプレイ近景2D表示

	int sceneMode_ = 1;		// シーンモード（0:ゲームプレイ 1:タイトル 2:ゲームオーバー）

	void TitleUpdade();		// タイトル更新
	void TitleDraw2DNear();	// タイトル2D

	// タイトル（スプライト）
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;

	// エンター（スプライト）
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;

	int gameTimer_ = 0;

	void GameOverUpdade();		// ゲームオーバー更新
	void GameOverDraw2DNear();	// ゲームオーバー2D

	// ゲームオーバー（スプライト）
	uint32_t textureHandleGameOver_ = 0;
	Sprite* spriteGameOver_ = nullptr;

	// 初期化
	void GamePlayStart();
};