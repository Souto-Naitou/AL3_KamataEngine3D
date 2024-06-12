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
#include <sstream>

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

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	void CheckAllCollisions();

	void AddEnemyBullet(EnemyBullet* _enemyBullet);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	uint32_t reticleHandle_ = 0u;

	// モデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	
	// 自キャラ
	Player* player_ = nullptr;

	// 敵キャラ
	std::list<Enemy*> enemies_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// DebugCamera
	DebugCamera* debugCamera_ = nullptr;

	bool isDebugCameraActive_ = false;

	// 天球
	std::unique_ptr<Skydome> skydome_;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	// 敵弾
	std::list<EnemyBullet*> enemyBullets_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	bool isWaitEnemy = false;
	int32_t enemyWaitTime = 0;

private: // メンバ関数
	void MakeEnemyInstance(Vector3 _pos);
};
