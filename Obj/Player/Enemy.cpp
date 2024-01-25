#include "Enemy.h"
#include "Function.h"
#include <cassert>
#include"ImGuiManager.h"

  void Enemy::Initialize(const std::vector<Model*>& models)
  {
	BaseCharacter::Initialize(models);
	worldTransform_.Initialize();
	
	//初期位置
	worldTransform_.translation_.z = 40.0f;
  }



 void Enemy::Update() 
 {
	  ////行列を定数バッファに転送する
	  worldTransform_.TransferMatrix();

	  	
	

	  // キャラクターも異動ベクトル
	  Vector3 move = {kCharacterSpeed, 0.0f, 0.0f};

	 // 正規化する 
	  move = Normalize(move) * kCharacterSpeed;

	
	 
	  // 移動ベクトルのY軸周り角度
	  worldTransform_.rotation_.y = std::atan2(move.x, move.z);

	  // 座標移動（ベクトルの加算）
	  //worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	  ////平行移動行列
	  worldTransform_.matWorld_ = MakeAffineMatrix(
	      worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	  	worldTransform_.TransferMatrix();



		switch (phase_) {
	    case Phase::Approach:
	    default:
		    ApproachUpdate();

		    break;

	    case Phase::Leave:
		    LeaveUpdate();
		    break;
	    }




		ImGui::Begin("Enemy");

	    ImGui::InputFloat3("PlayerPositionBody", &worldTransform_.translation_.x);
	    ImGui::SliderFloat3("PlayerSlideBody", &worldTransform_.translation_.x, -20.0f, 30.0f);
	   

	    ImGui::End();
 }



 void Enemy::Draw(const ViewProjection& viewProjection)
 {
	  models_[Enemymodel]->Draw(worldTransform_, viewProjection);
 }

 Vector3 Enemy::GetWorldPosition() 
 {
	  Vector3 worldPos;

	  // ワールド行列の「平行移動成分」を取得(ワールド座標)
	  worldPos.x = worldTransform_.matWorld_.m[3][0];
	  worldPos.y = worldTransform_.matWorld_.m[3][1];
	  worldPos.z = worldTransform_.matWorld_.m[3][2];

	  return worldPos;
 }

 //敵の行動
 	void Enemy::ApproachUpdate() {
	  // 移動(ベクトルの加算)

	  // 移動（ベクトル加算）
	  worldTransform_.translation_.x -= kCharacterSpeed;
	  ;
	  // 指定の位置に到達したら離脱する
	  if (worldTransform_.translation_.x <-100.0f) {
		  phase_ = Phase::Leave;
	  }
 }

 void Enemy::LeaveUpdate() {
	  // 移動(ベクトルを加算)
	  // 移動（ベクトル加算）
	  worldTransform_.translation_.x += kCharacterSpeed;
	  if (worldTransform_.translation_.x > 100.0f) {
		  phase_ = Phase::Approach;
	  }
 }