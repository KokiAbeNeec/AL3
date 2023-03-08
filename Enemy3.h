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

class Enemy3 {
  public: // �����o�֐�
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	Enemy3();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Enemy3();

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

	void Enemy3Born();   // �G����
	void Enemy3Update(); // �G�X�V
	void Enemy3Move();   // �G�ړ�

	int GetEnemy3Flag() { return enemy3Flag_; }
	float GetEnemy3X() { return worldTransformEnemy3_.translation_.x; }
	float GetEnemy3Z() { return worldTransformEnemy3_.translation_.z; }
	void Hit() { enemy3Flag_ = 0; }

  private:
	// �G
	uint32_t textureHandleEnemy3_ = 0;
	Model* modelEnemy3_ = 0;
	WorldTransform worldTransformEnemy3_;

	float enemy3Speed_ = 0;
	int enemy3Flag_ = 0; // ���݃t���O
};
