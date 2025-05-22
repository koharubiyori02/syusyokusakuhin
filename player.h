//===========================================================
//
//�@�v���C���[
// 
//===========================================================

#pragma once

#include "gameObject.h"
#include "component.h"
#include "PlayerBullet//bulletFactory.h"

#include <list>

#define PLAYER_DEFAULT_HP 5
#define PLAYER_MOVESPEED 45.0f

class PlayerState;

//�v���C���[�X�e�[�g�̎���
enum class PlayerStateName : unsigned int
{
	PlayerIdle,		//�A�C�h�����
	PlayerMove,		//�����Ă�����
	PlayerShooting, //�e�������Ă�����

	Num
};

class Player : public GameObject
{
private:
	PlayerState*	m_State{};			//�v���C���[�̃X�e�[�g
	std::vector<PlayerState*> m_States;

	BulletFactory* m_BulletFactory{};	//�e�����̃t�@�N�g���[

	ID3D11VertexShader*		m_VertexShader{};
	ID3D11PixelShader*		m_PixelShader{};
	ID3D11InputLayout*		m_VertexLayout{};

	int m_KillCount = 0;				//�|������
	
	std::string				m_AnimationName1{};
	std::string				m_AnimationName2{};
	float					m_AnimationBlendRatio = 0.0f;
	int						m_AnimationFrame = 0;

	bool					m_IsMoving = true;//�����Ă��邩�ǂ���
	float					m_Speed = 45.0f;  //�e��

	bool					m_IsDeath = false;//����ł��邩�ǂ���
	int						m_DeathCount = 0; //���t���[�����delete���邩�̃J�E���g

	XMFLOAT3 DEFAULT_SCALE = { 0.01f,0.01f,0.01f };

public:
	Player(int hp)
	{
		SetHp(hp);
	}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ImGuiDebug() override;

	void ChangeState(PlayerStateName name);//�X�e�[�g��؂�ւ���

	BulletFactory* GetBulletFactory() const { return m_BulletFactory; }

	void SetVelocity(const XMFLOAT3 vel) { m_Velocity = vel; }
	XMFLOAT3 GetVelocity() const { return m_Velocity; }

	void KillCountPlus() { m_KillCount++; }
	void ResetKillCount() { m_KillCount = 0; }
	int GetKillCount() const  { return m_KillCount; }

	void PlusShootDelayCount();//���ˊԊu��Z������

	bool GetMoving() const { return m_IsMoving; }

	void SetSpeed(const float speed) { m_Speed = speed; }
	float GetSpeed() const { return m_Speed; }
};