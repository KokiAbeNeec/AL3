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

class Enemy2 {
  public: // �����o�֐�
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	Enemy2();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Enemy2();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	void Enemy2Born();   // �G����
	void Enemy2Update(); // �G�X�V
	void Enemy2Move(); // �G�ړ�

	int GetEnemy2Flag() { return enemy2Flag_; }
	float GetEnemy2X() { return worldTransformEnemy2_.translation_.x; }
	float GetEnemy2Z() { return worldTransformEnemy2_.translation_.z; }
	void Hit() { enemy2Flag_ = 0; }

  private:
	// �G
	uint32_t textureHandleEnemy2_ = 0;
	Model* modelEnemy2_ = 0;
	WorldTransform worldTransformEnemy2_;

	float enemy2Speed_ = 0;	// �ړ����x
	int enemy2Flag_ = 0; // ���݃t���O
};