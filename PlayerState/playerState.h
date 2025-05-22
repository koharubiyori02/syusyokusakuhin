//===========================================================
//
//�@�v���C���[�X�e�[�g�̊��N���X
// 
//===========================================================

#pragma once
#include "..//main.h"
#include "../player.h"
#include "../playerShoot.h"

class PlayerState
{
private:
	Player* m_Player;

	float m_ShootDelay = 60.0f;//���ˊԊu
	float m_Counter = 0.0f;	   //���ˊԊu�̃J�E���^�[
	bool m_IsShooting = false;//���ˍd�������ǂ���

protected:

	Player* GetPlayer() { return m_Player; }

	XMFLOAT3 m_Pos = {};
	XMFLOAT3 m_OldPos = {};
	XMFLOAT3 m_Vel = {};

	bool m_Active = false;//���̃X�e�[�g���A�N�e�B�u���ǂ���

	PlayerStateName m_StateName;//�X�e�[�g��enum�ŒT���p

	PlayerShoot* m_PlayerShoot;
public:
	PlayerState(Player* p, PlayerStateName name,PlayerShoot* shoot) : m_Player(p), m_StateName(name),m_PlayerShoot(shoot){}
	~PlayerState(){}

	virtual void Start();
	virtual void Update();
	virtual void Exit();

	void SetActive(const bool active) { m_Active = active; }
	bool const GetActive() { return m_Active; }

	PlayerStateName const GetStateName() { return m_StateName; }

	PlayerShoot* const GetPlayerShoot() { return m_PlayerShoot; }
};