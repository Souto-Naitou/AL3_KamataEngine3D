#include "PlayerBullet.h"
#include <cassert>
#include "TextureManager.h"

void PlayerBullet::Initialize(Model* _model, const Vector3& _position)
{
	assert(_model);

	model_ = _model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = _position;
}

void PlayerBullet::Update()
{
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& _viewProjection)
{
	model_->Draw(worldTransform_, _viewProjection, textureHandle_);
}
