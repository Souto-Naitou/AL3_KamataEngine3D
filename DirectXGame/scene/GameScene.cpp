#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() 
{

	delete player_;

	delete debugCamera_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャハンドルを取得
	textureHandle_ = TextureManager::Load("img/player.png");
	// 3Dモデルの生成
	model_ = Model::Create();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// デバッグカメラ生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する (アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	
}

void GameScene::Update() 
{

	// 自キャラの更新
	player_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0))
	{
		isDebugCameraActive_ = isDebugCameraActive_ ? false : true;
	}
#endif

	if (isDebugCameraActive_)
	{
		// デバッグカメラの更新
		debugCamera_->Update();

		const ViewProjection& debugCameraVP = debugCamera_->GetViewProjection();
		viewProjection_.matView = debugCameraVP.matView;
		viewProjection_.matProjection = debugCameraVP.matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
	else
	{
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}


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
	
	player_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
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
