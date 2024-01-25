#include "BaseCharacter.h"
#include"Function.h"
void BaseCharacter::Initialize(const std::vector<Model*>& models) 
{ 
	models_ = models; 
worldTransform_.Initialize();

}

void BaseCharacter::Update() 
{

worldTransform_.UpdeateMatrix();


}

void BaseCharacter::Draw(const ViewProjection& viewProjection) 
{ 
	for (Model* model : models_)
	{
	model->Draw(worldTransform_, viewProjection);
	}

}

void BaseCharacter::SetLocalPosition(const Vector3& position)
{
	worldTransform_.translation_ = position;
}

void BaseCharacter::SetLocalRotation(const Vector3& rotaion) 
{
	worldTransform_.rotation_ = rotaion;
}
