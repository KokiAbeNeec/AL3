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
	// �v���C���[
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
	// �ړ�
	// �E�ֈړ��ƉE�̈ړ��͈͐���
	if (input_->PushKey(DIK_D) && worldTransformPlayer_.translation_.x <= 8.0f) {
		worldTransformPlayer_.translation_.x += 0.2f;
		worldTransformPlayer_.rotation_.y = 5.0f;
	}
	// ���ֈړ��ƍ��̈ړ��͈͐���
	if (input_->PushKey(DIK_A) && worldTransformPlayer_.translation_.x >= -8.0f) {
		worldTransformPlayer_.translation_.x -= 0.2f;
		worldTransformPlayer_.rotation_.y = -5.0f;
	}
	// ���ֈړ��Ɖ��̈ړ��͈͐���
	if (input_->PushKey(DIK_W) && worldTransformPlayer_.translation_.z <= 16.0f) {
		worldTransformPlayer_.translation_.z += 0.2f;
		worldTransformPlayer_.rotation_.y = 9.5f;
	}
	// ��O�ֈړ��Ǝ�O�̈ړ��͈͐���
	if (input_->PushKey(DIK_S) && worldTransformPlayer_.translation_.z >= -4.0f) {
		worldTransformPlayer_.translation_.z -= 0.2f;
		worldTransformPlayer_.rotation_.y = 0.0f;
	}

	// �s��X�V
	worldTransformPlayer_.UpdateMatrix();
}