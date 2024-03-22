#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DebugCamera.h>
#include"GameMap.h"
#include"Camera.h"
#include<memory>
#include"Enemy.h"
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{

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

		void CheckAllCollision();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	// 3Dモデル
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	ViewProjection viewProjection_;


	//プレイヤー
	std::unique_ptr<Player> player_;
	
	//敵
	std::unique_ptr<Enemy> enemy_;
	
	//カメラ
	bool isDebgCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
	std::unique_ptr<Camera> camera_;

	//マップ
	GameMap* gameMap_ = nullptr;

	int StageSwitching = false;
	int stage_ = 0;
};