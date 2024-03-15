#include "Player.h"
#include "Function.h"
#include <ImguiManager.h>
#include <cassert>





// デストラクタ
Player::~Player() 
{
	
}


void Player::Initialize(Model* model, uint32_t textureHandle, GameMap* gameMap) {
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録する
	this->model_ = model;
	this->textureHandle_ = textureHandle;
	gameMap_ = gameMap;

	textureHandle_ = TextureManager::Load("Player6.png");
	goaltextureHandle_ = TextureManager::Load("goal.png");

	// ゴール仮
	sprite_ = Sprite::Create(goaltextureHandle_ ,{100,100});
	worldTransform_.Initialize();
	


	////ここの処理よくわからんから保留できれば一つにまとめたい
	//for (int  y = 0; y < gameMap_->GetStageYMax(); y++) 
	//{
	//	for (int x = 0; x < gameMap_->GetStageXMax(); x++) 
	//	{
	//		map[y][x] = map1[y][x];
	//	}
	//}

	//プレイヤー１の初期化

	for (int y = 0; y < gameMap_->GetStageYMax(); y++)
	{
		for (int x = 0; x < gameMap_->GetStageXMax(); x++) 
		{
			if (gameMap_->GetMapDataBase(y,x ) ==
			    6) 
			{
				worldTransform_.translation_.x = (float)x * 2;
				worldTransform_.translation_.y = (float)y * 2;
				break;	
			}
			
		}
		
	}

		// プレイヤー２の初期化
	for (int y = 0; y < gameMap_->GetStageYMax(); y++) 
	{
		for (int x = 0; x < gameMap_->GetStageXMax(); x++) 
		{

			if (gameMap_->GetMapDataBase(y, x) == 7) {
				worldTransformSecondPlayer_.translation_.x = (float)x * 2;
				worldTransformSecondPlayer_.translation_.y = (float)y * 2;
				break;
			}
		}
	}


	worldTransformSecondPlayer_.Initialize();
			
	 
	//ゴールフラグ初期化
	goalCheck = false;

	// シングルトンインスタンスを取得
	input_ = Input::GetInstance();
}




void Player::Update() 
{
	
	////キャラクターの移動ベクトル
	Vector3 move = {0.0, 0.0f, 0.0f};
	Vector3 move2 = {0.0, 0.0f, 0.0f};
	////キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;
	
	//当たり判定呼び出し
	CheckAllCollision();

	////押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) 
	{
		float playerPosx = worldTransform_.translation_.x - kCharacterSpeed;
		float playerPosy = worldTransform_.translation_.y;

		float secondPlayerPosx = worldTransformSecondPlayer_.translation_.x - kCharacterSpeed;
		float secondPlayerPosy = worldTransformSecondPlayer_.translation_.y;
		
		if (gameMap_->ChecMap(playerPosx, playerPosy) == false) 
		{
			move.x -= kCharacterSpeed;
			
		}
		if (gameMap_->ChecMap(secondPlayerPosx, secondPlayerPosy) == false)
		{
			move2.x -= kCharacterSpeed;
		}

	} else if (input_->PushKey(DIK_RIGHT)) 
	{
		float x = worldTransform_.translation_.x + kCharacterSpeed;
		float y = worldTransform_.translation_.y;

		float x2 = worldTransformSecondPlayer_.translation_.x + kCharacterSpeed;
		float y2 = worldTransformSecondPlayer_.translation_.y;
		
		if (gameMap_->ChecMap(x,y) == false) 
		{
			move.x += kCharacterSpeed;
			
		}

		if (gameMap_->ChecMap(x2, y2) == false) 
		{
		move2.x += kCharacterSpeed;
		}

	} else 
	{
		move.x = 0.0f;
		move.y = 0.0f;
		move2.x = 0.0f;
		move2.y = 0.0f;
	}

	 // ジャンプ開始
	if (jumpAction_ == false) 
	{

		if (input_->TriggerKey(DIK_SPACE))
		{
			jumpAction_ = true;
			jumpSpeed = 1;
			jumpActionSecondPlayer_ = true;
			jumpSpeedSecondPlayer = 1;
		}
	}
	Jump();
	JumpSecondPlayer();
	//マップ切り替え仮
	

		if (input_->TriggerKey(DIK_S) && StageSwitching == false )
		{
		float playerPosx = worldTransform_.translation_.x - kCharacterSpeed;
		float playerPosy = worldTransform_.translation_.y;

		float secondPlayerPosx = worldTransformSecondPlayer_.translation_.x - kCharacterSpeed;
		float secondPlayerPosy = worldTransformSecondPlayer_.translation_.y;
		
		if (gameMap_->ChecNextMap(playerPosx,playerPosy)==false &&gameMap_->ChecNextMap(secondPlayerPosx,secondPlayerPosy)==false)
		{

			stage_ = 1;
			StageSwitching = true;
			gameMap_->Stage(stage_);
		}
	    
		} else if (input_->TriggerKey(DIK_S))
		{
		float playerPosx = worldTransform_.translation_.x - kCharacterSpeed;
		float playerPosy = worldTransform_.translation_.y;

		float secondPlayerPosx = worldTransformSecondPlayer_.translation_.x - kCharacterSpeed;
		float secondPlayerPosy = worldTransformSecondPlayer_.translation_.y;
		if (gameMap_->ChecNextMap(playerPosx, playerPosy) == false &&
		    gameMap_->ChecNextMap(secondPlayerPosx, secondPlayerPosy)==false) {
			stage_ = 0;
			StageSwitching = false;
			gameMap_->Stage(stage_);
		}
		}
	
	// 移動の限界
	

	
	// 座標移動（ベクトルの加算）
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	////平行移動行列

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
	
	
	//
	//// 座標移動（ベクトルの加算）
	worldTransformSecondPlayer_.translation_ = Add(worldTransformSecondPlayer_.translation_, move2);
	//////平行移動行列

	worldTransformSecondPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformSecondPlayer_.scale_, worldTransformSecondPlayer_.rotation_, worldTransformSecondPlayer_.translation_);

	worldTransformSecondPlayer_.TransferMatrix();
	//
	//弾の寿命
	


	// 画面に座標を出す
	ImGui::Begin("Player");

	ImGui::InputFloat3("PlayerPosition", &worldTransform_.translation_.x);
	ImGui::SliderFloat3("PlayerSlide", &worldTransform_.translation_.x, -20.0f, 30.0f);
	ImGui::InputFloat3("Playerジャンプn", &jumpSpeed);
	ImGui::SliderFloat3("Playerzyannpu", &jumpSpeed, -20.0f, 30.0f);
	ImGui::End();
}



void Player::Draw(ViewProjection& viewProjection_) 
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	model_->Draw(worldTransformSecondPlayer_, viewProjection_, textureHandle_);
	
	
}

void  Player::Draw2D() 
{
	if (goalCheck == true)
	{
		     sprite_->Draw(); 
	}
}







void Player::Jump() 
{
	           

	            // 下にブロックがないとき落下開始

	            // ジャンプ実装
	            if (jumpAction_ == false)
				{
		          float x = worldTransform_.translation_.x;
		          float y = worldTransform_.translation_.y - 0.1f;
		        
		          if (gameMap_->ChecMap(x, y) == false) 
			      {
			       jumpAction_ = true;
			       jumpSpeed = 0;
		          }
	            }

	            if (jumpAction_ == true) 
				{

		         if (jumpSpeed > 0) 
				 {

			       for (float i = 0; i < jumpSpeed; i += 0.1f)
				   {
				     float x = worldTransform_.translation_.x;
				     float y = worldTransform_.translation_.y + 0.1f;
				     if (gameMap_->ChecMap(x, y) == false) 
				     {
					     worldTransform_.translation_.y += 0.1f;
					 } else 
				     {
					 jumpSpeed = 0;
					 break;
				     }
			       }

		        } else // 下降
		          {
			        for (float i = jumpSpeed; i < 0; i += 0.1f) 
					{
				      float x = worldTransform_.translation_.x;
				      float y = worldTransform_.translation_.y - 0.1f;
				      if (gameMap_->ChecMap(x, y) == false) 
					  {
					     worldTransform_.translation_.y -= 0.1f;
				       } else 
					  {
					   jumpAction_ = false;
					   break;
					  }
					}
		           }  
				    jumpSpeed -= 0.05f;
	            }

}

void Player::JumpSecondPlayer() 
{
	            // ジャンプ実装
	            if (jumpActionSecondPlayer_ == false) {
		            float x = worldTransformSecondPlayer_.translation_.x;
		            float y = worldTransformSecondPlayer_.translation_.y - 0.1f;

		            if (gameMap_->ChecMap(x, y) == false) {
			        jumpActionSecondPlayer_ = true;
			        jumpSpeedSecondPlayer= 0;
		            }
	            }

	            if (jumpActionSecondPlayer_ == true) {

		            if (jumpSpeedSecondPlayer > 0) {

			        for (float i = 0; i < jumpSpeedSecondPlayer; i += 0.1f) 
					{
				      float x = worldTransformSecondPlayer_.translation_.x;
				      float y = worldTransformSecondPlayer_.translation_.y + 0.1f;
				      if (gameMap_->ChecMap(x, y) == false) {
					   worldTransformSecondPlayer_.translation_.y += 0.1f;
				      } else {
					   jumpSpeedSecondPlayer = 0;
					   break;
				      }
			        }

		            } else // 下降
		            {
			        for (float i = jumpSpeedSecondPlayer; i < 0; i += 0.1f) {
				      float x = worldTransformSecondPlayer_.translation_.x;
				      float y = worldTransformSecondPlayer_.translation_.y - 0.1f;
				      if (gameMap_->ChecMap(x, y) == false) {
					   worldTransformSecondPlayer_.translation_.y -= 0.1f;
				      } else {
					   jumpActionSecondPlayer_ = false;
					   break;
				      }
			        }
		            }
		            jumpSpeedSecondPlayer -= 0.05f;
	            }
}


//プレイヤー同士の当たり判定
void Player::CheckAllCollision() 
{
	Vector3 posA;
    Vector3 posB;
	           
	posA = GetWorldPositionSecondPlayer();
	posB = GetWorldPosition();

	float preyerAB =   (posB.x - posA.x) * (posB.x - posA.x) +
	                   (posB.y - posA.y) * (posB.y - posA.y) +
	                   (posB.z - posA.z) * (posB.z - posA.z);

	float RadiusAB = (radius_ + radius_) + (radius_ + radius_);


	if (preyerAB <= RadiusAB) 
	{
		            goalCheck = true;
	}
}

Vector3 Player::GetWorldPosition() {
	            Vector3 worldPos;

	            worldPos.x = worldTransform_.matWorld_.m[3][0];
	            worldPos.y = worldTransform_.matWorld_.m[3][1];
	            worldPos.z = worldTransform_.matWorld_.m[3][2];
	            return worldPos;
}

Vector3 Player::GetWorldPositionSecondPlayer()
{ 
	Vector3 worldPosSecondPlayer;
	
	worldPosSecondPlayer.x = worldTransformSecondPlayer_.translation_.x;
	worldPosSecondPlayer.y = worldTransformSecondPlayer_.translation_.y;
	worldPosSecondPlayer.z = worldTransformSecondPlayer_.translation_.z;


	return worldPosSecondPlayer;
}

