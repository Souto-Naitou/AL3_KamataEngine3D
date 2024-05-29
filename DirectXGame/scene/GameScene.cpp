#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Vector3/calc/vector3calc.h"
#include "Skydome.h"

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete player_;
	delete debugCamera_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャハンドルを取得
	textureHandle_ = TextureManager::Load("img/player.png");
	// 3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("tenkyu", true);

	// 自キャラの生成
	player_ = new Player();

	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	// 敵キャラの生成
	enemy_ = new Enemy();

	// 敵キャラの初期化
	enemy_->Initialize(model_, Vector3(6.0f, 2.0f, 100.0f), Vector3(0.0f, 0.0f, -0.05f));

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// デバッグカメラ生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する (アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	enemy_->SetPlayer(player_);

	skydome_ = std::make_unique<Skydome>();
	skydome_.get()->Initialize(modelSkydome_);
}

void GameScene::Update() 
{

	// 自キャラの更新
	player_->Update();
	// 敵キャラの更新
	if (enemy_)
		enemy_->Update();

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

	CheckAllCollisions();
	skydome_.get()->Update();
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
	
	skydome_.get()->Draw(viewProjection_);

	player_->Draw(viewProjection_);
	if (enemy_)
		enemy_->Draw(viewProjection_);

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

void GameScene::CheckAllCollisions()
{
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemyBullets)
	{
		posB = bullet->GetWorldPosition();
		float aToB = Length(Subtract(posA, posB));
		if (aToB < 3.0f)
		{
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	posA = enemy_->GetWorldPosition();
	for (PlayerBullet* bullet : playerBullets)
	{
		posB = bullet->GetWorldPosition();
		float aToB = Length(Subtract(posA, posB));
		if (aToB < 3.0f)
		{
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* pBullet : playerBullets)
	{
		for (EnemyBullet* eBullet : enemyBullets)
		{
			float aToB = Length(Subtract(pBullet->GetWorldPosition(), eBullet->GetWorldPosition()));
			if (aToB < 3.0f)
			{
				pBullet->OnCollision();
				eBullet->OnCollision();
			}
		}
	}
#pragma endregion
}
