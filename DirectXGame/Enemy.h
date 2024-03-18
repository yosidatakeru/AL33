#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Function.h"
#include"GameMap.h"
#include"Input.h"
class Enemy 
{
public:
	~Enemy();

	/// </summary>
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

private:

	Model* model_;
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0u;
	GameMap* gameMap_;

	// キーボード入力
	Input* input_ = nullptr;

	// ジャンプパラメータ
	bool jumpAction_ = false;
	float jumpSpeed = 0;
};
