#include "Enemy3.h"
#include "TextureManager.h"
#include <cassert>
#include <time.h>

Enemy3::Enemy3() {}

Enemy3::~Enemy3() { delete modelEnemy3_; }

void Enemy3::Initialize() {
	// “G
	textureHandleEnemy3_ = TextureManager::Load("enemy3/enemy3.png");
	modelEnemy3_ = Model::CreateFromOBJ("enemy3", true);
	worldTransformEnemy3_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy3_.Initialize();
}

void Enemy3::Update() { Enemy3Update(); }

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
	worldTransformEnemy3_.translation_.x += enemy3Speed_;
}

void Enemy3::Enemy3Born() {
	if (enemy3Flag_ == 0) {
		int x = rand() % 2;
		float x2 = 0;
		if (x == 0) {
			x2 = 10.0f;
		} else {
			x2 = -10.0f;
		}
		int z = rand() % 80 + 40;
		float z2 = (float)z / 10 - 4;
		worldTransformEnemy3_.translation_.x = x2;
		worldTransformEnemy3_.translation_.z = z2;
		if (worldTransformEnemy3_.translation_.x == 10.0f) {
			enemy3Speed_ = -0.05f;
			worldTransformEnemy3_.rotation_.y = -5.0f;
		} else {
			enemy3Speed_ = 0.05f;
			worldTransformEnemy3_.rotation_.y = 5.0f;
		}
		enemy3Flag_ = 1;
	}

	if (enemy3Flag_ == 1 && worldTransformEnemy3_.translation_.x >= 10.5f ||
	  worldTransformEnemy3_.translation_.x <= -10.5f) {
		enemy3Flag_ = 0;
	}
}
