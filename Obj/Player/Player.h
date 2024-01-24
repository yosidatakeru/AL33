#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Function.h"
#include "Input.h"
#include"BaseCharacter.h"
class Player : public BaseCharacter {

	/// <summary>
	/// デストラクタ
	/// </summary>
	
	/// <summary>
	/// 自キャラ
	/// </summary>
public:
	enum  {
		kModelIndexBody,
		kModelIndexHead,
		kMidelIndexL_arm,
		kMidelIndexR_arm,
	};
	/// <summary>
	/// 初期化
	/// </summary>
   void Initialize(const std::vector<Model*>&models)override;
	
	//void Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm);


	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	


	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection)override;

	


	void SetViewProjection(const ViewProjection* viewProjectio) 
	{
		 viewProjection_ = viewProjectio;
	}


	

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	


private:
	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();

	
	Vector3 GetWorldPosition();

	//プレイヤー モデルデータ
	Model* modelFighterBody_=nullptr ;
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

	
	//Model* model_ = nullptr;
	
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