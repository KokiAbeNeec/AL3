#include "Enemy.h"
#include "TextureManager.h"
#include <cassert>
#include <time.h>

Enemy::Enemy() {

}

Enemy::~Enemy() { 
	delete modelEnemy_;
}

void Enemy::Initialize() {
	// “G
	textureHandleEnemy_ = TextureManager::Load("enemy/enemy.png");
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.rotation_ = {0.0f, 0.9f, 0.0f};
	worldTransformEnemy_.Initialize();
}

void Enemy::Update() { 
	EnemyUpdate();
}

void Enemy::Draw(ViewProjection viewProjection_) {
	if (enemyFlag_ == 1) {
		modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
	}
}

void Enemy::EnemyUpdate() {
	EnemyMove(); // ˆÚ“®

	// s—ñXV
	worldTransformEnemy_.UpdateMatrix();

	EnemyBorn(); // ”­¶
}

void Enemy::EnemyMove() {

}

void Enemy::EnemyBorn() {
	if (enemyFlag_ == 0) {
		int x = rand() % 80;
		float x2 = (float)x / 10 - 4;
		int z = rand() % 80 + 40;
		float z2 = (float)z / 10 - 4;
		worldTransformEnemy_.translation_.x = x2;
		worldTransformEnemy_.translation_.z = z2;
		enemyFlag_ = 1;
	}
}
