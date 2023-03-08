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

class Player {
  public: // �����o�֐�
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	Player();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Player();

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

	void PlayerUpdate(); // �v���C���[�X�V

	float GetPlayerX() { return worldTransformPlayer_.translation_.x; }
	float GetPlayerZ() { return worldTransformPlayer_.translation_.z; }
	float PlayerResetX() { return worldTransformPlayer_.translation_.x = 0; }
	float PlayerResetZ() { return worldTransformPlayer_.translation_.z = -1.0f; }

	private:
	Input* input_ = nullptr;
	// �v���C���[
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;
};