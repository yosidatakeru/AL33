#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Function.h"
#include "Input.h"
#include"GameMap.h"
#include<list>
#include"Sprite.h"
#include"Enemy.h"

 class Player   
 {

	
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	/// <summary>
	/// 自キャラ
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
	void Draw2D();
	
	void Jump();
	// プレイヤー２のパラメータ
	void JumpSecondPlayer();
	
	//プレイヤー同士の当たり判定
	void CheckAllCollision();

	float GetRadius() const { return radius_; }
	

	Vector3 GetWorldPosition();
	Vector3 GetWorldPos();

	Vector3 GetWorldPositionSecondPlayer();

	void palyerReset();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	//プレイヤー２のパラメータ
	WorldTransform worldTransformSecondPlayer_;
	
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//ゴール用
	Sprite* sprite_ = nullptr;
	uint32_t goaltextureHandle_ = 0u;




	Player* player_ = nullptr;
	
	Enemy* enenmy = nullptr;

	//プレイヤーのラディウス(当たり判定用)
	const float_t radius_ = 0.2f;
	// キーボード入力
	Input* input_ = nullptr;
	GameMap* gameMap_ = nullptr;
	
	//ジャンプパラメータ
	bool jumpAction_ = false;
	float jumpSpeed = 0;

	//プレイヤー２のパラメータ
	bool jumpActionSecondPlayer_ = false;
	float jumpSpeedSecondPlayer = 0;
	


	//ゴール用のフラグ確認用

	bool goalCheck = false;


};