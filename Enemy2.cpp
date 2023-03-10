#include "Enemy2.h"
#include "TextureManager.h"
#include <cassert>
#include <time.h>

Enemy2::Enemy2() {

}

Enemy2::~Enemy2() { 
	delete modelEnemy2_;
}

void Enemy2::Initialize() {
	// “G
	textureHandleEnemy2_ = TextureManager::Load("enemy2/enemy2.png");
	modelEnemy2_ = Model::CreateFromOBJ("enemy2", true);
	worldTransformEnemy2_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy2_.Initialize();
}

void Enemy2::Update() {
	Enemy2Update();
}

void Enemy2::Draw(ViewProjection viewProjection_) {
	if (enemy2Flag_ == 1) {
		modelEnemy2_->Draw(worldTransformEnemy2_, viewProjection_, textureHandleEnemy2_);
	}
}

void Enemy2::Enemy2Update() {
	Enemy2Move(); // ˆÚ“®

	// s—ñXV
	worldTransformEnemy2_.UpdateMatrix();

	Enemy2Born(); // ”­¶
}

void Enemy2::Enemy2Move() { 

}

void Enemy2::Enemy2Born() {

}