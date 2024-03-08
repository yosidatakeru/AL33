#include "gameMap.h"
#include <Function.h>
#include <dinput.h>

GameMap::~GameMap() {}

void GameMap::Initialize(Model* model, uint32_t textureHandle) 
{ 
	this->model_ = model;
	this->texureHandle_ = textureHandle;
	//仮
	texureHandle_ = TextureManager::Load("uvChecker.png");
	


	//ステージ切り替え
	for (int y = 0; y < StageYMax; y++)
	{
		for (int x = 0; x < StageXMax; x++) 
		{
			switch (Map_) 
			{ 
				case 0:
				    mapDataBase[y][x] = mapData3[y][x];
				    mapDataBase2[y][x] = mapData2[y][x];
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
			if (mapData[y][x] == 1) 
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

