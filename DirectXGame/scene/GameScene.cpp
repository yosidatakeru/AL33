#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene()
{
	
	 
	
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

	
	// デバックカメラの生成
	debugCamera_ = new DebugCamera(720, 1280);
	Vector3 rotation = {0.0f, 0.0f, 0.0f};
	camera_ = std::make_unique<Camera>();
	camera_->Initialize(player_->GetWorldPosition(), rotation);
	//railCamera_->Initialize(player_->GetWorldPosition(), rotation);

	// 軸方向表示の表示を有効化する
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);


}

void GameScene::Update()
{
	// 自キャラの更新
	player_->Update();
	//敵の更新
	enemy_->Update();

	gameMap_->Update();
	debugCamera_->Update();

	

	#ifdef _DEBUG
	// デバックの頭文字
	if (input_->TriggerKey(DIK_Q)) {
		isDebgCameraActive_ = true;
	}

#endif
	//if (isDebgCameraActive_) 
	//{
		camera_->Update();
		viewProjection_.matView = camera_->GetViewProjection().matView;
		viewProjection_.matProjection = camera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	//} else {
		//railCamera_->Update();
		
		//viewProjection_.UpdateMatrix();
	//}


}

void GameScene::Draw() {

	;
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);


	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

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
	enemy_->Draw(viewProjection_);

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
	/// </summary>
	player_->Draw2D();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}