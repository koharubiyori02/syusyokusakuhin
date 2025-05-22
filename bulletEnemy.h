//===========================================================
//
// �G�̒e
// 
//===========================================================

#pragma once

#include "gameObject.h"
#include "component.h"
#include <list>

class BulletEnemy : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_Life = 0;				//�e�̎���
	const int MAX_LIFE = 300;	//�e�̎����̌��E
	float m_Speed = 0.1f;

	bool m_Active = false;

public:
	~BulletEnemy() {}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	int GetLife() const { return m_Life; }

	void SetActive(const bool active) { m_Active = active; }
	bool const GetActive() { return m_Active; }

	//�e�̏�����
	void ResetBullet()
	{
		m_Active = false;
		m_Life = 0;
	}
};