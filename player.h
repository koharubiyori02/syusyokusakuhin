//===========================================================
//
//　プレイヤー
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

//プレイヤーステートの識別
enum class PlayerStateName : unsigned int
{
	PlayerIdle,		//アイドル状態
	PlayerMove,		//動いている状態
	PlayerShooting, //弾を撃っている状態

	Num
};

class Player : public GameObject
{
private:
	PlayerState*	m_State{};			//プレイヤーのステート
	std::vector<PlayerState*> m_States;

	BulletFactory* m_BulletFactory{};	//弾生成のファクトリー

	ID3D11VertexShader*		m_VertexShader{};
	ID3D11PixelShader*		m_PixelShader{};
	ID3D11InputLayout*		m_VertexLayout{};

	int m_KillCount = 0;				//倒した数
	
	std::string				m_AnimationName1{};
	std::string				m_AnimationName2{};
	float					m_AnimationBlendRatio = 0.0f;
	int						m_AnimationFrame = 0;

	bool					m_IsMoving = true;//動いているかどうか
	float					m_Speed = 45.0f;  //弾速

	bool					m_IsDeath = false;//死んでいるかどうか
	int						m_DeathCount = 0; //何フレーム後にdeleteするかのカウント

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

	void ChangeState(PlayerStateName name);//ステートを切り替える

	BulletFactory* GetBulletFactory() const { return m_BulletFactory; }

	void SetVelocity(const XMFLOAT3 vel) { m_Velocity = vel; }
	XMFLOAT3 GetVelocity() const { return m_Velocity; }

	void KillCountPlus() { m_KillCount++; }
	void ResetKillCount() { m_KillCount = 0; }
	int GetKillCount() const  { return m_KillCount; }

	void PlusShootDelayCount();//発射間隔を短くする

	bool GetMoving() const { return m_IsMoving; }

	void SetSpeed(const float speed) { m_Speed = speed; }
	float GetSpeed() const { return m_Speed; }
};