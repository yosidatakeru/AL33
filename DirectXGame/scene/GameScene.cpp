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

    worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	

	
	
	//newの代わり
	// 自キャラの生成
	player_ = std::make_unique<Player>();
    //enemy_->Initialize();
	// テクスチャ読み込み
	//プレイヤーの場合テクスチャの読み取りが必要
	textureHandle_ = TextureManager::Load("Player/tex.png");
	// 3Dモデルの生成
    modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
    std::vector<Model*> playerModels = {
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get()};


	// 自キャラの初期化
	player_->Initialize(playerModels);

	



	//敵
	enemy_ = std::make_unique<Enemy>();

	enemyModel_.reset(Model::CreateFromOBJ("enem", true));
	
	 std::vector<Model*> enemyModels = {enemyModel_.get()};

	 enemy_->Initialize(enemyModels);




	// 生成
	skydome_ = std::make_unique<Skydome>();

	// フォルダの名前を指定してね

	skydomeModel_.reset(Model::CreateFromOBJ("CelestialSphere", true));

	
	// 天球の初期化
	skydome_->Initialize(skydomeModel_.get(), skydomeTextureHandle_);



	// 生成
	ground_ = std::make_unique<Ground>();

	// フォルダの名前を指定してね

	gronudModel_.reset(Model::CreateFromOBJ("Ground", true));

	// テクスチャ読み込み
	
	// 地面の初期化
	ground_->Initialize(gronudModel_.get(), GroundTextureHandle_);




	// ビュープロジェクション
	// forZを適度に大きい値に変更する
	// 大きくしすぎるとZファイティングになるよ
	viewProjection_.farZ = 1200.0f;
	// 初期化
	viewProjection_.Initialize();





	 //デバックカメラの生成
	debugCamera_ = new DebugCamera(720, 1280);



	//追従カメラ

	followCamera_ = std::make_unique<FollowCamera>();

	followCamera_->Initialize();

	player_->SetViewProjection(&followCamera_->GetViewProlection());
	//自キャラのワールドトランスフォームを追従カメラセット
	followCamera_->SetTarget(&player_->GetWorldTransform());	
	
	
	
	// 軸方向表示の表示を有効化する
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);


}








void GameScene::Update() 
{
	// 自キャラの更新
	player_->Update();

	//敵
	enemy_->Update();

	debugCamera_->Update();

	//天球
	skydome_->Update();

	// 天球
	ground_->Update();
	// 敵
	//enemy_->Update();
	
	//当たり判定
	CheckAllisions();

	
	// デバックの頭文字
	 if (input_->TriggerKey(DIK_Q)) 
	 {
	 	isDebgCameraActive_ = true;
	 }
 
	 if (isDebgCameraActive_)
	 {
	 	debugCamera_->Update();
	 	viewProjection_.matView = debugCamera_->GetViewProjection().matView;
	 	viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
	 	
	 } else 
	 {
		// カメラ
		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProlection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProlection().matProjection;
	 	// ビュープロジェクション行列の更新と転送
	 	//viewProjection_.UpdateMatrix();
	 }
	 // ビュープロジェクション行列の転送
	 viewProjection_.TransferMatrix();
 
} 
 
 
 
 
 
 
 
 
 
 
void  GameScene::Draw()
{ 
 
	 
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
 
     //敵
	 enemy_->Draw(viewProjection_);

	 //天球
	 skydome_->Draw(viewProjection_);
 
	 //地面
	 ground_->Draw(viewProjection_);
	
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








// 当たり判定
void GameScene::CheckAllisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;
#pragma region 自キャラと敵弾の当たり判定

	// 自キャラの座標
	posA = player_->GetWorldPosition();
	posB = enemy_->GetWorldPosition();
	// 自キャラと敵弾全ての当たり判定
	
		float distanceAB = (posB.x - posA.x) * (posB.x - posA.x) +
	                   (posB.y - posA.y) * (posB.y - posA.y) +
	                   (posB.z - posA.z) * (posB.z - posA.z);

	
		float RadiusAB = (player_->GetRadius() + enemy_->GetRadius()) +
		                 (player_->GetRadius() + enemy_->GetRadius());


		if (distanceAB <= RadiusAB)
		{
		player_->OnCollsion();
		}
	}
