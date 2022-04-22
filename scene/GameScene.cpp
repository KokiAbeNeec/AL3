#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

// コンストラクタ
GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() { 
	delete spriteBG_;
	delete modelStage_;
	delete modelPlayer_;
	delete modelBeam_;
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
}

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

void GameScene::BeamUpdate() {
	BeamMove();		// 移動

	// 行列更新
	worldTransformBeam_.UpdateMatrix();

	BeamBorn();		// ビーム発生
}

// ビーム移動
void GameScene::BeamMove() {
	if (beamFlag_ == 1) {
		worldTransformBeam_.translation_.z += 0.1f;	// 移動
		worldTransformBeam_.rotation_.x += 0.1f;	// 回転
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
		worldTransformBeam_.translation_.z = worldTransformPlayer_.translation_.z;
		worldTransformBeam_.translation_.x = worldTransformPlayer_.translation_.x;
		beamFlag_ = 0;
	}
}

// 更新
void GameScene::Update() {
	PlayerUpdate();
	BeamUpdate();
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


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

