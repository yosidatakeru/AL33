#include "Obj/Player/Player.h"
#include "Function.h"
#include <ImguiManager.h>
#include <cassert>
#include "GameScene.h"






//void Player::Initialize(Model* modelBody,Model*modelHead,Model*modelL_arm,Model* modelR_arm) 
void Player::Initialize(const std::vector<Model*>& models) 
	{
	
	//assert(model);
	
	// 引数として受け取ったデータをメンバ変数に記録する

	
	BaseCharacter::Initialize(models);
	// ワールド変数の初期化
	worldTransform_.Initialize();
	


	//体（プレイヤーモデル）
	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;

	//頭
	worldTransformHead_.Initialize();
	worldTransformHead_.parent_ = &worldTransform_;
	worldTransformHead_.translation_.y = 1.2619f;

	//左腕
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.parent_ = &worldTransform_;
	worldTransformL_arm_.translation_.y = 1.2619f;
	worldTransformL_arm_.translation_.x = -0.5275f;

	//右腕
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.parent_ = &worldTransform_;
	worldTransformR_arm_.translation_.y = 1.2619f;
	worldTransformR_arm_.translation_.x = +0.5275f;
	// シングルトンインスタンスを取得
	input_ = Input::GetInstance();

	//浮遊ギミック初期化
	InitializeFloatingGimmick();
	
	isDead_ = false;
}










void Player::Update() 
{

	

	////行列を定数バッファに転送する
	worldTransform_.TransferMatrix();
	worldTransformBody_.TransferMatrix();
	worldTransformHead_.TransferMatrix();
	worldTransformL_arm_.TransferMatrix();
	worldTransformR_arm_.TransferMatrix();


	
	const float kCharacterSpeed = 0.3f;
	
	// キャラクターも異動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) 
	{
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}
	//正規化する
	move = Normalize(move) * kCharacterSpeed;


	




	//回転	
		Matrix4x4 matRot = MakeRotateXYZMatrix
		(
		    viewProjection_->rotation_.x, viewProjection_->rotation_.y,
		    viewProjection_->rotation_.z);

		move = TransformNormal(move, matRot);
		
	





	

	//移動ベクトルのY軸周り角度
	worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	
	// 座標移動（ベクトルの加算）
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	////平行移動行列

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);



	// 体(モデル)
	//  移動ベクトルのY軸周り角度
	worldTransformBody_.rotation_.y = std::atan2(move.x, move.z);

	// 座標移動（ベクトルの加算）
	worldTransformBody_.translation_ = Add(worldTransformBody_.translation_, move);
	////平行移動行列

	worldTransformBody_.matWorld_ = MakeAffineMatrix(
	    worldTransformBody_.scale_, worldTransformBody_.rotation_,
	    worldTransformBody_.translation_);

	

	//頭(モデル)
	// 移動ベクトルのY軸周り角度
	worldTransformHead_.rotation_.y = std::atan2(move.x, move.z);

	// 座標移動（ベクトルの加算）
	worldTransformHead_.translation_ = Add(worldTransformHead_.translation_, move);
	////平行移動行列

	worldTransformHead_.matWorld_ = MakeAffineMatrix(
	    worldTransformHead_.scale_, worldTransformHead_.rotation_,worldTransformHead_.translation_);

	

	// 左腕(モデル)
	//  移動ベクトルのY軸周り角度
	worldTransformL_arm_.rotation_.y = std::atan2(move.x, move.z);

	// 座標移動（ベクトルの加算）
	worldTransformL_arm_.translation_ = Add(worldTransformL_arm_.translation_, move);
	////平行移動行列

	worldTransformL_arm_.matWorld_ = MakeAffineMatrix(
	worldTransformL_arm_.scale_, worldTransformL_arm_.rotation_,
	worldTransformL_arm_.translation_);


	// 右腕(モデル)
	//  移動ベクトルのY軸周り角度
	worldTransformR_arm_.rotation_.y = std::atan2(move.x, move.z);

	// 座標移動（ベクトルの加算）
	worldTransformR_arm_.translation_ = Add(worldTransformR_arm_.translation_, move);
	////平行移動行列

	worldTransformR_arm_.matWorld_ = MakeAffineMatrix(
	    worldTransformR_arm_.scale_, worldTransformR_arm_.rotation_,
	    worldTransformR_arm_.translation_);
	
	
	// 浮遊ギミック更新
	UpdateFloatingGimmick();

	worldTransform_.TransferMatrix();
	worldTransformBody_.TransferMatrix();
	worldTransformHead_.TransferMatrix();
	worldTransformL_arm_.TransferMatrix();
	worldTransformR_arm_.TransferMatrix();





	// 画面に座標を出す
	ImGui::Begin("Player");

	ImGui::InputFloat3("PlayerPositionBody", &worldTransformBody_.translation_.x);
	ImGui::SliderFloat3("PlayerSlideBody", &worldTransformBody_.translation_.x, -20.0f, 30.0f);
	ImGui::InputFloat3("PlayerPosition", &worldTransformHead_.translation_.x);
	ImGui::SliderFloat3("PlayerSlide", &worldTransformHead_.translation_.x, -20.0f, 30.0f);
	ImGui::InputFloat3("PlayerPosition", &worldTransformL_arm_.translation_.x);
	ImGui::SliderFloat3("PlayerSlide", &worldTransformL_arm_.translation_.x, -20.0f, 30.0f);
	ImGui::InputFloat3("PlayerPosition", &worldTransformR_arm_.translation_.x);
	ImGui::SliderFloat3("PlayerSlide", &worldTransformR_arm_.translation_.x, -20.0f, 30.0f);

	ImGui::End();
}






void Player::Draw(const ViewProjection& viewProjection)
{

	if (isDead_ == false) 
	{
		models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
		models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
		models_[kMidelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
		models_[kMidelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
	}

}








Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;

	// ワールド行列の「平行移動成分」を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	worldPos.x = worldTransformBody_.matWorld_.m[3][0];
	worldPos.y = worldTransformBody_.matWorld_.m[3][1];
	worldPos.z = worldTransformBody_.matWorld_.m[3][2];
	

	return worldPos;
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() 
{

	const float suteppu = 2.0f * 3.14f / period;

	floatingParameter_ += suteppu;

	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * 3.14f);

	worldTransformHead_.translation_.y = std::sin(floatingParameter_) * floating;
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floating;
	worldTransformL_arm_.translation_.y = std::sin(floatingParameter_) * floating;
	worldTransformR_arm_.translation_.y = std::sin(floatingParameter_) * floating;
}

//当たり判定の処理
void Player::OnCollsion() { isDead_ = true; };