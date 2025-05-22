#include "collision.h"

//簡易当たり判定
bool Collision::NormalCollision(XMFLOAT3 posA, XMFLOAT3 posB)
{
	XMVECTOR vecA, vecB;
	vecA = XMLoadFloat3(&posA);
	vecB = XMLoadFloat3(&posB);

	//posAとposBの距離
	float length = XMVectorGetX(XMVector3Length(XMVectorSubtract(vecA, vecB)));

	if (length < m_HitRange)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Collision::AABBCollision(XMFLOAT3 posA, XMFLOAT3 scaleA, XMFLOAT3 posB, XMFLOAT3 scaleB)
{
	XMVECTOR vecA{}, vecB{}, vecScaleA{}, vecScaleB{};

	vecA = XMLoadFloat3(&posA);
	vecB = XMLoadFloat3(&posB);
	vecScaleA = XMLoadFloat3(&scaleA);
	vecScaleB = XMLoadFloat3(&scaleB);

	XMVECTOR minA = XMVectorSubtract(vecA, vecScaleA * 0.5f);
	XMVECTOR maxA = XMVectorAdd(vecA, vecScaleA * 0.5f);
	XMVECTOR minB = XMVectorSubtract(vecB, vecScaleB * 0.5f);
	XMVECTOR maxB = XMVectorAdd(vecB, vecScaleB * 0.5f);

	//重なり判定
	XMVECTOR lessThanMax = XMVectorGreaterOrEqual(maxA, minB);
	XMVECTOR greaterThanMin = XMVectorGreaterOrEqual(maxB, minA);
	XMVECTOR overlap = XMVectorAndInt(lessThanMax, greaterThanMin);

	//全軸で重なっていれば当たっている
	return XMVectorGetX(overlap) && XMVectorGetY(overlap) && XMVectorGetZ(overlap);
}

bool Collision::OBBCollision(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 axis1, XMFLOAT3 axis2,XMFLOAT3 axis3, XMFLOAT3 scale)
{
	XMVECTOR vecA, vecB, scl;
	vecA = XMLoadFloat3(&pos1);
	vecB = XMLoadFloat3(&pos2);
	scl = XMLoadFloat3(&scale);

	//OBB
	XMVECTOR direction;//相手から自分へのベクトル
	direction = XMVectorSubtract(vecA, vecB);
	
	//分離軸
	XMVECTOR axisX = XMLoadFloat3(&axis1);
	float dotX = XMVectorGetX(XMVector3Dot(direction, axisX));

	//分離軸
	XMVECTOR axisY = XMLoadFloat3(&axis2);
	float dotY = XMVectorGetX(XMVector3Dot(direction, axisY));

	//分離軸
	XMVECTOR axisZ = XMLoadFloat3(&axis3);
	float dotZ = XMVectorGetZ(XMVector3Dot(direction, axisZ));

	if (-XMVectorGetX(scl) < dotX && dotX < XMVectorGetX(scl) &&
		-XMVectorGetY(scl) < dotY && dotY < XMVectorGetY(scl) &&
		-XMVectorGetZ(scl) < dotZ && dotZ < XMVectorGetZ(scl))
	{
		return true;
	}
	else
	{
		return false;
	}
}

