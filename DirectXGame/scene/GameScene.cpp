#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() 
{

}

GameScene::~GameScene()
{
	delete model_;

		

	
}

void GameScene::Initialize() 
{

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	

	
	// 3Dモデルの生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();




	// マップの初期化
	gameMap_ = new GameMap();
	gameMap_->Initialize(model_, textureHandle_);


	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_,gameMap_);


	// 自キャラの生成
	enemy_ = std::make_unique<Enemy>();
	// 自キャラの初期化
	enemy_->Initialize(model_, textureHandle_, gameMap_);

	textureHandleTitle_ = TextureManager::Load("kuttukero title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, { 0.0f, 0 });

	textureHandleTutorial_ = TextureManager::Load("kuttukero_Rule.png");
	spriteTutorial_ = Sprite::Create(textureHandleTutorial_, { 0.0f, 0 });

	
	// デバックカメラの生成
	debugCamera_ = new DebugCamera(720, 1280);
	Vector3 rotation = {0.0f, 0.0f, 0.0f};
	camera_ = std::make_unique<Camera>();
	camera_->Initialize(player_->GetWorldPosition(), rotation);
	 
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	
}


void GameScene::Update()
{
	switch ((sceneMode_)) {
	case 0:
		GamePlayUpdate();
		break;
	case 1:
		TutorialUpdate();
		break;
	case 2:
		GamePlayUpdate();
		break;
	}

	//// 自キャラの更新
	//player_->Update();
	////敵の更新
	////enemy_->Update();

	//gameMap_->Update();
	//debugCamera_->Update();


	//// カメラの処理
	//if (gameMap_->ChecNextMap(player_->GetWorldPos().x, player_->GetWorldPos().y) == true &&
	//	gameMap_->ChecNextMap(player_->GetWorldPositionSecondPlayer().x,player_->GetWorldPositionSecondPlayer().y)==true&&
	//    camera_->GetWorldTransform().translation_.y<=56.0f)
	//{
	//	
	//	camera_->GetWorldTransform().translation_.y++;

	//		
	//	//56.0fに行ったらマップウを切り替え処理する

	//}
	//if (camera_->GetWorldTransform().translation_.y >= 56.0f)
	//{
	//	camera_->GetWorldTransform().translation_.y = 18.0f;
	//	int i = gameMap_->GetMap();
	//	i += 1;
	//	gameMap_->SetMap(i);
	//	player_->Initialize(model_, textureHandle_, gameMap_);
	//	gameMap_->Initialize(model_, textureHandle_);

	//}

	//リセット
	if (input_->TriggerKey(DIK_S)) 
	{
		player_->Initialize(model_, textureHandle_, gameMap_);
	}



	

	#ifdef _DEBUG
	// デバックの頭文字
	if (input_->TriggerKey(DIK_Q)) 
	{
		isDebgCameraActive_ = true;
	}

#endif
	
		//camera_->Update();
		//viewProjection_.matView = camera_->GetViewProjection().matView;
		//viewProjection_.matProjection = camera_->GetViewProjection().matProjection;

		//// ビュープロジェクション行列の転送
		//viewProjection_.TransferMatrix();
	
		////敵とプレイヤーの当たり判定
	 //   CheckAllCollision();

		

}

void GameScene::TitleUpdate() {

	if (input_->TriggerKey(DIK_RETURN)) {
		sceneMode_ = 1;
	}

}

void GameScene::TutorialUpdate() {

	if (input_->TriggerKey(DIK_RETURN)) {
		sceneMode_ = 2;
	}

}

void GameScene::GamePlayUpdate() {

	// 自キャラの更新
	player_->Update();
	//敵の更新
	//enemy_->Update();

	gameMap_->Update();
	debugCamera_->Update();


	// カメラの処理
	if (gameMap_->ChecNextMap(player_->GetWorldPos().x, player_->GetWorldPos().y) == true &&
		gameMap_->ChecNextMap(player_->GetWorldPositionSecondPlayer().x,player_->GetWorldPositionSecondPlayer().y)==true&&
	    camera_->GetWorldTransform().translation_.y<=56.0f)
	{
		
		camera_->GetWorldTransform().translation_.y++;

			
		//56.0fに行ったらマップウを切り替え処理する

	}
	if (camera_->GetWorldTransform().translation_.y >= 56.0f)
	{
		camera_->GetWorldTransform().translation_.y = 18.0f;
		int i = gameMap_->GetMap();
		i += 1;
		gameMap_->SetMap(i);

		player_->Initialize(model_, textureHandle_, gameMap_);
		gameMap_->Initialize(model_, textureHandle_);

	}


	camera_->Update();
		viewProjection_.matView = camera_->GetViewProjection().matView;
		viewProjection_.matProjection = camera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	
		//敵とプレイヤーの当たり判定
	    CheckAllCollision();
	   // int i = gameMap_->GetMap();
}

void GameScene::DrawTitle() {

	spriteTitle_->Draw();

}

void GameScene::DrawTutorial() {

	spriteTutorial_->Draw();
}

void GameScene::Draw() 
{

	
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	
	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	gameMap_->Draw2D(); 
	
	player_->Draw2D();
	
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	// 自キャラの描画
	player_->Draw(viewProjection_);

	//敵の描画
//	enemy_->Draw(viewProjection_);

	//マップの描画
	gameMap_->Draw(viewProjection_);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);
	
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	
	if (sceneMode_ == 0) {
		DrawTitle();
	}

	if (sceneMode_ == 1) {
		DrawTutorial();
	}

	/// </summary>
	
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
// 敵とプレイヤーの当たり判定
void GameScene::CheckAllCollision() 
{ 
	Vector3 playerposA = player_->GetWorldPos();
	Vector3 playerposB = player_->GetWorldPositionSecondPlayer();
	Vector3 enemypos = enemy_->GetWorldPosition();

	float preyerA = (enemypos.x - playerposA.x) * (enemypos.x - playerposA.x) +
	                 (enemypos.y - playerposA.y) * (enemypos.y - playerposA.y) +
	                 (enemypos.z - playerposA.z) * (enemypos.z - playerposA.z);

	float RadiusA = (player_->GetRadius() + enemy_->GetRadius() + (player_->GetRadius() + enemy_->GetRadius()));

	if (preyerA <= RadiusA) 
	{
		player_->palyerReset();
		enemy_->enenmyReset();

	}

	float preyerB = (enemypos.x - playerposB.x) * (enemypos.x - playerposB.x) +
	                (enemypos.y - playerposB.y) * (enemypos.y - playerposB.y) +
	                (enemypos.z - playerposB.z) * (enemypos.z - playerposB.z);

	float RadiusB =
	    (player_->GetRadius() + enemy_->GetRadius() + (player_->GetRadius() + enemy_->GetRadius()));

	if (preyerB <= RadiusB)
	{
		player_->palyerReset();
		enemy_->enenmyReset();
	}

}



