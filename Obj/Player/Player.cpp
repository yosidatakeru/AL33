#include "Obj/Player/Player.h"
#include "Function.h"
#include <ImguiManager.h>
#include <cassert>
#include "GameScene.h"






void Player::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	this->model_ = model;
	this->textureHandle_ = textureHandle;
	// ワールド変数の初期化
	worldTransform_.Initialize();

	// シングルトンインスタンスを取得
	//input_ = Input::GetInstance();


	
}

void Player::InitializeFloatingGimmick() 
{ 
  floatingParameter_ = 0.0f;

}






void Player::Update() 
{
	////行列を定数バッファに転送する
	worldTransform_.TransferMatrix();
	
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

	worldTransform_.TransferMatrix();






	// 画面に座標を出す
	ImGui::Begin("Player");

	ImGui::InputFloat3("PlayerPosition", &worldTransform_.translation_.x);
	ImGui::SliderFloat3("PlayerSlide", &worldTransform_.translation_.x, -20.0f, 30.0f);
	
	


	ImGui::End();
}

void Player::UpdateFloatingGimmick() 
{


}




void Player::Draw( ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}




Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;

	// ワールド行列の「平行移動成分」を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

