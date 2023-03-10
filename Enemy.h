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

class Enemy {
  public: // �����o�֐�
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	Enemy();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Enemy();

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

	void EnemyBorn(); // �G����
	void EnemyUpdate(); // �G�X�V
	void EnemyMove();   // �G�ړ�

	int GetEnemyFlag() { return enemyFlag_; }
	float GetEnemyX() { return worldTransformEnemy_.translation_.x; }
	float GetEnemyZ() { return worldTransformEnemy_.translation_.z; }
	void Hit() { enemyFlag_ = 0; }

	private:
	// �G
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = 0;
	WorldTransform worldTransformEnemy_;

	float enemySpeed_ = 0;	// �ړ����x
	int enemyFlag_ = 0; // ���݃t���O
};