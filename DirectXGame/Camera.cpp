#include "Camera.h"

void Camera::Initialize(Vector3 transkation, Vector3 rotation)
{
	// ワールドトランスフォームの初期設定
	worldTransform_.rotation_ = rotation;
	worldTransform_.translation_ = transkation;
	worldTransform_.Initialize();

	// カメラの位置
	// カメラの座標をここでいじる
	Vector3 move = {0.00f, 0.0f, -10.0f};

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// カメラの回転
	// 角度をこれでいじる
	Vector3 rotate = {0.0f, 0.0f, 0.00f};

	worldTransform_.rotation_ = Add(worldTransform_.rotation_, rotate);

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 1200.0f;
	viewProjection_.Initialize();

}

void Camera::Update() 
{
	worldTransform_.UpdeateMatrix();

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");

	// スライダーでtranslationとratationを表示
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, -200.0f, 200.0f);
	//ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -50.0f, 50.0f);

	ImGui::End();
}
