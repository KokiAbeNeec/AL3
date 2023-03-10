#include "Enemy3.h"
#include "TextureManager.h"
#include <cassert>
#include <time.h>

Enemy3::Enemy3() {

}

Enemy3::~Enemy3() { 
	delete modelEnemy3_;
}

void Enemy3::Initialize() {
	// “G
	textureHandleEnemy3_ = TextureManager::Load("enemy3/enemy3.png");
	modelEnemy3_ = Model::CreateFromOBJ("enemy3", true);
	worldTransformEnemy3_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy3_.Initialize();
}

void Enemy3::Update() {
	Enemy3Update();
}

void Enemy3::Draw(ViewProjection viewProjection_) {
	if (enemy3Flag_ == 1) {
		modelEnemy3_->Draw(worldTransformEnemy3_, viewProjection_, textureHandleEnemy3_);
	}
}

void Enemy3::Enemy3Update() {
	Enemy3Move(); // ˆÚ“®

	// s—ñXV
	worldTransformEnemy3_.UpdateMatrix();

	Enemy3Born(); // ”­¶
}

void Enemy3::Enemy3Move() { 

}

void Enemy3::Enemy3Born() {

}