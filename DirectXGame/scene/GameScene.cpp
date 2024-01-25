#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

#include <imgui.h>


GameScene::GameScene() {}


GameScene::~GameScene()
{ delete titleSprite_;
	delete clearSprite_;
	delete explanationSprite_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

    worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	
	titleTexture = 0;
	titleTextureNumber_ = 1;
	titleTimeLimit = 240;
	titleTime = 240;
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

	//タイトル
	titletextureHandle_ = TextureManager::Load("title.png");
	titleSprite_ = Sprite::Create(titletextureHandle_, {100, 50,});

	eraseHandle_ = TextureManager::Load("title2.png");
	eraseSprite_ = Sprite::Create(eraseHandle_,{ 100,50,});

	
	//クリア画面
	 cleartureHandle_ = TextureManager::Load("clear.png");
	clearSprite_ = Sprite::Create(cleartureHandle_, {100, 50});
	
	loseHandle_ = TextureManager::Load("defeat.png");
	loseSprite_ = Sprite::Create(loseHandle_, {100, 50});
	//操作説明
	explanationHandle_ = TextureManager::Load("explanation.png");
	explanationSprite_ = Sprite::Create(explanationHandle_, {100, 50});

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

	TimeLimit = 1200;

}








void GameScene::Update() 
{

	switch (scene_) {
	case Scene::Title:
	default:
		TitleScene();
		break;

	case Scene::Ready:
		ReadyScene();
		
		break;

	case Scene::Game:
		GamePlayScene();
		break;

	case Scene::Result:
		ResultScene();
		break;

	case Scene::Win:
		WinScene();
		break;

	case Scene::Lose:

		LoseScene();
		break;
	}


	
} 
 
 
 
 
void  GameScene::Draw()
{ 
 
	 
	 // コマンドリストの取得
	 ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
 
#pragma region 背景スプライト描画
	 // 背景スプライト描画前処理
	 Sprite::PreDraw(commandList);

	  switch (draw2DObjectScene_) {
	 case Draw2DObjectScene::Title:

	 default:
		if (titleTexture == 0) 
		{

			if (titleTimeLimit >= 120) 
			{
				titleSprite_->Draw();
			} else {
				eraseSprite_->Draw();
			}
		} 
		else if (titleTime%8<=0)
		{
			eraseSprite_->Draw();
		}
		else 
		{
			titleSprite_->Draw();
		}
		


		break;

	 case Draw2DObjectScene::Ready:
		explanationSprite_->Draw();
		
		break;

	 case Draw2DObjectScene::Game:
		//GamePlayDraw2DObjectScene();
		break;

	 case Draw2DObjectScene::Result:
		//ResultDraw2DObjectScene();
		break;

	 case Draw2DObjectScene::Win:
		clearSprite_->Draw();
		 
		break;

	 case Draw2DObjectScene::Lose:
		loseSprite_->Draw();
		break;
	 }
	
	
	 
 
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
	 


	 switch (draw3DObjectScene_) {
	 case Draw3DObjectScene::Title:
		
	 default:
		TitleDraw3DObjectScene();
		break;

	 case Draw3DObjectScene::Ready:
		ReadyDraw3DObjectScene();
		break;

	 case Draw3DObjectScene::Game:
		GamePlayDraw3DObjectScene();
		break;

	 case Draw3DObjectScene::Result:
		ResultDraw3DObjectScene();
		break;

	 case Draw3DObjectScene::Win:
		WinDraw3DObjectScene();
		break;

	 case Draw3DObjectScene::Lose:
		LoseDraw3DObjectScene();
		break;
	 }
	
	
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

#pragma region アップデートの処理




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
		scene_ = Scene::Win;
		draw3DObjectScene_ = Draw3DObjectScene::Win;
		draw2DObjectScene_ = Draw2DObjectScene::Win;
		}
}




void GameScene::TitleScene() {
	    titleTimeLimit--;
	    if (titleTextureNumber_ == 1) 
		{

		if (input_->TriggerKey(DIK_SPACE)) 
		{
			titleTextureNumber_ = 2;
			titleTexture = 1;
		
		}
	    }
	   
		if (titleTimeLimit <= 0)
		{
		titleTimeLimit = 240;
		}

	    if (titleTexture == 1) 
		{
		titleTime--;
		if (titleTime <= 0) 
		{

			scene_ = Scene::Ready;
			draw3DObjectScene_ = Draw3DObjectScene::Ready;
			draw2DObjectScene_ = Draw2DObjectScene::Ready;
		}
		}

}




void GameScene::ReadyScene() 
{ 
	titleTexture = 0;
 Initialize();
	    
		scene_ = Scene::Game;
		draw3DObjectScene_ = Draw3DObjectScene::Game;
		draw2DObjectScene_ = Draw2DObjectScene::Game;
	    
}





void GameScene::GamePlayScene() 
{
	//死亡までのカウントダウン
	    TimeLimit--;
	    if (TimeLimit < 0)
		{
		scene_ = Scene::Lose;
		draw3DObjectScene_ = Draw3DObjectScene::Lose;
		draw2DObjectScene_ = Draw2DObjectScene::Lose;
	    }

	    titleTextureNumber_ = 2;
	    // 自キャラの更新
	    player_->Update();

	    // 敵
	    enemy_->Update();

	    debugCamera_->Update();

	    // 天球
	    skydome_->Update();

	    // 天球
	    ground_->Update();
	    // 敵
	    // enemy_->Update();

	    // 当たり判定
	    CheckAllisions();

	    // デバックの頭文字
	    if (input_->TriggerKey(DIK_Q)) {
		isDebgCameraActive_ = true;
	    }

	    if (isDebgCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

	    } else {
		// カメラ
		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProlection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProlection().matProjection;
		// ビュープロジェクション行列の更新と転送
		// viewProjection_.UpdateMatrix();
	    }
	    // ビュープロジェクション行列の転送
	    viewProjection_.TransferMatrix();
}




void GameScene::ResultScene() {}





void GameScene::WinScene() 
{
	    titleTextureNumber_ = 1;
	    if (input_->TriggerKey(DIK_SPACE)) 
		{

		// カウントダウンへ
		scene_ = Scene::Title;
		draw3DObjectScene_ = Draw3DObjectScene::Title;
		draw2DObjectScene_ = Draw2DObjectScene::Title;
	    }
}

void GameScene::LoseScene() 
{
	    titleTextureNumber_ = 1;
	    if (input_->TriggerKey(DIK_SPACE)) {

		// カウントダウンへ
		scene_ = Scene::Title;
		draw3DObjectScene_ = Draw3DObjectScene::Title;
		draw2DObjectScene_ = Draw2DObjectScene::Title;
	    }
}

void GameScene::TitleDraw3DObjectScene()
{ }

void GameScene::ReadyDraw3DObjectScene() {}

void GameScene::GamePlayDraw3DObjectScene() 
{
	    // 自キャラの描画
	    player_->Draw(viewProjection_);

	    // 敵
	    enemy_->Draw(viewProjection_);

	    // 天球
	    skydome_->Draw(viewProjection_);

	    // 地面
	    ground_->Draw(viewProjection_);

		
}

void GameScene::ResultDraw3DObjectScene() {}


void GameScene::WinDraw3DObjectScene() {}

void GameScene::LoseDraw3DObjectScene() {}

#pragma endregion