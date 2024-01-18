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
	void Initialize(Model* model, uint32_t textureHandle);

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

	// モデルデータ
	Model* modelFighterBody_;
	Model* modelFighterHead_;
	Model* modelFighterL_arm_;
	Model* modelFighterR_arm_;

	//ギミックパラメータ
	float floatingParameter_ = 0.0f;

	// ワールド変換データ
	WorldTransform worldTransform_;

	
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	
	// キーボード入力
	Input* input_ = nullptr;

	//カメラ
	const ViewProjection* viewProjection_ = nullptr;

	/* ----- GamePad ゲームパッド ----- */

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState_;
};