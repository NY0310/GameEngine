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
	isParantInfluence = false;

}


/// <summary>
/// ワールド行列の掛け算を入れ替える
/// </summary>
/// <param name="order"></param>
void MatrixObject::ChangeOrder(WorldMatrixOrder::ORDER order)
{
	this->order = worldMatrixOrderFactory->Set(order);
}



/// <summary>
/// ワールド行列作成
/// </summary>
void MatrixObject::Calc()
{

	//全ての行列を作成
	CalcAllMatrix();
	//ワールド行列を作成
	CalcWorldMatrix();
	//ワールド・ビュー・プロジェクション行列を作成
	CalcWVP();
}

/// <summary>
/// 親のワールド行列に影響を与える
/// </summary>
/// <param name="parentWorldMatrix">親のワールド行列</param>
void MatrixObject::Calc(const D3DXMATRIX& parentWorldMatrix)
{
	//全ての行列を作成
	CalcAllMatrix();
	//ワールド行列を作成
	CalcWorldMatrix();
	this->worldMatrix *= parentWorldMatrix;
	//ワールド・ビュー・プロジェクション行列を作成
	CalcWVP();

}


/// <summary>
/// 全行列作成
/// </summary>
void MatrixObject::CalcAllMatrix()
{
	//移動行列作成
	CalcTransferMatrix();
	//回転行列作成
	CalcScaleMatrix();
	//スケール行列作成
	CalcRotationMatrix();
}


/// <summary>
/// ワールド行列を作成
/// </summary>
void MatrixObject::CalcWorldMatrix()
{
	//スケール・回転・移動行列からワールド行列作成
	order._Get()->Calculation(worldMatrix, scaleMatrix, rotationMatrix, transferMatrix);
}


/// <summary>
/// ワールド・ビュー・プロジェクション行列を作成
/// </summary>
void MatrixObject::CalcWVP()
{
	FollowCamera* camera = FollowCamera::GetInstance();

	if (isBillBoard)
	{

		//ビルボード行列、ワールド行列・ビュー行列・プロジェクション行列を作成
		wvp = camera->GetBillboard()  * worldMatrix  * camera->GetView() * camera->GetProjection();
	}
	else
	{
		//ワールド行列・ビュー行列・プロジェクション行列を作成
		wvp = worldMatrix * camera->GetView() * camera->GetProjection();
	}
}

/// <summary>
/// 移動行列作成
/// </summary>
void MatrixObject::CalcTransferMatrix()
{
	if(isCalcTransferMatrix)
	D3DXMatrixTranslation(&transferMatrix, this->transfer.x, this->transfer.y, this->transfer.z);
}

/// <summary>
/// 回転行列作成
/// </summary>
void MatrixObject::CalcRotationMatrix()
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
void MatrixObject::CalcScaleMatrix()
{
	if(isCalcScaleMatrix)
	D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
}


