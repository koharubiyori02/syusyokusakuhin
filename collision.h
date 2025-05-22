//===========================================================
//
// ìñÇΩÇËîªíË
// 
//===========================================================

#pragma once
#include "main.h"

class Collision
{
private:
	const float m_HitRange = 2.0f;//NormalCollisionópÇÃìñÇΩÇ¡ÇΩîªíËÇÃãóó£

public:
	static Collision& GetInstance()
	{
		static Collision collision;
		return collision;
	}

	bool NormalCollision(XMFLOAT3 posA, XMFLOAT3 posB);																	//ä»à’è’ìÀîªíË
	bool AABBCollision(XMFLOAT3 posA, XMFLOAT3 scaleA, XMFLOAT3 posB, XMFLOAT3 scaleB);									//AABB
	bool OBBCollision(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 axis1, XMFLOAT3 axis2 ,XMFLOAT3 axis3, XMFLOAT3 scale);	//OBB
};