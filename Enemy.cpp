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
	// 敵
	textureHandleEnemy_ = TextureManager::Load("enemy/enemy.png");
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
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
	EnemyMove(); // 移動

	// 行列更新
	worldTransformEnemy_.UpdateMatrix();

	EnemyBorn(); // 発生
}

void Enemy::EnemyMove() {

}

void Enemy::EnemyBorn() {

}