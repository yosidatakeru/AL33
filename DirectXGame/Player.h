#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Function.h"
#include "Input.h"
#include"GameMap.h"
#include<list>
class Player {

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	/// <summary>
	/// 自キャラ
	/// </summary>
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, GameMap* gameMap);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection_);

	
	void Jump();

	
	Vector3 GetWorldPosition();

	

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform worldTransformSecondPlayer_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Player* player_ = nullptr;
	// キーボード入力
	Input* input_ = nullptr;
	GameMap* gameMap_ = nullptr;
	int stage_ = 0;
	//ジャンプパラメータ
	bool jumpAction_ = false;
	float jumpSpeed = 0;

	int StageSwitching = false;
};