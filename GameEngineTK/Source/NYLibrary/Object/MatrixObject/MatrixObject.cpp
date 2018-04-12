/// <summary>
/// オブジェクトの行列クラス
/// </summary>
#include "MatrixObject.h"
using namespace NYLibrary;

MatrixObject::MatrixObject()
{
	MatrixInitialize();
}


void NYLibrary::MatrixObject::MatrixInitialize()
{
	isUseQuternion = true;
	transfer = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
	localSize = 0;
	eulerAangle = D3DXVECTOR3(0, 0, 0);
	quaternion = D3DXQUATERNION(0, 0, 0, 0);
	D3DXMatrixIdentity(&worldMatrix);//ワールド行列
	D3DXMatrixIdentity(&transferMatrix);
	D3DXMatrixIdentity(&scaleMatrix);//スケール行列
	D3DXMatrixIdentity(&rotationMatrix);//回転行列
	D3DXMatrixIdentity(&wvp);//ワールドビュープロジェクション行列
	worldMatrixOrderFactory = new WorldMatrixOrderFactory();
	//ワールド行列の掛け算順序ファクトリーの初期化
	order = worldMatrixOrderFactory->Set(WorldMatrixOrder::ORDER::SCALEMAT_ROTOMAT_TRANSMAT);
	//ビルボードにしない
	isBillBoard = false;

}


/// <summary>
/// ワールド行列の掛け算を入れ替える
/// </summary>
/// <param name="order"></param>
void MatrixObject::ChangeOrder(WorldMatrixOrder::ORDER order)
{
	//this->order.release();
	this->order = worldMatrixOrderFactory->Set(order);
}



/// <summary>
/// ワールド行列作成
/// </summary>
void MatrixObject::Calc()
{
	FollowCamera* camera = FollowCamera::GetInstance();
	D3DXMATRIX view;

	//全ての行列を作成
	CreateAllMatrix();
	//スケール・回転・移動行列からワールド行列作成
	order._Get()->Calculation(worldMatrix, scaleMatrix, rotationMatrix, transferMatrix);

	if (isBillBoard)
	{
		D3DXMATRIX w;
		D3DXMATRIX bill = camera->GetBillboard();
		D3DXMATRIX test;
		D3DXMatrixRotationY(&test, D3DX_PI);
		//ワールド行列・ビュー行列・プロジェクション行列を作成
		wvp = camera->GetBillboard()  * worldMatrix  * camera->GetView() * camera->GetProjection();
	}
	else
	{
		//ワールド行列・ビュー行列・プロジェクション行列を作成
		wvp = worldMatrix * camera->GetView() * camera->GetProjection();
	}

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
	if(isCalcTransferMatrix)
	D3DXMatrixTranslation(&transferMatrix, this->transfer.x, this->transfer.y, this->transfer.z);
}

/// <summary>
/// 回転行列作成
/// </summary>
void MatrixObject::CreateRotationMatrix()
{
	if(isCalcRotationMatrix)
	//クォータニオンを使用するか
	if (isUseQuternion)
	{
		D3DXMatrixRotationQuaternion(&rotationMatrix, &quaternion);
	}
	else
	{
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, eulerAangle.x, eulerAangle.y, eulerAangle.z);
	}
}


/// <summary>
/// スケール行列作成
/// </summary>
void MatrixObject::CreateScaleMatrix()
{
	if(isCalcScaleMatrix)
	D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
}


