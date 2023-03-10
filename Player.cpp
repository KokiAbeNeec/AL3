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
	if (playerFlag_ == 1) {
		modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
	}
}

void Player::PlayerUpdate() {

	// 行列更新
	worldTransformPlayer_.UpdateMatrix();
}