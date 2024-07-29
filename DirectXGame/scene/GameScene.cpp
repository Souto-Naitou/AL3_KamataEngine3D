#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Vector3/calc/vector3calc.h"
#include "Skydome.h"
#include <fstream>
#include <filesystem>

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete player_;
	delete debugCamera_;
	delete modelSkydome_;
	for (Enemy* enemy : enemies_) delete enemy;
	for (EnemyBullet* eBullet : enemyBullets_) delete eBullet;
	delete collisionManager;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャハンドルを取得
	textureHandle_ = TextureManager::Load("img/player.png");
	TextureManager::Load("x.png");

	// 3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("tenkyu", true);

	// レールカメラ生成
	railCamera_ = new RailCamera();
	railCamera_->Initialize({ 0,0,1 }, { 0, 0, 0 });

	// 自キャラの生成
	player_ = new Player();

	// 自キャラの初期化
	Vector3 playerPosition(0, 0, 0);
	player_->Initialize(model_, textureHandle_, playerPosition);
	player_->SetParent(&railCamera_->GetWorldTransform());
	player_->SetEnemies(&enemies_);

	// 敵生成
	MakeEnemyInstance(Vector3(6.0f, 2.0f, 100.0f));

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// デバッグカメラ生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する (アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	skydome_ = std::make_unique<Skydome>();
	skydome_.get()->Initialize(modelSkydome_);

	// 敵発生スクリプトの読み込み
	LoadEnemyPopData();

	collisionManager = new CollisionManager;
}

void GameScene::Update() 
{
	// 自キャラの更新
	player_->Update(viewProjection_);
	// 敵キャラの更新
	for (Enemy* enemy : enemies_)
	{
		enemy->Update();
	}

	// レールカメラの更新
	if (railCamera_)
		railCamera_->Update();

	// 弾更新
	for (EnemyBullet* bullet : enemyBullets_)
	{
		bullet->Update();
	}

	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet)
		{
			if (bullet->IsDead())
			{
				delete bullet;
				return true;
			}
			return false;
		}
	);

	enemies_.remove_if([](Enemy* enemy)
		{
			if (enemy->IsDead())
			{
				delete enemy;
				return true;
			}
			return false;
		}
	);

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
		const ViewProjection& railCamVP = railCamera_->GetViewProjection();
		viewProjection_.matView = railCamVP.matView;
		viewProjection_.matProjection = railCamVP.matProjection;
		// ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}

	collisionManager->ClearColliderList();

	collisionManager->RegisterCollider(player_);
	for (auto enemy : enemies_) collisionManager->RegisterCollider(enemy);
	for (auto pBullet : player_->GetBullets()) collisionManager->RegisterCollider(pBullet);
	for (auto eBullet : enemyBullets_) collisionManager->RegisterCollider(eBullet);

	collisionManager->CheckAllCollisions();

	skydome_.get()->Update();

	// 敵発生コマンドを更新
	UpdateEnemyPopCommands();
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
	for (Enemy* enemy : enemies_)
	{
		enemy->Draw(viewProjection_);
	}
	// 弾描画
	//for (EnemyBullet* bullet : enemyBullets_)
	//{
	//	bullet->Draw(viewProjection_);
	//}

	railCamera_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::LoadEnemyPopData()
{
	// ファイルを開く
	std::ifstream file;
	//auto path = std::filesystem::current_path();
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands()
{
	// 待機処理
	if (isWaitEnemy)
	{
		enemyWaitTime--;
		if (enemyWaitTime <= 0)
		{
			// 待機終了
			isWaitEnemy = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0)
		{
			// コメント行を飛ばす
			continue;
		}
		if (word.find("POP") == 0)
		{
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			MakeEnemyInstance(Vector3(x, y, z));
		}
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWaitEnemy = true;
			enemyWaitTime = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::CheckAllCollisions()
{
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	// コライダー
	std::list<Collider*> colliders_;
	// コライダーをリストに登録
	colliders_.push_back(player_);
	for (Collider* col : enemies_) colliders_.push_back(col);

	// 自弾すべてについて
	for (PlayerBullet* pBullet : playerBullets)
	{
		colliders_.push_back(pBullet);
	}
	// 敵弾すべてについて
	for (EnemyBullet* eBullet : enemyBullets_)
	{
		colliders_.push_back(eBullet);
	}
	// リスト内のペアを総当り
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		Collider* colliderA = *itrA;
		// イテレータBはイテレータAの次の要素から回す (重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			Collider* colliderB = *itrB;
			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void GameScene::AddEnemyBullet(EnemyBullet* _enemyBullet)
{
	enemyBullets_.push_back(_enemyBullet);
	enemyBullets_.back()->SetPlayer(player_);
}

void GameScene::MakeEnemyInstance(Vector3 _pos)
{
	// 敵キャラの生成
	enemies_.push_back(new Enemy());

	// 敵キャラの初期化
	enemies_.back()->Initialize(model_, _pos, Vector3(0.0f, 0.0f, -0.05f));
	// 敵キャラにゲームシーンを渡す
	enemies_.back()->SetGameScene(this);
	enemies_.back()->SetPlayer(player_);
}

void GameScene::CheckCollisionPair(Collider* _colliderA, Collider* _colliderB)
{
	// 衝突フィルタリング
	if (
		!(_colliderA->GetCollisionAttribute() & _colliderB->GetCollisionMask()) ||
		!(_colliderB->GetCollisionAttribute() & _colliderA->GetCollisionMask())
		) return;

	Vector3 positionA = _colliderA->GetWorldPosition();
	Vector3 positionB = _colliderB->GetWorldPosition();
	// 判定に必要な前計算
	float radiusCompd = _colliderA->GetRadius() + _colliderB->GetRadius();
	Vector3 AtoBVect = positionB - positionA;
	float distAtoB = Length(AtoBVect);
	// 球と球の交差判定
	if (distAtoB < radiusCompd)
	{
		// コライダーAの衝突時コールバック呼出
		_colliderA->OnCollision();
		// コライダーBの衝突時コールバック呼出
		_colliderB->OnCollision();
	}
}
