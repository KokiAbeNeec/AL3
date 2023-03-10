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

	private:
	Input* input_ = nullptr;
	// �v���C���[
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	float playerSpeed_ = 0; // �ړ����x
	int playerFlag_ = 0; // ���݃t���O
};