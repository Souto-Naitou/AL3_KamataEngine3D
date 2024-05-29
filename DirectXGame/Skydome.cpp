#include "Skydome.h"

void Skydome::Initialize(Model* _model)
{
	model_ = _model;
	worldTransform_.Initialize();
}

void Skydome::Update()
{
	worldTransform_.scale_ = Vector3(50.0f, 50.0f, 50.0f);
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(ViewProjection& _viewProjection)
{
	model_->Draw(worldTransform_, _viewProjection);
}
