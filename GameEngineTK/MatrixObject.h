/// <summary>
/// オブジェクトの行列クラス
/// </summary>
#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>
#include "AStar/Math.h"

class MatrixObject {
public:
	using Vector3 = DirectX::SimpleMath::Vector3;
	//ワールド行列作成
	void WorldMatrixCreate();
	//ワールド座標設定
	virtual void SetPosition(const D3DXVECTOR3& position) { transfer = position; }
	//ワールド座標取得
	virtual const D3DXVECTOR3& GetPosition() { return transfer; }
	//スケール設定
	virtual void SetScale(const D3DXVECTOR3& scale) { this->scale = scale; }
	//スケール取得
	virtual const D3DXVECTOR3& GetScale() { return  this->scale; }
	//スケール設定
	virtual void SetRotation(const D3DXVECTOR3& rotation) { this->rotation = rotation; IsUseQuternion = false; }
	//スケール取得
	virtual const D3DXVECTOR3& GetRotation() { return  this->rotation; }
	//クォータニオン設定
	virtual void SetQuaternion(const D3DXQUATERNION& quaternion) { this->quaternion = quaternion; IsUseQuternion = true; }
	//クォータニオン取得
	virtual const D3DXQUATERNION& GetQuaternion() { return  this->quaternion; }
	//ワールド座標設定
	virtual void SetPosition(const Vector3& position) { transfer = Math::VectorToD3DXVECTOR3(position); }
	//ワールド座標取得
	virtual const Vector3& GetPositionMath() { return Math::D3DXVECTOR3ToVector(transfer); }
	//スケール設定
	virtual void SetScale(const Vector3& scale) { this->scale = Math::VectorToD3DXVECTOR3(scale); }
	//スケール取得
	virtual const Vector3& GetScaleMath() { return Math::D3DXVECTOR3ToVector(this->scale); }
	//回転設定
	virtual void SetRotation(const Vector3& rotation) { this->rotation = Math::VectorToD3DXVECTOR3(rotation); }
	//回転取得
	virtual const Vector3& GetRotationMath() { return Math::D3DXVECTOR3ToVector(this->rotation); IsUseQuternion = false; }
	//クォータニオン設定
	virtual void SetQuaternion(const DirectX::SimpleMath::Quaternion& quaternion) { this->quaternion = Math::QuaterniontoD3DXQUATERNION(quaternion); IsUseQuternion = true; }
	//クォータニオン取得
	virtual const DirectX::SimpleMath::Quaternion& GetQuaternionMath() { return  Math::D3DXQUATERNIONtoQuaternion(this->quaternion); }
	//ワールド行列取得
	const D3DXMATRIX& GetWorldMatrix() { return worldMatrix; }
private:
	//全行列作成
	void CreateAllMatrix();
	//ワールド行列作成
	void CreateTransferMatrix();
	//スケール行列作成
	void CreateScaleMatrix();
	//回転行列作成
	void CreateRotationMatrix();
private:
	D3DXMATRIX worldMatrix;//ワールド行列
	D3DXVECTOR3 transfer;//移動量
	D3DXVECTOR3 scale;//サイズ
	D3DXVECTOR3 rotation;//回転
	D3DXQUATERNION quaternion;//クォータニオン
	D3DXMATRIX transferMatrix;//移動行列
	D3DXMATRIX scaleMatrix;//スケール行列
	D3DXMATRIX rotationMatrix;//回転行列
	bool IsUseQuternion = false;//クォータニオンを使用するか
};