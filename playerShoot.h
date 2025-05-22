//===========================================================
//
//　プレイヤーの弾を発射する機能
// 
//===========================================================

#pragma once

class PlayerShoot
{
private:
	float m_ShootDelay = 60.0f;//発射間隔
	float m_Counter = 0.0f;	   //発射間隔のカウンター
	bool m_IsShooting = false; //発射硬直中かどうか
public:

	void Shoot(XMFLOAT3 position);//弾を発射する

	void PlusCount();			  //硬直のカウントを進める

	bool GetIsShooting() const { return m_IsShooting; }

	void PlusDelayCount()		  //発射間隔を短くする
	{
		m_ShootDelay -= 10.0f;
		if (m_ShootDelay <= 10)
			m_ShootDelay = 10.0f;
	}
};