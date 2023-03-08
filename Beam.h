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

class Beam {
  public: // �����o�֐�
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	Beam();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Beam();

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

	void BeamBorn();    // �G����
	void BeamUpdate(); // �G�X�V
	void BeamMove();    // �G�ړ�

	float GetBeamX() { return worldTransformBeam_.translation_.x; }
	float GetBeamZ() { return worldTransformBeam_.translation_.z; }
	void Hit() { beamFlag_ = 0; }
	private:
	Input* input_ = nullptr;
	// �r�[��
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;

	int beamFlag_ = 0; // ���݃t���O
	Player* player_;
};