#include"WorldTransform.h"
#include"Model.h"
#include"ViewProjection.h"
class BaseCharacter 
{
public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(const std::vector<Model*>& models);


	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection);

	

	

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetLocalPosition(const Vector3& position);

	void SetLocalRotation(const Vector3& rotaion);

	const Vector3& GetLocalPosition() const { return worldTransform_.translation_; }

	

protected:

	
	// モデル
	std::vector<Model*> models_;

	// ワールド変換データ
	WorldTransform worldTransform_;
	
	

};

