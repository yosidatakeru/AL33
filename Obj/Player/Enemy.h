#pragma once
#include <ViewProjection.h>
#include "Model.h"
#include "WorldTransform.h"
#include"Player.h"
#include"ViewProjection.h"
class Enemy : public BaseCharacter 
{
public:
	enum {
		Enemymodel
	};
	//	//Vector3 velociy_;
	//	/// <summary>
	//	/// 初期化
	//	/// </summary>
	void Initialize(const std::vector<Model*>& models) override;
	//
	//	/// <summary>
	//	/// 更新
	//	/// </summary>
	void Update() override;
	//
	//	/// <summary>
	//	/// 描画
	//	/// </summary>
	void Draw(const ViewProjection& viewProjection)override;


private:
	
	// ワールドトランスホーム:
	WorldTransform worldTransform_;
	
	Vector3 GetWorldPosition();

    const float kCharacterSpeed = 1.0f;
	// カメラ
	const ViewProjection* viewProjection_ = nullptr;
	
	//敵の行動
	void ApproachUpdate();

	void LeaveUpdate();

	// enum宣言
	enum class Phase {
		Approach, // 接近
		Leave,    // 離脱
	};
	Phase phase_ = Phase();
};