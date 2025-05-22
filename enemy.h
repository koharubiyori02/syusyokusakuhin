//===========================================================
//
// �G���G
// 
//===========================================================

#pragma once

#include "main.h"
#include "gameObject.h"
#include "component.h"
#include "EnemyState//enemyState.h"
#include "ShootType//shootType.h"

#define ENEMY_MAXHP 100
#define ENEMY_ATK 1

class EnemyHpGage;

class Enemy :public GameObject
{
private:
	EnemyState* m_State{};					//�G�̃X�e�[�g
	ShootType* shootType{};					//�P�����O�A�˂�

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	bool m_Tutorial = false;				//�`���[�g���A���p���ǂ���

	EnemyHpGage* m_Gage{};					//HP�Q�[�W

	XMFLOAT3 DEFAULT_SCALE = { 1.2f, 1.2f, 1.2f };

public:
	Enemy(ShootType* type,int hp,int atk) 
		:shootType(type)
	{
		SetHp(hp);
		SetAtk(atk);
	}

	Enemy(int hp, bool t)
		:m_Tutorial(t)
	{
		SetHp(hp);
	}

	~Enemy()
	{
		delete m_State;
		delete shootType;		
	}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ImGuiDebug() override;

	void Shoot();	//�e����


	template<typename T>
	void SetEnemyState()
	{
		if (m_State != nullptr)
		m_State->Exit();

		delete m_State;
		m_State = new T(this);
		m_State->Start();
	}
};