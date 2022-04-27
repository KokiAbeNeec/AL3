﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <time.h>

using namespace DirectX;

// コンストラクタ
GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() { 
	delete spriteBG_;
	delete modelStage_;
	delete modelPlayer_;
	delete modelBeam_;
	delete modelEnemy_;
}

// 初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// BG（スプライト）
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	// ビュープロジェクションの初期化
	viewProjection_.eye = {0, 1, -6};
	viewProjection_.target = {0, 1, 0};
	viewProjection_.Initialize();

	// ステージ
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};
	worldTransformStage_.Initialize();

	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	// ビーム
	textureHandleBeam_ = TextureManager::Load("Beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformBeam_.Initialize();

	// 敵
	textureHandleEnemy_ = TextureManager::Load("Enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.Initialize();

	// 乱数の初期化
	srand(time(NULL));
}

// プレイヤー更新
void GameScene::PlayerUpdate() {
	// 移動
	// 右へ移動と右の移動範囲制限
	if (input_->PushKey(DIK_RIGHT) && worldTransformPlayer_.translation_.x <= 4.0f) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}
	// 左へ移動と左の移動範囲制限
	if (input_->PushKey(DIK_LEFT) && worldTransformPlayer_.translation_.x >= -4.0f) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}

	// 行列更新
	worldTransformPlayer_.UpdateMatrix();
}

// ビーム更新
void GameScene::BeamUpdate() {
	BeamMove();		// 移動

	// 行列更新
	worldTransformBeam_.UpdateMatrix();

	BeamBorn();		// 発生
}

// ビーム移動
void GameScene::BeamMove() {
	if (beamFlag_ == 1) {
		worldTransformBeam_.translation_.z += 0.1f;	// 移動
		worldTransformBeam_.rotation_.x += 0.1f;	// 回転
	}
	else {
		worldTransformBeam_.translation_.z = worldTransformPlayer_.translation_.z;
		worldTransformBeam_.translation_.x = worldTransformPlayer_.translation_.x;
	}
}

// ビーム発射（発生）
void GameScene::BeamBorn() {
	// ビーム発生
	if (input_->PushKey(DIK_SPACE) && beamFlag_ == 0) {
		worldTransformBeam_.translation_.z = worldTransformPlayer_.translation_.z;
		worldTransformBeam_.translation_.x = worldTransformPlayer_.translation_.x;
		beamFlag_ = 1;
	}
	// ビーム消滅
	if (worldTransformBeam_.translation_.z>=40) {
		beamFlag_ = 0;
	}
}

void GameScene::EnemyUpdate() {
	EnemyMove();	// 移動

	// 行列更新
	worldTransformEnemy_.UpdateMatrix();

	EnemyBorn();	// 発生
}

// 敵移動
void GameScene::EnemyMove() {
	if (enemyFlag_ == 1) {
		worldTransformEnemy_.translation_.z -= 0.1f; // 移動
		worldTransformEnemy_.rotation_.x -= 0.1f;    // 回転
	}

	// 敵消滅
	if (worldTransformEnemy_.translation_.z <= -5) {
		enemyFlag_ = 0;
	}
}

// 敵発生
void GameScene::EnemyBorn() {
	if (enemyFlag_ == 0) {
		int x = rand() % 80;
		float x2 = (float)x / 10 - 4;
		worldTransformEnemy_.translation_.x = x2;
		worldTransformEnemy_.translation_.z = 40;
		enemyFlag_ = 1;
	}
}

// 衝突判定
void GameScene::Collision() { 
	// 衝突判定（プレイヤーと敵）
	CollisionPlayerEnemy();
	// 衝突判定（ビームと敵）
	CollisionBeamEnemy();
}

// 衝突判定（プレイヤーと敵）
void GameScene::CollisionPlayerEnemy() {
	// 敵が存在すれば
	if (enemyFlag_ == 1) {
		// 差を求める
		float dx = abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_.translation_.z);
		// 衝突したら
		if (dx < 1 && dz < 1) {
			// 存在しない
			enemyFlag_ = 0;
			playerLife_--;
		}
	}
}

// 衝突判定（ビームと敵）
void GameScene::CollisionBeamEnemy() {
	// 敵が存在すれば
	if (enemyFlag_ == 1) {
		// 差を求める
		float dx = abs(worldTransformBeam_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformBeam_.translation_.z - worldTransformEnemy_.translation_.z);
		// 衝突したら
		if (dx < 1 && dz < 1) {
			// 存在しない
			enemyFlag_ = 0;
			beamFlag_ = 0;
			gameScore_++;
		}
	}
}

// 更新
void GameScene::Update() {
	PlayerUpdate();
	BeamUpdate();
	EnemyUpdate();
	Collision();
}

// 表示
void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	// 背景
	spriteBG_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
	if (beamFlag_ == 1) {
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	}
	if (enemyFlag_ == 1) {
		modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	// ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	// プレイヤーライフ
	sprintf_s(str, "LIFE %d", playerLife_);
	debugText_->Print(str, 900, 10, 2);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}