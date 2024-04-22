#include "Player.h"
#include <cassert>

void Player::Initialize(Model* _model, uint32_t _textureHandle) 
{
	// NULLチェック
	assert(_model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = _model;
	textureHandle_ = _textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();

}

void Player::Update() 
{

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

}

void Player::Draw(ViewProjection& _viewProjection) 
{

	model_->Draw(worldTransform_, _viewProjection, textureHandle_);

}
