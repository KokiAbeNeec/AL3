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
	delete modelPlayer_;
	delete modelBeam_;
	delete modelEnemy_;
	delete spriteTitle_;
	delete spriteEnter_;
	delete spriteGameOver_;
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
	for (int i = 0; i < 10; i++) {
		textureHandleBeam_ = TextureManager::Load("Beam.png");
		modelBeam_ = Model::Create();
		worldTransformBeam_[i].scale_ = {0.5f, 0.5f, 0.5f};
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
	for (int i = 0; i < 10; i++) {
		worldTransformBeam_[i].UpdateMatrix();	
	}

	BeamBorn();		// 発生
}

// ビーム移動
void GameScene::BeamMove() {
	for (int i = 0; i < 10; i++) {
		if (beamFlag_[i] == 1) {
			worldTransformBeam_[i].translation_.z += 0.5f; // 移動
			worldTransformBeam_[i].rotation_.x += 0.5f;    // 回転
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
}

// 敵移動
void GameScene::EnemyMove() {
	for (int i = 0; i < 10; i++) {
		if (enemyFlag_[i] == 1) {
			worldTransformEnemy_[i].translation_.x += enemySpeed_[i];
			worldTransformEnemy_[i].translation_.z -= 0.5f; // 移動
			worldTransformEnemy_[i].rotation_.x -= 0.5f;    // 回転
		}
	}
	for (int i = 0; i < 10; i++) {
		if (worldTransformEnemy_[i].translation_.x >= 5) {
			enemySpeed_[i] = -0.2f;
		} else if (worldTransformEnemy_[i].translation_.x <= -5) {
			enemySpeed_[i] = 0.2f;
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
				worldTransformEnemy_[i].translation_.z = 40;
				enemyFlag_[i] = 1;
				break;
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
	// 敵が存在すれば
	for (int i = 0; i < 10; i++) {
		if (enemyFlag_[i] == 1) {
			// 差を求める
			float dx = abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_[i].translation_.x);
			float dz = abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_[i].translation_.z);
			// 衝突したら
			if (dx < 1 && dz < 1) {
				// 存在しない
				enemyFlag_[i] = 0;
				playerLife_--;
			}
		}	
	}
	if (playerLife_ == 0) {
		sceneMode_ = 2;
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
					// 存在しない
					enemyFlag_[i] = 0;
					beamFlag_[k] = 0;
					gameScore_++;
				}
			}
		}
	}
}

// **************************************************
// タイトル
// **************************************************
// タイトル更新
void GameScene::TitleUpdade() {
	// エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_RETURN)) {
		// モードをゲームプレイへ変更
		sceneMode_ = 0;
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
	 }
 }

// ゲームオーバー表示
void GameScene::GameOverDraw2DNear() {
	 spriteGameOver_->Draw();
	 if (gameTimer_ % 40 >= 20) {
		 spriteEnter_->Draw();
	 }
 }

// 初期化
void GameScene::GamePlayStart() {
	 for (int i = 0; i < 10; i++) {
		 enemyFlag_[i] = 0;
		 beamFlag_[i] = 0;
	 }
	playerLife_ = 1;
	gameScore_ = 0;
 }

// 更新
void GameScene::Update() { 
	// 各シーンの更新処理を呼び出す
	switch (sceneMode_){
	case 0:
		GamePlayUpdate();	// ゲームプレイ更新
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

  // ゲームプレイ更新
void GameScene::GamePlayUpdate() {
	PlayerUpdate(); // プレイヤー更新
	BeamUpdate();   // 敵更新
	EnemyUpdate();  // ビーム更新
	Collision();    // 衝突判定
}

// ゲームプレイ3D表示
void GameScene::GamePlayDraw3D() {
	// ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	// プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);

	// ビーム
	for (int i = 0; i < 10; i++) {
		if (beamFlag_[i] == 1) {
			modelBeam_->Draw(worldTransformBeam_[i], viewProjection_, textureHandleBeam_);
		}
	}

	// 敵
	for (int i = 0; i < 10; i++) {
		if (enemyFlag_[i] == 1) {
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
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	sprintf_s(str, "LIFE %d", playerLife_);
	debugText_->Print(str, 900, 10, 2);
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