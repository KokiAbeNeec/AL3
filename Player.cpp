#include "Player.h"
#include "TextureManager.h"
#include <cassert>
#include <time.h>

Player::Player() {

}

Player::~Player() { 
	delete modelPlayer_; 
}

void Player::Initialize() {
	input_ = Input::GetInstance();
	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player/player.png");
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();
}

void Player::Update() {
	PlayerUpdate();
}

void Player::Draw(ViewProjection viewProjection_) {
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
}

void Player::PlayerUpdate() {
	// 移動
	// 右へ移動と右の移動範囲制限
	if (input_->PushKey(DIK_D) && worldTransformPlayer_.translation_.x <= 8.0f) {
		worldTransformPlayer_.translation_.x += 0.2f;
		worldTransformPlayer_.rotation_.y = 5.0f;
	}
	// 左へ移動と左の移動範囲制限
	if (input_->PushKey(DIK_A) && worldTransformPlayer_.translation_.x >= -8.0f) {
		worldTransformPlayer_.translation_.x -= 0.2f;
		worldTransformPlayer_.rotation_.y = -5.0f;
	}
	// 奥へ移動と奥の移動範囲制限
	if (input_->PushKey(DIK_W) && worldTransformPlayer_.translation_.z <= 16.0f) {
		worldTransformPlayer_.translation_.z += 0.2f;
		worldTransformPlayer_.rotation_.y = 9.5f;
	}
	// 手前へ移動と手前の移動範囲制限
	if (input_->PushKey(DIK_S) && worldTransformPlayer_.translation_.z >= -4.0f) {
		worldTransformPlayer_.translation_.z -= 0.2f;
		worldTransformPlayer_.rotation_.y = 0.0f;
	}

	// 行列更新
	worldTransformPlayer_.UpdateMatrix();
}