#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "RailCamera.h"
#include "EnemyBullet.h"

class Skydome;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void CheckAllCollisions();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// モデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	
	// 自キャラ
	Player* player_ = nullptr;

	// 敵キャラ
	Enemy* enemy_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// DebugCamera
	DebugCamera* debugCamera_ = nullptr;

	bool isDebugCameraActive_ = false;

	std::unique_ptr<Skydome> skydome_;

	RailCamera* railCamera_ = nullptr;

	std::list<EnemyBullet*> enemyBullets_;
};
