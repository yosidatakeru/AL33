#pragma once
#include"DebugCamera.h"
#include<ViewProjection.h>
#include"WorldTransform.h"
class FollowCamera 
{

	public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	const ViewProjection& GetViewProlection() { return viewProjection_; }

	private:

	//ヴュープロジェクト
	    ViewProjection viewProjection_;

	   const WorldTransform* target_ = nullptr;
	  
};
