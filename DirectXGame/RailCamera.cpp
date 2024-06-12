#include "RailCamera.h"
#include "Matrix4x4/calc/matrix4calc.h"
#include <ImGuiManager.h>

void RailCamera::Initialize(Vector3 _translate, Vector3 _rotate)
{
	// ワールドトランスフォームの初期設定
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransform_.rotation_ = _rotate;
	worldTransform_.translation_ = _translate;
	worldTransform_.matWorld_ = MakeAffineMatrix(
		worldTransform_.scale_,
		worldTransform_.rotation_,
		worldTransform_.translation_
	);

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	translateSpeed_.z = -0.01f;
}

void RailCamera::Update()
{
	for (int i = 0; i < 3; i++)
	{
		*(&worldTransform_.translation_.x + i) += *(&translateSpeed_.x + i);
		*(&worldTransform_.rotation_.x + i) += *(&rotateSpeed_.x + i);
	}
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.matProjection = MakePerspectiveFovMatrix(
		viewProjection_.fovAngleY, float(1280) / float(720), viewProjection_.nearZ, viewProjection_.farZ
	);
	
	// カメラの座標を画面表示する処理
	ImGui::SetNextWindowSize(ImVec2(340, 150));
	ImGui::Begin("Camera", (bool*)false, ImGuiWindowFlags_NoResize);
	ImGui::DragFloat3("position", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("speed.translate", &translateSpeed_.x, 0.01f);
	ImGui::DragFloat3("speed.rotate", &rotateSpeed_.x, 0.01f);

	ImGui::End();
}
