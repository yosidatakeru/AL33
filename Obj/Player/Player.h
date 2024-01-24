#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Function.h"
#include "Input.h"
#include"PlayerBullet.h"
#include<list>
class Player {

	/// <summary>
	/// デストラクタ
	/// </summary>
	
	/// <summary>
	/// 自キャラ
	/// </summary>
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm);

	void InitializeFloatingGimmick();

	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void UpdateFloatingGimmick();


	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	


	void SetViewProjection(const ViewProjection* viewProjectio) 
	{
		 viewProjection_ = viewProjectio;
	}

	Vector3 GetWorldPosition();

	

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	


private:

	//プレイヤー モデルデータ
	Model* modelFighterBody_ = nullptr;
	Model* modelFighterHead_ = nullptr;
	Model* modelFighterL_arm_ = nullptr;
	Model* modelFighterR_arm_ = nullptr;

	//ギミックパラメータ
	float floatingParameter_ = 0.0f;

	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	
	// キーボード入力
	Input* input_ = nullptr;

	//カメラ
	const ViewProjection* viewProjection_ = nullptr;

	/* ----- GamePad ゲームパッド ----- */

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState_;

	//パラメータ
	
 float period = 100.0f;

 float floating = 0.5f ;
};