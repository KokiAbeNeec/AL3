#include "GameScene.h"
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
	delete beam;
	delete enemy;
	delete enemy2;
	delete enemy3;
	delete line;
	delete player;
}

// 初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// BG（スプライト）
	textureHandleBG_ = TextureManager::Load("bg.png");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});
	// タイトル
	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});
	// ゲームオーバー
	textureHandleGameOver_ = TextureManager::Load("gameover.png");
	spriteGameOver_ = Sprite::Create(textureHandleGameOver_, {0, 0});
	textureHandleGameOverFont_ = TextureManager::Load("gameoverfont.png");
	spriteGameOverFont_ = Sprite::Create(textureHandleGameOverFont_, {200, 200});
	// ゲームクリア
	textureHandleGameClear_ = TextureManager::Load("gameclear.png");
	spriteGameClear_ = Sprite::Create(textureHandleGameClear_, {0, 0});
	textureHandleGameClearFont_ = TextureManager::Load("gameclearfont.png");
	spriteGameClearFont_ = Sprite::Create(textureHandleGameClearFont_, {200, 200});
	// 決定
	textureHandlePush_ = TextureManager::Load("pressspacefont.png");
	spritePush_ = Sprite::Create(textureHandlePush_, {200, 500});

	// ビュープロジェクションの初期化
	viewProjection_.eye = {0, 5, -10};
	viewProjection_.target = {0, 1, 0};
	viewProjection_.Initialize();

	// ステージ
	textureHandleStage_ = TextureManager::Load("stage.png");
	modelStage_ = Model::Create();
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {10, 1, 20};
	worldTransformStage_.Initialize();

	// サウンドデータの読み込み
	soundDataHandleTitleBGM_ = audio_->LoadWave("audio/BGM1.wav");
	soundDataHandleGamePlayBGM_ = audio_->LoadWave("audio/BGM2.wav");
	soundDataHandleGameOverBGM_ = audio_->LoadWave("audio/BGM3.wav");
	soundDataHandleEnemyHitSE_ = audio_->LoadWave("audio/Attack.wav");
	soundDataHandlePushHitSE_ = audio_->LoadWave("audio/KetteionSE.wav");

	// タイトルBGMを再生
	voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true);

	// 乱数の初期化
	srand(time(NULL));

	enemy = new Enemy();
	enemy->Initialize();

	enemy3 = new Enemy3();
	enemy3->Initialize();

	player = new Player();
	player->Initialize();

	enemy2 = new Enemy2();
	enemy2->Initialize(player);

	beam = new Beam();
	beam->Initialize(player);

	line = new Line();
	line->Initialize(player);
}

// 衝突判定
void GameScene::Collision() { 
	// 衝突判定（プレイヤーと敵2）
	CollisionPlayerEnemy2();
	// 衝突判定（プレイヤーと敵3）
	CollisionPlayerEnemy3();
	// 衝突判定（ビームと敵）
	CollisionBeamEnemy();
	// 衝突判定（線と敵）
	CollisionLineEnemy();
}

// 衝突判定（プレイヤーと敵2）
void GameScene::CollisionPlayerEnemy2() {
	// 敵2が存在すれば
	if (enemy2->GetEnemy2Flag() == 1) {
		// 差を求める
		float dx = abs(player->GetPlayerX() - enemy2->GetEnemy2X());
		float dz = abs(player->GetPlayerZ() - enemy2->GetEnemy2Z());
		// 衝突したら
		if (dx < 1 && dz < 1) {
			enemy2->Hit();
			enemy3->Hit();
			player->PlayerResetX();
			player->PlayerResetZ();
			for (int i = 0; i < 15; i++) {
				line->LineResetX(i);
				line->LineResetZ(i);
				line->LineResetTime(i);
			}
			playerLife_--;
			audio_->PlayWave(soundDataHandleEnemyHitSE_);
			if (playerLife_ == 0) {
				sceneMode_ = 2;
				// BGM切り替え
				audio_->StopWave(voiceHandleBGM_); // BGMを停止
				voiceHandleBGM_ =
				  audio_->PlayWave(soundDataHandleGameOverBGM_, true); // ゲームプレイBGMを再生
			}
		}
	}
}

// 衝突判定（プレイヤーと敵3）
void GameScene::CollisionPlayerEnemy3() {
	// 敵2が存在すれば
	if (enemy3->GetEnemy3Flag() == 1) {
		// 差を求める
		float dx = abs(player->GetPlayerX() - enemy3->GetEnemy3X());
		float dz = abs(player->GetPlayerZ() - enemy3->GetEnemy3Z());
		// 衝突したら
		if (dx < 1 && dz < 1) {
			enemy2->Hit();
			enemy3->Hit();
			player->PlayerResetX();
			player->PlayerResetZ();
			for (int i = 0; i < 15; i++) {
				line->LineResetX(i);
				line->LineResetZ(i);
				line->LineResetTime(i);
			}
			playerLife_--;
			audio_->PlayWave(soundDataHandleEnemyHitSE_);
			if (playerLife_ == 0) {
				sceneMode_ = 2;
				// BGM切り替え
				audio_->StopWave(voiceHandleBGM_); // BGMを停止
				voiceHandleBGM_ =
				  audio_->PlayWave(soundDataHandleGameOverBGM_, true); // ゲームプレイBGMを再生
			}
		}
	}
}

// 衝突判定（ビームと敵）
void GameScene::CollisionBeamEnemy() {
	//// 敵が存在すれば
	//if (enemy->GetEnemyFlag() == 1) {
	//	// 差を求める
	//	float dx = abs(beam->GetBeamX() - enemy->GetEnemyX());
	//	float dz = abs(beam->GetBeamZ() - enemy->GetEnemyZ());
	//	// 衝突したら
	//	if (dx < 1 && dz < 1) {
	//		// 存在しない
	//		//enemy->GetEnemyFlag() == 0;
	//		enemy->Hit();
	//		beam->Hit();
	//		gameScore_++;
	//	}
	//}
}

void GameScene::CollisionLineEnemy() {
	// 敵が存在すれば
	if (enemy->GetEnemyFlag() == 1) {
		// 差を求める
		for (int i = 0; i < 15; i++) {
			float dx = abs(player->GetPlayerX() - line->GetLineX(i));
			float dz = abs(player->GetPlayerZ() - line->GetLineZ(i));
			// 衝突したら
			if (dx < 1 && dz < 1) {
				int a = 0;
				int b = 0;
				int c = 0;
				int d = 0;
				for (int k = 0; k < 15; k++) {
					float dx2 = abs(line->GetLineX(k) - enemy->GetEnemyX());
					float dz2 = abs(line->GetLineZ(k) - enemy->GetEnemyZ());
					if (dx2 > 1) {
						line->HitL();
						a = 1;
					}
					if (dx2 < 1) {
						line->HitR();
						b = 1;
					}
					if (dz2 > 1) {
						line->HitU();
						c = 1;
					}
					if (dz2 < 1) {
						line->HitD();
						d = 1;
					}
					if (a == 1 && b == 1 && c == 1 && d == 1) {
						k = 15;
						s += 1;
						if (s == 10) {
							sceneMode_ = 3;
							// BGM切り替え
							audio_->StopWave(voiceHandleBGM_); // BGMを停止
							voiceHandleBGM_ = audio_->PlayWave(
							  soundDataHandleGameOverBGM_, true); // ゲームプレイBGMを再生
						}
					}
					line->FullHit(enemy);
				}
			}
			line->HitReset();
		}
	}
}

// 初期化
void GameScene::GamePlayStart() { 
	player->PlayerResetX();
	player->PlayerResetZ();
	line->GameScoreReset();
	for (int i = 0; i < 15; i++) {
		line->LineResetX(i);
		line->LineResetZ(i);
		line->LineResetTime(i);
	}
	enemy->Hit();
	enemy2->Hit();
	enemy3->Hit();
	playerLife_ = 3;
	s = 0;
}

// タイトル更新
void GameScene::TitleUpdade() {
	// エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		// 初期化
		GamePlayStart();
		// モードをゲームプレイへ変更
		sceneMode_ = 1;

		audio_->PlayWave(soundDataHandlePushHitSE_);
		// BGM切り替え
		audio_->StopWave(voiceHandleBGM_); // BGMを停止
		voiceHandleBGM_ =audio_->PlayWave(soundDataHandleGamePlayBGM_, true); // ゲームプレイBGMを再生
	}
}

// ゲームオーバー更新
void GameScene::GameOverUpdade() {
	// エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		// モードをゲームプレイへ変更
		sceneMode_ = 0;

		audio_->PlayWave(soundDataHandlePushHitSE_);
		// BGM切り替え
		audio_->StopWave(voiceHandleBGM_); // BGMを停止
		voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true); // ゲームプレイBGMを再生
	}
}

void GameScene::GameClearUpdade() {
	// エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		// モードをゲームプレイへ変更
		sceneMode_ = 0;

		audio_->PlayWave(soundDataHandlePushHitSE_);
		// BGM切り替え
		audio_->StopWave(voiceHandleBGM_);                                  // BGMを停止
		voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true); // ゲームプレイBGMを再生
	}
}

// 更新
void GameScene::Update() {
	switch (sceneMode_) {
	case 0:
		TitleUpdade();
		break;
	case 1:
		player->Update();
		Collision();
		enemy->Update();
		enemy2->Update();
		enemy3->Update();
		line->Update();
		break;
	case 2:
		GameOverUpdade();
		break;
	case 3:
		GameClearUpdade();
		break;
	}
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
	switch (sceneMode_) {
	case 0:
		spriteTitle_->Draw();
		spritePush_->Draw();
		break;
	case 1:
		spriteBG_->Draw();
		break;
	case 2:
		spriteGameOver_->Draw();
		spriteGameOverFont_->Draw();
		break;
	case 3:
		spriteGameClear_->Draw();
		spriteGameClearFont_->Draw();
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
	switch (sceneMode_) {
	case 1:
		// ステージ
		modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

		player->Draw(viewProjection_);
		beam->Draw(viewProjection_);
		enemy->Draw(viewProjection_);
		enemy2->Draw(viewProjection_);
		enemy3->Draw(viewProjection_);
		line->Draw(viewProjection_);
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
	switch (sceneMode_) {
	case 1:
		// ゲームスコア
		char str[100];
		sprintf_s(str, "SCORE %d", line->GetGameScore());
		debugText_->Print(str, 200, 10, 2);

		// プレイヤーライフ
		sprintf_s(str, "LIFE %d", playerLife_);
		debugText_->Print(str, 900, 10, 2);
		break;
	}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}