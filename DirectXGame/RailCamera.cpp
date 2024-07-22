#include "RailCamera.h"
#include "Matrix4x4/calc/matrix4calc.h"
#include <ImGuiManager.h>
#include "Interpolation.h"
#include <vector>
#include <Vector3.h>
#include <PrimitiveDrawer.h>
#include "Vector3/calc/vector3calc.h"

void RailCamera::Initialize(Vector3 _translate, Vector3 _rotate)
{
	worldTransform_.Initialize();
	// ワールドトランスフォームの初期設定
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransform_.rotation_ = _rotate;
	worldTransform_.translation_ = _translate;
	worldTransform_.matWorld_ = MakeAffineMatrix(
		worldTransform_.scale_,
		worldTransform_.rotation_,
		worldTransform_.translation_
	);

	controlPoints_ = {
	{0,0,0},
	{10,10,10},
	{10,15,20},
	{20,15,30},
	{20,0,2},
	{30,0,10},
	};

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	translateSpeed_.z = -0.05f;
}

void RailCamera::Update()
{
	//for (int i = 0; i < 3; i++)
	//{
	//	*(&worldTransform_.translation_.x + i) += *(&translateSpeed_.x + i);
	//	*(&worldTransform_.rotation_.x + i) += *(&rotateSpeed_.x + i);
	//}


	t += 0.003f;
	float t2 = t + 0.1f;
	if (t >= 1.0f) t = 0.0f;
	if (t2 >= 1.0f) t2 = 1.0f;

	Vector3 position = interpolation_.CatmullRom(controlPoints_, t);
	if (t2 < 1.0f)
	{
		Vector3 rpos = interpolation_.CatmullRom(controlPoints_, t2);
		Vector3 distTtoT2 = rpos - position;

		float zxLen = sqrtf(powf(distTtoT2.x, 2.0f) + powf(distTtoT2.z, 2.0f));
		float targetZ = atan2f(distTtoT2.y, distTtoT2.x);
		float targetX = atan2f(-distTtoT2.y, zxLen);
		float targetY = atan2f(distTtoT2.z, distTtoT2.x);
		worldTransform_.rotation_.x = targetX;
		worldTransform_.rotation_.y = targetY;
		worldTransform_.rotation_.z = targetZ;
	}

	worldTransform_.translation_ = position;

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
	ImGui::DragFloat3("speed.translate", &translateSpeed_.x, 0.001f);
	ImGui::DragFloat3("speed.rotate", &rotateSpeed_.x, 0.001f);

	worldTransform_.TransferMatrix();

	ImGui::End();
}

void RailCamera::Draw(const ViewProjection& _viewProjection)
{
	interpolation_.DrawCatmullRom(controlPoints_, _viewProjection, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}
