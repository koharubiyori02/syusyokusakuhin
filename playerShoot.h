//===========================================================
//
//�@�v���C���[�̒e�𔭎˂���@�\
// 
//===========================================================

#pragma once

class PlayerShoot
{
private:
	float m_ShootDelay = 60.0f;//���ˊԊu
	float m_Counter = 0.0f;	   //���ˊԊu�̃J�E���^�[
	bool m_IsShooting = false; //���ˍd�������ǂ���
public:

	void Shoot(XMFLOAT3 position);//�e�𔭎˂���

	void PlusCount();			  //�d���̃J�E���g��i�߂�

	bool GetIsShooting() const { return m_IsShooting; }

	void PlusDelayCount()		  //���ˊԊu��Z������
	{
		m_ShootDelay -= 10.0f;
		if (m_ShootDelay <= 10)
			m_ShootDelay = 10.0f;
	}
};