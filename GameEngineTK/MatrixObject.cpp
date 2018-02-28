/// <summary>
/// オブジェクトの行列クラス
/// </summary>
#include "MatrixObject.h"

MatrixObject::MatrixObject()
{
	scale = D3DXVECTOR3(1,1,1);
	rotation = D3DXVECTOR3(0, 0, 0);
}

/// <summary>
/// ワールド行列作成
/// </summary>
void MatrixObject::Update()
{
	FollowCamera* camera = FollowCamera::GetInstance();
	//全ての行列を作成
	CreateAllMatrix();
	//スケール・回転・移動行列からワールド行列作成
	worldMatrix = scaleMatrix * rotationMatrix * transferMatrix;
	//ワールド行列・ビュー行列・プロジェクション行列を作成
	wvp = worldMatrix * camera->GetView() * camera->GetProjection();
}

/// <summary>
/// 全行列作成
/// </summary>
void MatrixObject::CreateAllMatrix()
{
	//移動行列作成
	CreateTransferMatrix();
	//回転行列作成
	CreateScaleMatrix();
	//スケール行列作成
	CreateRotationMatrix();
}

/// <summary>
/// 移動行列作成
/// </summary>
void MatrixObject::CreateTransferMatrix()
{
	D3DXMatrixTranslation(&transferMatrix, this->transfer.x, this->transfer.y, this->transfer.y);
}

/// <summary>
/// 回転行列作成
/// </summary>
void MatrixObject::CreateRotationMatrix()
{
	//クォータニオンを使用するか
	if (IsUseQuternion)
	{
		D3DXMatrixRotationQuaternion(&rotationMatrix, &quaternion);
	}
	else
	{
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.x, rotation.y, rotation.z);
	}
}


/// <summary>
/// スケール行列作成
/// </summary>
void MatrixObject::CreateScaleMatrix()
{
	D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
}


