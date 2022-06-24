#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <time.h>

using namespace DirectX;

// コンストラクタ
GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() { 
	delete modelStage_;
	delete modelPlayer_;
	delete modelBeam_;
	delete modelEnemy_;
	delete spriteBG_;
	delete spriteTitle_;
	delete spriteEnter_;
	delete spriteGameOver_;
	for (int i = 0; i < 5; i++) {
		delete spriteNumber_[i];		
	}
	delete spriteScore_;
	for (int i = 0; i < 3; i++) {
		delete spriteLife_[i];	
	}
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
	for (int i = 0; i < 20; i++) {
		textureHandleStage_ = TextureManager::Load("stage2.jpg");
		modelStage_ = Model::Create();
		worldTransformStage_[i].translation_ = {0, -1.5f, 2.0f * i - 5};
		worldTransformStage_[i].scale_ = {4.5f, 1, 1};
		worldTransformStage_[i].Initialize();
	}

	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	// ビーム
	for (int i = 0; i < 10; i++) {
		textureHandleBeam_ = TextureManager::Load("Beam.png");
		modelBeam_ = Model::Create();
		worldTransformBeam_[i].scale_ = {0.3f, 0.3f, 0.3f};
		worldTransformBeam_[i].Initialize();
	}

	// 敵
	for (int i = 0; i < 10; i++) {
		textureHandleEnemy_ = TextureManager::Load("Enemy.png");
		modelEnemy_ = Model::Create();
		worldTransformEnemy_[i].scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformEnemy_[i].Initialize();
	}

	// 乱数の初期化
	srand(time(NULL));

	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});

	textureHandleEnter_ = TextureManager::Load("enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {400, 500});

	textureHandleGameOver_ = TextureManager::Load("gameover.png");
	spriteGameOver_ = Sprite::Create(textureHandleGameOver_, {0, 200});

	textureHandleNumber_ = TextureManager::Load("number.png");
	for (int i = 0; i < 5; i++) {
		spriteNumber_[i] = Sprite::Create(textureHandleNumber_, {300.0f + i * 26, 0});
	}

	textureHandleScore_ = TextureManager::Load("score.png");
	spriteScore_ = Sprite::Create(textureHandleScore_, {150, 0});

	textureHandleLife_ = TextureManager::Load("player.png");
	for (int i = 0; i < 3; i++) {
		spriteLife_[i] = Sprite::Create(textureHandleLife_, {900.0f + i * 60, 10});	
	}

	// サウンドデータの読み込み
	soundDataHandleTitleBGM_ = audio_->LoadWave("Audio/Ring05.wav");
	soundDataHandleGamePlayBGM_ = audio_->LoadWave("Audio/Ring08.wav");
	soundDataHandleGameOverBGM_ = audio_->LoadWave("Audio/Ring09.wav");
	soundDataHandleEnemyHitSE_ = audio_->LoadWave("Audio/chord.wav");
	soundDataHandlePlayerHitSE_ = audio_->LoadWave("Audio/tada.wav");

	// タイトルBGMを再生
	voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true);
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

	if (playerTimer_ > 0) {
		playerTimer_--;
	}

	// 行列更新
	worldTransformPlayer_.UpdateMatrix();
}

// ビーム更新
void GameScene::BeamUpdate() {
	BeamMove();		// 移動

	// 行列更新
	for (int i = 0; i < 10; i++) {
		worldTransformBeam_[i].UpdateMatrix();	
	}

	BeamBorn();		// 発生
}

// ビーム移動
void GameScene::BeamMove() {
	for (int i = 0; i < 10; i++) {
		if (beamFlag_[i] == 1) {
			worldTransformBeam_[i].translation_.z += 0.25f; // 移動
			worldTransformBeam_[i].rotation_.x += 0.25f;    // 回転
		}

		else {
			worldTransformBeam_[i].translation_.z = worldTransformPlayer_.translation_.z;
			worldTransformBeam_[i].translation_.x = worldTransformPlayer_.translation_.x;
		}
	}
}

// ビーム発射（発生）
void GameScene::BeamBorn() {
	// ビーム発生
	if (beamTimer_ == 0) {
		for (int i = 0; i < 10; i++) {
			if (input_->PushKey(DIK_SPACE) && beamFlag_[i] == 0) {
				worldTransformBeam_[i].translation_.z = worldTransformPlayer_.translation_.z;
				worldTransformBeam_[i].translation_.x = worldTransformPlayer_.translation_.x;
				beamFlag_[i] = 1;
				beamTimer_ = 1;
				break;
			}
		}
	} else {
		beamTimer_++;
		if (beamTimer_ > 10) {
			beamTimer_ = 0;
		}
	}
	// ビーム消滅
	for (int i = 0; i < 10; i++) {
		if (worldTransformBeam_[i].translation_.z >= 40) {
			beamFlag_[i] = 0;
		}
	}
}

void GameScene::EnemyUpdate() {
	EnemyMove();	// 移動

	// 行列更新
	for (int i = 0; i < 10; i++) {
		worldTransformEnemy_[i].UpdateMatrix();	
	}

	EnemyBorn();	// 発生
	EnemyJump();	// 消滅
}

// 敵移動
void GameScene::EnemyMove() {
	for (int i = 0; i < 10; i++) {
		if (enemyFlag_[i] == 1) {
			worldTransformEnemy_[i].translation_.x += enemySpeed_[i];
			worldTransformEnemy_[i].translation_.z -= 0.15f; // 移動
			worldTransformEnemy_[i].rotation_.x -= 0.25f;    // 回転
		}
		// タイマーにより速度を設定
		worldTransformEnemy_[i].translation_.z -= gameTimer_ / 10000.0f;
	}

	for (int i = 0; i < 10; i++) {
		if (enemyFlag_[i] == 1) {
			if (worldTransformEnemy_[i].translation_.x >= 5) {
				enemySpeed_[i] = -0.2f;
			} else if (worldTransformEnemy_[i].translation_.x <= -5) {
				enemySpeed_[i] = 0.2f;
			}
		}
	}

	// 敵消滅
	for (int i = 0; i < 10; i++) {
		if (worldTransformEnemy_[i].translation_.z <= -5) {
			enemyFlag_[i] = 0;
		}
	}
}

// 敵発生
void GameScene::EnemyBorn() {
	for (int i = 0; i < 10; i++) {
		if (enemyFlag_[i] == 0) {
			if (rand() % 10 == 0) {
				int x = rand() % 80;
				float x2 = (float)x / 10 - 4;
				if (rand() % 2 == 0) {
					enemySpeed_[i] = 0.2f;
				} else {
					enemySpeed_[i] = -0.2f;
				}
				worldTransformEnemy_[i].translation_.x = x2;
				worldTransformEnemy_[i].translation_.y = 0;
				worldTransformEnemy_[i].translation_.z = 40;
				enemyFlag_[i] = 1;
				break;
			}
		}
	}
}

// 敵ジャンプ
void GameScene::EnemyJump() {
	// 敵でループ
	for (int i = 0; i < 10; i++) {
		// 消滅演出ならば
		if (enemyFlag_[i] == 2) {
			// 移動（Y座標に速度を加える）
			worldTransformEnemy_[i].translation_.y += enemyJumpSpeed_[i];
			// 速度を減らす
			enemyJumpSpeed_[i] -= 0.1f;
			// 斜め移動
			worldTransformEnemy_[i].translation_.x += enemySpeed_[i] * 2;
			// 下へ落ちると消滅
			if (worldTransformEnemy_[i].translation_.y < -3) {
				enemyFlag_[i] = 0;	// 存在しない
			}
		}
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
	for (int i = 0; i < 10; i++) {
		// 敵が存在すれば
		if (enemyFlag_[i] == 1) {
			// 差を求める
			float dx = abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_[i].translation_.x);
			float dz = abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_[i].translation_.z);
			// 衝突したら
			if (dx < 1 && dz < 1) {
				enemyJumpSpeed_[i] = 1;
				enemyFlag_[i] = 2;
				playerLife_--;
				playerTimer_ = 60;
				audio_->PlayWave(soundDataHandlePlayerHitSE_);
			}
		}
	}
	if (playerLife_ == 0) {
		sceneMode_ = 2;

		// BGM切り替え
		audio_->StopWave(voiceHandleBGM_);  // BGMを停止
		voiceHandleBGM_ = audio_->PlayWave(soundDataHandleGameOverBGM_, true); // ゲームプレイBGMを再生
	}
}

// 衝突判定（ビームと敵）
void GameScene::CollisionBeamEnemy() {
	// 敵が存在すれば
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			if (enemyFlag_[i] == 1) {
				// 差を求める
				float dx = abs(
				  worldTransformBeam_[k].translation_.x - worldTransformEnemy_[i].translation_.x);
				float dz = abs(
				  worldTransformBeam_[k].translation_.z - worldTransformEnemy_[i].translation_.z);
				// 衝突したら
				if (dx < 1 && dz < 1) {
					enemyJumpSpeed_[i] = 1;
					enemyFlag_[i] = 2;
					beamFlag_[k] = 0;
					gameScore_++;
					audio_->PlayWave(soundDataHandleEnemyHitSE_);
				}
			}
		}
	}
}

// タイトル更新
void GameScene::TitleUpdade() {
	// エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_RETURN)) {
		// モードをゲームプレイへ変更
		sceneMode_ = 0;
		GamePlayStart();

		// BGM切り替え
		audio_->StopWave(voiceHandleBGM_);	// BGMを停止
		voiceHandleBGM_ = audio_->PlayWave(soundDataHandleGamePlayBGM_, true);	// ゲームプレイBGMを再生
	}
}

// タイトル表示
void GameScene::TitleDraw2DNear() {
	spriteTitle_->Draw();
	if (gameTimer_ % 40 >= 20) {
		spriteEnter_->Draw();
	}
 }

// ゲームオーバー更新
void GameScene::GameOverUpdade() {
	 // エンターキーを押した瞬間
	 if (input_->TriggerKey(DIK_RETURN)) {
		// 初期化
		GamePlayStart();
		 // モードをゲームプレイへ変更
		 sceneMode_ = 1;

		 // BGM切り替え
		 audio_->StopWave(voiceHandleBGM_);  // BGMを停止
		 voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true); // ゲームプレイBGMを再生
	 }
 }

// ゲームオーバー表示
void GameScene::GameOverDraw2DNear() {
	 spriteGameOver_->Draw();
	 if (gameTimer_ % 40 >= 20) {
		 spriteEnter_->Draw();
	 }
 }

// スコア数値の表示
void GameScene::DrawScore() {
	spriteScore_->Draw();

	// 各桁の値を取り出す
	char eachNumber[5] = {};			// 各桁の値
	int number = gameScore_;			// 表示する数字

	int keta = 10000;					// 最初の桁

	for (int i = 0; i < 5; i++) {
		eachNumber[i] = number / keta;	// 今の桁の値を求める
		number = number % keta;			// 次の桁以下の値を取り出す
		keta = keta / 10;				// 桁を進める
	}

	// 各桁の数値を描画
	for (int i = 0; i < 5; i++) {
		spriteNumber_[i]->SetSize({32, 64});
		spriteNumber_[i]->SetTextureRect({32.0f * eachNumber[i], 0}, {32, 64});
		spriteNumber_[i]->Draw();
	}

	// ライフを描画
	for (int i = 0; i < playerLife_; i++) {
		spriteLife_[i]->SetSize({40, 40});
		spriteLife_[i]->SetTextureRect({0, 0}, {200, 200});
		spriteLife_[i]->Draw();	
	}
 }

// 初期化
void GameScene::GamePlayStart() {
	 for (int i = 0; i < 10; i++) {
		 enemyFlag_[i] = 0;
		 beamFlag_[i] = 0;
	 }
	 gameTimer_ = 0;
	 playerTimer_ = 0;
	playerLife_ = 3;
	gameScore_ = 0;
 }

// 更新
void GameScene::Update() { 
	// 各シーンの更新処理を呼び出す
	switch (sceneMode_){
	case 0:
		GamePlayUpdate();	// ゲームプレイ更新
		gameTimer_++;
		break;
	case 1:
		TitleUpdade();		// タイトル更新
		gameTimer_++;
		break;
	case 2:
		GameOverUpdade();	// ゲームオーバー更新
		gameTimer_++;
		break;
	}
}

void GameScene::StageUpdate() {
	// 各ステージでループ
	for (int i = 0; i < 20; i++) {
		// 手前に移動
		worldTransformStage_[i].translation_.z -= 0.1f;
		// 端まで来たら奥へ戻る
		if (worldTransformStage_[i].translation_.z < -5) {
			worldTransformStage_[i].translation_.z += 40;
		}
		// 行列更新
		worldTransformStage_[i].UpdateMatrix();
	}
}

  // ゲームプレイ更新
void GameScene::GamePlayUpdate() {
	PlayerUpdate(); // プレイヤー更新
	BeamUpdate();   // 敵更新
	EnemyUpdate();  // ビーム更新
	Collision();    // 衝突判定
	StageUpdate();	// ステージ更新
}

// ゲームプレイ3D表示
void GameScene::GamePlayDraw3D() {
	// ステージ
	for (int i = 0; i < 20; i++) {
		modelStage_->Draw(worldTransformStage_[i], viewProjection_, textureHandleStage_);
	}

	// プレイヤー
	if (playerTimer_ % 4 < 2) {
		modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
	}
	
	// ビーム
	for (int i = 0; i < 10; i++) {
		if (beamFlag_[i] == 1) {
			modelBeam_->Draw(worldTransformBeam_[i], viewProjection_, textureHandleBeam_);
		}
	}

	// 敵
	for (int i = 0; i < 10; i++) {
		if (enemyFlag_[i] == 1 || enemyFlag_[i] == 2) {
			modelEnemy_->Draw(worldTransformEnemy_[i], viewProjection_, textureHandleEnemy_);
		}
	}
}

// ゲームプレイ背景2D表示
void GameScene::GamePlayDraw2DBack() {
	// 背景
	spriteBG_->Draw();
}

// ゲームプレイ近景2D表示
void GameScene::GamePlayDraw2DNear() {
	// スコア表示
	DrawScore();
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
	// 各シーンの2D背景表示を呼び出す
	switch (sceneMode_) {
	case 0:
		GamePlayDraw2DBack();	// ゲームプレイ2D背景表示
		break;
	case 1:
		TitleDraw2DNear();		// タイトル2D表示
		break;
	case 2:
		GamePlayDraw2DBack(); // ゲームプレイ2D背景表示
		break;
	}
	
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
	// 各シーンの3D表示を呼び出す
	switch (sceneMode_) {
	case 0:
		GamePlayDraw3D(); // ゲームプレイ3D表示
		break;
	case 2:
		GamePlayDraw3D(); // ゲームプレイ3D表示
		break;
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
	// 各シーンの2D近景表示を呼び出す
	switch (sceneMode_) {
	case 0:
		GamePlayDraw2DNear(); // ゲームプレイ2D近景表示
		break;
	case 2:
		GamePlayDraw2DNear(); // ゲームプレイ2D近景表示
		GameOverDraw2DNear(); // ゲームオーバー2D近景表示
		break;
	}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}