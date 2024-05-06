#include "Player.h"
#include <cassert>
#include <ImGuiManager.h>

#include "Matrix4x4/calc/matrix4calc.h"
#include "Vector3/calc/vector3calc.h"
#include "MathExtension/mathExtension.h"

void Player::Initialize(Model* _model, uint32_t _textureHandle) 
{
	// NULLチェック
	assert(_model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = _model;
	textureHandle_ = _textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

}

void Player::Update() 
{
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_UP))
	{
		move.y += kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		move.y -= kCharacterSpeed;
	}

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// 移動限界座標
	const float kMoveLimitX = 32;
	const float kMoveLimitY = 16;

	// 範囲を超えない処理
	worldTransform_.translation_.x = clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	// キャラクター攻撃処理
	Attack();

	// 弾更新
	if (bullet_)
	{
		bullet_->Update();
	}

	ImGui::Begin("pos");

	ImGui::Text("x:%3.1f", worldTransform_.translation_.x);
	ImGui::Text("y:%3.1f", worldTransform_.translation_.y);

	ImGui::End();
}

void Player::Draw(ViewProjection& _viewProjection) 
{

	model_->Draw(worldTransform_, _viewProjection, textureHandle_);

	// 弾描画
	if (bullet_)
	{
		bullet_->Draw(_viewProjection);
	}

}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		// 弾を登録する
		bullet_ = newBullet;
	}
}

void Player::Rotate()
{
	// 回転速さ{ラジアン / frame}
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A))
	{
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D))
	{
		worldTransform_.rotation_.y += kRotSpeed;
	}
}
