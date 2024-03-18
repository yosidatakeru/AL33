#include "gameMap.h"
#include <Function.h>
#include <dinput.h>

GameMap::~GameMap() {}

void GameMap::Initialize(Model* model, uint32_t textureHandle) 
{ 
	this->model_ = model;
	this->texureHandle_ = textureHandle;
	//仮
	texureHandle_ = TextureManager::Load("block.png");
	redblocktexureHandle_ = TextureManager::Load("redblock.png");
	blueblooktexureHandle_ = TextureManager::Load("blueblook.png");
	Block_OFF2texureHandle_ = TextureManager::Load("Block_OFF2.png");
	Block_ON2texureHandle_ = TextureManager::Load("Block_ON2.png");
	//ステージ切り替え
	for (int y = 0; y < StageYMax; y++)
	{
		for (int x = 0; x < StageXMax; x++) 
		{
			switch (Map_) 
			{ 
				case 0:
				mapDataBase[y][x] = tutorial1[y][x];
				mapDataBase2[y][x] = tutorial2[y][x];
				break;


				case 1:
				mapDataBase[y][x] = stage1Map1[y][x];
				mapDataBase2[y][x] = stage1Map2[y][x];
				break;

				case 2:
				mapDataBase[y][x] = stage2Map1[y][x];
				mapDataBase2[y][x] = stage2Map2[y][x];
				break;

				case 3:
				mapDataBase[y][x] = stage3Map1[y][x];
				mapDataBase2[y][x] = stage3Map2[y][x];
				break;

				case 4:
				mapDataBase[y][x] = stage4Map1[y][x];
				mapDataBase2[y][x] = stage4Map2[y][x];
				break;
			}	
			
		}
	}

	//初期化処理
	Stage(stage_); 


	//ワールドトランスホームの初期化
	for (int y = 0; y < StageYMax ; y++)
	{
		for (int x = 0; x < StageXMax ; x++)
		{
			worldTransform_[y][x].translation_.x = (float)x * 2;
			worldTransform_[y][x].translation_.y = (float)y * 2;
			worldTransform_[y][x].Initialize();
			
			 
		
			
		    
		}
	}
	
	

	}



void GameMap::Update() 
{
	

	for (int y = 0; y < StageYMax; y++) 
	{
		for (int x = 0; x < StageXMax; x++) 
		{
			    worldTransform_[y][x].matWorld_ = MakeAffineMatrix(
			    worldTransform_[y][x].scale_, worldTransform_[y][x].rotation_,
			    worldTransform_[y][x].translation_);
				worldTransform_[y][x].TransferMatrix();	

				
		}
	}
}





void GameMap::Draw(ViewProjection& viewProjection_) 
{
	for (int y = 0; y < StageYMax; y++) 
	{ 
		for (int x = 0; x < StageXMax; x++) 
		{
			if (mapData[y][x] == 1) 
			{
				model_->Draw(worldTransform_[y][x], viewProjection_, texureHandle_);
			}		
			
			if (mapData[y][x] == 2) 
			{
				model_->Draw(worldTransform_[y][x], viewProjection_, redblocktexureHandle_);
			}		

			if (mapData[y][x] == 4) 
			{
				model_->Draw(worldTransform_[y][x], viewProjection_, Block_ON2texureHandle_);
			}		

			if (mapData[y][x] == 3) 
			{
				model_->Draw(worldTransform_[y][x], viewProjection_, blueblooktexureHandle_);
			}		
			
			if (mapData[y][x] == 5) {
				model_->Draw(worldTransform_[y][x], viewProjection_, Block_OFF2texureHandle_);
			}		

		}
	}
}



//当たり判定マップ
bool GameMap::ChecMap(float px, float py) 
{
	for (int y = 0; y < StageYMax; y++) 
	{
		for (int x = 0; x < StageXMax; x++)
		{
			if (mapData[y][x] == 1 || mapData[y][x] == 2 || mapData[y][x] ==3) 
			{
				float x2 = worldTransform_[y][x].translation_.x;
				float y2 = worldTransform_[y][x].translation_.y;
				if (abs(x2 - px) < 2.0f && abs(y2 - py) < 2.0f)
				{
					return true;
				}
			}
		}
	}
	
	return false;
}

//マップ切り替え際今いる場所にぶっろくがあるかどうかの確認
bool GameMap::ChecNextMap(float px, float py) 
{
	for (int y = 0; y < StageYMax; y++) {
		for (int x = 0; x < StageXMax; x++) {
			if (mapData[y][x] == 4 || mapData[y][x] == 5 ) 
			{
				float x2 = worldTransform_[y][x].translation_.x;
				float y2 = worldTransform_[y][x].translation_.y;
				if (abs(x2 - px) < 2.0f && abs(y2 - py) < 2.0f)
				{
					return true;
				}
			}
		}
	}
	return false;

}


//ステージ切り替え用
void GameMap::Stage(int stage) 
{ 
	stage_ = stage; 
	for (int y = 0; y < StageYMax; y++) 
	{
		for (int x = 0; x < StageXMax; x++) 
		{

			switch (stage_) 
			{
			case 0:
				mapData[y][x] = mapDataBase[StageYMax - y - 1][x];
				break;

			case 1:
				mapData[y][x] = mapDataBase2[StageYMax - y - 1][x];
				
				break;
			}
		}
	}
}

