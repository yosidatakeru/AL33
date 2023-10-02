#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene()
{
	
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	

	

	
	// 3Dモデルの生成
	playerModel_.reset(Model::Create());
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize(playerModel_.get(), textureHandle_);

	

	//敵/////////////
//const float kEnemySpeed = -0.5f;
//	Vector3 velocity(0, 0, kEnemySpeed);
//	Vector3 enemyPosition(0, 1.0, 100);
//
//	//enemyModel_ = Model::Create();
//	//enemy_ = new Enemy();
//	//enemy_->Initialize(enemyModel_, enemyPosition, velocity);
	/////////////////////////

	 //デバックカメラの生成
	debugCamera_ = new DebugCamera(720, 1280);

	// 軸方向表示の表示を有効化する
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);


}








void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	debugCamera_->Update();

	// 敵
	/*enemy_->Update();*/


	#ifdef _DEBUG
	// デバックの頭文字
	if (input_->TriggerKey(DIK_Q)) {
		isDebgCameraActive_ = true;
	}

#endif
	if (isDebgCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}


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

	// 敵の描画
	/*enemy_->Draw(viewProjection_);*/
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

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}