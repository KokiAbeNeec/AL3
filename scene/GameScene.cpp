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
	delete enemy;
	delete enemy2;
	delete enemy3;
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

	player = new Player();
	player->Initialize();

	enemy = new Enemy();
	enemy->Initialize();

	enemy2 = new Enemy2();
	enemy2->Initialize();

	enemy3 = new Enemy3();
	enemy3->Initialize();
}

// 初期化
void GameScene::GamePlayStart() { 

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
		enemy->Update();
		enemy2->Update();
		enemy3->Update();
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
		enemy->Draw(viewProjection_);
		enemy2->Draw(viewProjection_);
		enemy3->Draw(viewProjection_);
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
		char str[100];

		break;
	}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}