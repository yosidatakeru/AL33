#include "Enemy.h"

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, uint32_t textureHandle, GameMap* gameMap) {
	// 引数として受け取ったデータをメンバ変数に記録する
	this->model_ = model;
	this->textureHandle_ = textureHandle;
	gameMap_ = gameMap;

	
	textureHandle_ = TextureManager::Load("uvChecker.png");

		
	enemyWorldTransform_.Initialize();

	// プレイヤー１の初期化

	for (int y = 0; y < gameMap_->GetStageYMax(); y++) 
	{
		for (int x = 0; x < gameMap_->GetStageXMax(); x++) 
		{
			if (gameMap_->GetMapDataBase(y, x) == 8)
			{
				enemyWorldTransform_.translation_.x = (float)x * 2;
				enemyWorldTransform_.translation_.y = (float)y * 2;
				break;
			}
		}
	}

	// シングルトンインスタンスを取得
	input_ = Input::GetInstance();
}

void Enemy::Update()
{
	
	////キャラクターの移動ベクトル
	Vector3 move = {0.0, 0.0f, 0.0f};

	////キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;

	// 当たり判定呼び出し
	//CheckAllCollision();

	////押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) {
		float EnemyPosx = enemyWorldTransform_.translation_.x - kCharacterSpeed;
		float EnemyPosy = enemyWorldTransform_.translation_.y;

		if (gameMap_->ChecMap(EnemyPosx, EnemyPosy) == false) 
		{
			move.x -= kCharacterSpeed;
		}
		
		

	} else if (input_->PushKey(DIK_RIGHT)) 
{
		float x = enemyWorldTransform_.translation_.x + kCharacterSpeed;
		float y = enemyWorldTransform_.translation_.y;

		
		if (gameMap_->ChecMap(x, y) == false)
		{
			move.x += kCharacterSpeed;
		}

		
		

	} else {
		move.x = 0.0f;
		move.y = 0.0f;
		
		
	}

	// ジャンプ開始
	if (jumpAction_ == false )
	{

		if (input_->TriggerKey(DIK_SPACE)) 
		{
			jumpAction_ = true;
			jumpSpeed = 1;
			
			
		}
	}
	Jump();
	

	
	// 座標移動（ベクトルの加算）
	enemyWorldTransform_.translation_ = Add(enemyWorldTransform_.translation_, move);
	////平行移動行列

	enemyWorldTransform_.matWorld_ = MakeAffineMatrix(
	    enemyWorldTransform_.scale_, enemyWorldTransform_.rotation_, enemyWorldTransform_.translation_);

	enemyWorldTransform_.TransferMatrix();

	
	


}

void Enemy::Draw(ViewProjection& viewProjection_) 
{
	model_->Draw(enemyWorldTransform_, viewProjection_, textureHandle_);
}

void Enemy::Jump() 
{

	// 下にブロックがないとき落下開始

	// ジャンプ実装
	if (jumpAction_ == false) {
		float x = enemyWorldTransform_.translation_.x;
		float y = enemyWorldTransform_.translation_.y - 0.1f;

		if (gameMap_->ChecMap(x, y) == false) {
			jumpAction_ = true;
			jumpSpeed = 0;
		}
	}

	if (jumpAction_ == true) {

		if (jumpSpeed > 0) {

			for (float i = 0; i < jumpSpeed; i += 0.1f) {
				float x = enemyWorldTransform_.translation_.x;
				float y = enemyWorldTransform_.translation_.y + 0.1f;
				if (gameMap_->ChecMap(x, y) == false) {
					enemyWorldTransform_.translation_.y += 0.1f;
				} else {
					jumpSpeed = 0;
					break;
				}
			}

		} else // 下降
		{
			for (float i = jumpSpeed; i < 0; i += 0.1f) {
				float x = enemyWorldTransform_.translation_.x;
				float y = enemyWorldTransform_.translation_.y - 0.1f;
				if (gameMap_->ChecMap(x, y) == false) {
					enemyWorldTransform_.translation_.y -= 0.1f;
				} else {
					jumpAction_ = false;
					break;
				}
			}
		}
		jumpSpeed -= 0.05f;
	}
}

void Enemy::enenmyReset() 
{
	for (int y = 0; y < gameMap_->GetStageYMax(); y++) 
	{
		for (int x = 0; x < gameMap_->GetStageXMax(); x++) 
		{
			if (gameMap_->GetMapDataBase(y, x) == 8) 
			{
				enemyWorldTransform_.translation_.x = (float)x * 2;
				enemyWorldTransform_.translation_.y = (float)y * 2;
				break;
			}
		}
	}
}

Vector3 Enemy::GetWorldPosition() 
{
	Vector3 worldPos;

	worldPos.x = enemyWorldTransform_.translation_.x;
	worldPos.y = enemyWorldTransform_.translation_.y;
	worldPos.z = enemyWorldTransform_.translation_.z;

	return worldPos;
}

Vector3 Enemy::GetWorldPos() 
{
	Vector3 worldPos;

	worldPos.x = enemyWorldTransform_.matWorld_.m[3][0];
	worldPos.y = enemyWorldTransform_.matWorld_.m[3][1];
	worldPos.z = enemyWorldTransform_.matWorld_.m[3][2];

	return worldPos;
}




