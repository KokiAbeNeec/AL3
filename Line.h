#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>
#include "Player.h"
#include "Enemy.h"

class Line {
  public: // �����o�֐�
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	Line();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Line();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Player* player);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	void LineBorn(); // �G����

	float GetLineX(int i) { return worldTransformLine_[i].translation_.x; }
	float GetLineZ(int i) { return worldTransformLine_[i].translation_.z; }
	void HitL() { HitFlagL_ = 1; }
	void HitR() { HitFlagR_ = 1; }
	void HitU() { HitFlagU_ = 1; }
	void HitD() { HitFlagD_ = 1; }
	void FullHit(Enemy* enemy);
	void HitReset() {
		HitFlagL_ = 0;
		HitFlagR_ = 0;
		HitFlagU_ = 0;
		HitFlagD_ = 0;
	}
	float LineResetX(int i) { return worldTransformLine_[i].translation_.x = 200; }
	float LineResetZ(int i) { return worldTransformLine_[i].translation_.z = 200; }
	int LineResetTime(int i) { return LineFlag_[i] = i * 10; }
	int GetGameScore() { return gameScore_; }
	int GameScoreReset() { return gameScore_ = 0; }
  private:
	Input* input_ = nullptr;
	// ��
	uint32_t textureHandleLine_ = 0;
	Model* modelLine_ = nullptr;
	WorldTransform worldTransformLine_[15];

	int LineFlag_[15] = {}; // ���݃t���O
	int HitFlagL_ = 0;
	int HitFlagR_ = 0;
	int HitFlagU_ = 0;
	int HitFlagD_ = 0;
	int LineTime_[15] = {0,  10, 20,  30,  40,  50,  60, 70,
	                     80, 90, 100, 110, 120, 130, 140}; // ���ݎ���
	// �Q�[���X�R�A
	int gameScore_ = 0;
	int Scoreflag_ = 0;

	uint32_t soundDataHandlePlayerHitSE_ = 0; // �v���C���[�q�b�gSE
	
	Player* player_;
	Enemy* enemy_;
	Audio* audio_;
};