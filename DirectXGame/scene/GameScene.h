#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include"Obj/Player/Player.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DebugCamera.h>
#include"Obj/Player/Enemy.h"
#include"Obj/Skydome/Skydome.h"
#include "Obj/Ground/Ground.h"

#include <memory>
#include <Obj/Camera/FollowCamera.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene 
{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//当たり判定
	void CheckAllisions();


	// Update用
	void TitleScene();

	void ReadyScene();

	void GamePlayScene();

	void ResultScene();

	void WinScene();

	void LoseScene();



	// 3DObject
	void TitleDraw3DObjectScene();

	void ReadyDraw3DObjectScene();

	void GamePlayDraw3DObjectScene();

	void ResultDraw3DObjectScene();

	void WinDraw3DObjectScene();

	void LoseDraw3DObjectScene();



private: // メンバ変数

	int32_t titleTextureNumber_ = 0;

	int32_t titleTexture =0 ;
	ViewProjection viewProjection_;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	
	
	
   uint32_t textureHandle_ = 0;

	uint32_t titletextureHandle_ = 0;
 
	Sprite* titleSprite_ = nullptr;

	uint32_t eraseHandle_ = 0;

	Sprite* eraseSprite_ = nullptr;

	uint32_t clearHandle_ = 0;

	uint32_t cleartureHandle_ = 0;

	Sprite* clearSprite_ = nullptr;

	uint32_t explanationHandle_ = 0;

	Sprite* explanationSprite_ = nullptr;

	uint32_t loseHandle_ = 0;

	Sprite* loseSprite_ = nullptr;

	int32_t TimeLimit = 1200;

	int32_t titleTimeLimit = 240;
	
	int32_t titleTime = 240;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
	// 3Dモデル
	
	#pragma region プレイヤー
   std::unique_ptr<Model> playerModel_;
   std::unique_ptr<Player> player_;
   std::unique_ptr<Model> modelFighterBody_ = nullptr;
   std::unique_ptr<Model> modelFighterHead_ = nullptr;
   std::unique_ptr<Model> modelFighterL_arm_ = nullptr;
   std::unique_ptr<Model> modelFighterR_arm_ = nullptr;
#pragma endregion



	#pragma region 敵
   std::unique_ptr<Model> enemyModel_=nullptr;
   std::unique_ptr<Enemy> enemy_;
	
   

	#pragma endregion





	#pragma region 天球のメンバ変数
	// テクスチャハンドル
	uint32_t skydomeTextureHandle_ = 0u;

	// 3Dモデル
	std::unique_ptr<Model> skydomeModel_;


	// 天球ポインタ
	std::unique_ptr<Skydome> skydome_;

	#pragma endregion





#pragma region 地面
//  テクスチャハンドル
uint32_t GroundTextureHandle_ = 0u;


	
// 3Dモデル
std::unique_ptr<Model> gronudModel_;

// 地面ポインタ
std::unique_ptr<Ground> ground_;

#pragma endregion




#pragma region カメラ

DirectXCommon* dxCommon_ = nullptr;
bool isDebgCameraActive_ = false;
DebugCamera* debugCamera_ = nullptr;

 std::unique_ptr<FollowCamera> followCamera_ ; 


#pragma endregion
	
	enum class Scene {
	 // タイトル
	 Title,

	 // カウントダウン
	 Ready,

	 // ゲーム
	 Game,

	 // 結果
	 Result,

	 // 勝ち
	 Win,

	 // 負け
	 Lose,

 };

 Scene scene_ = Scene();


 	enum class DrawSpriteScene 
	{
	 // タイトル
	 Title,

	 // カウントダウン
	 Ready,

	 // ゲーム
	 Game,

	 // 結果
	 Result,

	 // 勝ち
	 Win,

	 // 負け
	 Lose,
 };

 DrawSpriteScene drawSpriteScene_ = DrawSpriteScene();

 enum class Draw3DObjectScene {
	 // タイトル
	 Title,

	 // カウントダウン
	 Ready,

	 // ゲーム
	 Game,

	 // 結果
	 Result,

	 // 勝ち
	 Win,

	 // 負け
	 Lose,
 };

 Draw3DObjectScene draw3DObjectScene_ = Draw3DObjectScene();


 enum class Draw2DObjectScene {
	 // タイトル
	 Title,

	 // カウントダウン
	 Ready,

	 // ゲーム
	 Game,

	 // 結果
	 Result,

	 // 勝ち
	 Win,

	 // 負け
	 Lose,
 };

 Draw2DObjectScene draw2DObjectScene_ = Draw2DObjectScene();

};