#include "Beam.h"
#include "TextureManager.h"
#include <cassert>
#include <time.h>

Beam::Beam() {

}

Beam::~Beam() {
	delete modelBeam_;
}

void Beam::Initialize(Player* player) {
	player_ = player;
	input_ = Input::GetInstance();
	// �r�[��
	textureHandleBeam_ = TextureManager::Load("Beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformBeam_.Initialize();
}

void Beam::Update() {
	BeamUpdate();
}

void Beam::Draw(ViewProjection viewProjection_) {
	if (beamFlag_ == 1) {
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	}
}

void Beam::BeamUpdate() {
	BeamMove(); // �ړ�

	// �s��X�V
	worldTransformBeam_.UpdateMatrix();

	BeamBorn(); // ����
}

void Beam::BeamMove() {
	if (beamFlag_ == 1) {
		worldTransformBeam_.translation_.z += 0.1f; // �ړ�
		worldTransformBeam_.rotation_.x += 0.1f;    // ��]
	} else {
		worldTransformBeam_.translation_.z = player_->GetPlayerZ();
		worldTransformBeam_.translation_.x = player_->GetPlayerX();
	}
}

void Beam::BeamBorn() {
	// �r�[������
	if (input_->PushKey(DIK_SPACE) && beamFlag_ == 0) {
		worldTransformBeam_.translation_.z = player_->GetPlayerZ();
		worldTransformBeam_.translation_.x = player_->GetPlayerX();
		beamFlag_ = 1;
	}
	// �r�[������
	if (worldTransformBeam_.translation_.z >= 40) {
		beamFlag_ = 0;
	}
}