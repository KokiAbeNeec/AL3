#include "Line.h"
#include "TextureManager.h"
#include <cassert>
#include <time.h>

Line::Line() {

}

Line::~Line() { 
	delete modelLine_;
}

void Line::Initialize(Player* player) {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	player_ = player;
	textureHandleLine_ = TextureManager::Load("enemy4/enemy4.png");
	modelLine_ = Model::CreateFromOBJ("enemy4", true);
	for (int i = 0; i < 15; i++) {
		worldTransformLine_[i].scale_ = {0.3f, 0.3f, 0.3f};
		worldTransformLine_[i].Initialize();
	}
	soundDataHandlePlayerHitSE_ = audio_->LoadWave("audio/Kakomu.wav");
}

void Line::Update() {
	// çsóÒçXêV
	for (int i = 0; i < 15; i++) {
		worldTransformLine_[i].UpdateMatrix();
	}

	LineBorn(); // î≠ê∂
}

void Line::Draw(ViewProjection viewProjection_) {
	for (int i = 0; i < 15; i++) {
		if (LineFlag_[i] == 1) {
			modelLine_->Draw(worldTransformLine_[i], viewProjection_, textureHandleLine_);
		}
	}
}

void Line::LineBorn() { 
	for (int i = 0; i < 15; i++) {
		if (LineTime_[i] > 0) {
			LineTime_[i]--;
		}
	}

	// ê¸î≠ê∂
	for (int i = 0; i < 15; i++) {
		if (LineTime_[i] == 0) {
			if (input_->PushKey(DIK_D)) {
				worldTransformLine_[i].translation_.x = player_->GetPlayerX() - 1;
				worldTransformLine_[i].translation_.z = player_->GetPlayerZ();
				worldTransformLine_[i].rotation_.y = 5.0f;
			}
			else if (input_->PushKey(DIK_A)) {
				worldTransformLine_[i].translation_.x = player_->GetPlayerX() + 1;
				worldTransformLine_[i].translation_.z = player_->GetPlayerZ();
				worldTransformLine_[i].rotation_.y = -5.0f;
			}
			else if (input_->PushKey(DIK_W)) {
				worldTransformLine_[i].translation_.x = player_->GetPlayerX();
				worldTransformLine_[i].translation_.z = player_->GetPlayerZ() - 1;
				worldTransformLine_[i].rotation_.y = 9.5f;
			}
			else if (input_->PushKey(DIK_S)) {
				worldTransformLine_[i].translation_.x = player_->GetPlayerX();
				worldTransformLine_[i].translation_.z = player_->GetPlayerZ() + 1;
				worldTransformLine_[i].rotation_.y = 0.0f;
			} else {
				worldTransformLine_[i].translation_.x = player_->GetPlayerX();
				worldTransformLine_[i].translation_.z = player_->GetPlayerZ();
				worldTransformLine_[i].rotation_.y = 0.0f;
			}
			LineFlag_[i] = 1;
			LineTime_[i] = 150;
		}
	}
}

void Line::FullHit(Enemy* enemy) {
	if (HitFlagL_ == 1 && HitFlagR_ == 1 && HitFlagU_ == 1 && HitFlagD_ == 1) {
		gameScore_ += 100;
		Scoreflag_ = 1;
		enemy->Hit();
		audio_->PlayWave(soundDataHandlePlayerHitSE_);
		int k = 0;
		for (int i = 0; i < 15; i++) {
			worldTransformLine_[i].translation_.x = 200;
			worldTransformLine_[i].translation_.z = 200;
			LineFlag_[i] = 0;
			LineTime_[i] = k;
			k += 10;
		}
	}
}