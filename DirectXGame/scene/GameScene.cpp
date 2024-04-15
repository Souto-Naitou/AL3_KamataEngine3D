#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <ImGuiManager.h>
#include <PrimitiveDrawer.h>
#include <AxisIndicator.h>


GameScene::GameScene() {}

GameScene::~GameScene() { 

	delete sprite_, model_, debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("img/play.png");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	soundDataHandle_ = audio_->LoadWave("se/put.mp3");
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
	// 移動した座標をスプライトに反映
	sprite_->SetPosition(Vector2(inputFloat[0], inputFloat[1]));
	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->PlayWave(soundDataHandle_);
	}
	ImGui::Begin("Debug");
	ImGui::Text("sprite: (%d, %d)", int(inputFloat[0]), int(inputFloat[1]));
	bool isPressButton_pre = isPressButton;
	isPressButton = ImGui::Button("playButton");
	ImGui::Checkbox("EnableDebugCamera", &enableDebugCamera);
	ImGui::SliderFloat2("Position", inputFloat, 0.0f, 1280.0f);
	ImGui::End();
	if (isPressButton && !isPressButton_pre) {
		audio_->PlayWave(soundDataHandle_);
	}
	if (enableDebugCamera)
	{
		debugCamera_->Update();
	}
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
	for (int i = 0; i < 40; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d({-50.0f + i * 2.5f, 0, -50.0f}, {-50.0f + i * 2.5f, 0, 50.0f}, {1.0f, 0.0f, 0.0f, 1.0f});
		for (int j = 0; j < 40; j++)
		{
			PrimitiveDrawer::GetInstance()->DrawLine3d({-50.0f, 0, -50.0f + j * 2.5f}, {50.0f, 0, -50.0f + j * 2.5f}, {0.0f, 0.0f, 1.0f, 1.0f});
		}
	}
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
	

}
