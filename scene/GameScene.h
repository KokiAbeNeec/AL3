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
#include "Enemy.h"
#include "Enemy3.h"
#include "Player.h"
#include "Enemy2.h"
#include "Beam.h"
#include "Line.h"

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

	// タイトル
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;
	// ゲームオーバー
	uint32_t textureHandleGameOver_ = 0;
	Sprite* spriteGameOver_ = nullptr;
	uint32_t textureHandleGameOverFont_ = 0;
	Sprite* spriteGameOverFont_ = nullptr;
	// ゲームクリア
	uint32_t textureHandleGameClear_ = 0;
	Sprite* spriteGameClear_ = nullptr;
	uint32_t textureHandleGameClearFont_ = 0;
	Sprite* spriteGameClearFont_ = nullptr;
	// 決定
	uint32_t textureHandlePush_ = 0;
	Sprite* spritePush_ = nullptr;

	// ビュープロジェクション（共通）
	ViewProjection viewProjection_;

	// ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTransformStage_;

	// 衝突判定
	void Collision();		// 衝突判定
	void CollisionPlayerEnemy2();	//衝突判定（プレイヤーと敵2）
	void CollisionPlayerEnemy3(); //衝突判定（プレイヤーと敵3）
	void CollisionBeamEnemy(); //衝突判定（ビームと敵）
	void CollisionLineEnemy();	// 衝突判定（線と敵）

	// 表示
	int playerLife_ = 3;	// プレイヤーライフ
	int s = 0;

	// シーン
	int sceneMode_ = 0;        // ゲームシーン
	void TitleUpdade();        // タイトル更新
	void GameOverUpdade();     // ゲームオーバー更新
	void GameClearUpdade();     // ゲームオーバー更新

	// 初期化
	void GamePlayStart();

	// サウンド
	uint32_t soundDataHandleTitleBGM_ = 0;    // タイトルBGM
	uint32_t soundDataHandleGamePlayBGM_ = 0; // ゲームプレイBGM
	uint32_t soundDataHandleGameOverBGM_ = 0; // ゲームオーバーBGM
	uint32_t soundDataHandleEnemyHitSE_ = 0;  // 敵ヒットSE
	uint32_t soundDataHandlePushHitSE_ = 0;  // 決定SE
	uint32_t voiceHandleBGM_ = 0;             // 音声再生ハンドル

	Enemy* enemy;
	Enemy3* enemy3;
	Player* player;
	Enemy2* enemy2;
	Beam* beam;
	Line* line;
};