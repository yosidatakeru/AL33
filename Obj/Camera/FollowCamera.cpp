#include "FollowCamera.h"
#include"Function.h"

void FollowCamera::Initialize()
{ 
	
	viewProjection_.farZ = 2000.0f;
	// ワールド変数の初期化
	viewProjection_.Initialize();
}

void FollowCamera::Update()
{

	
	//追従対象がいれば
	if (target_)
	{
		XINPUT_STATE joyStatoe;

		if (Input::GetInstance()->GetJoystickState(0, joyStatoe))
		{
			const float speed = 0.03f;

			viewProjection_.rotation_.y += (float)joyStatoe.Gamepad.sThumbRX / SHRT_MAX * speed;
		}


		//対象からカメラまでのオフセット
		 Vector3 offset = {0.0f, 2.0f, -10.0};


		
		Matrix4x4 matRot = MakeRotateXYZMatrix(
		     viewProjection_.rotation_.x, viewProjection_.rotation_.y, viewProjection_.rotation_.z);
		 
		offset = TransformNormal(offset, matRot);

		//座標のコピーしてオフセット分ずらす
		 viewProjection_.translation_ = target_->translation_ + offset;

		

	}

	viewProjection_.UpdateViewMatrix();

}
