//===========================================================
//
//　プレイヤーステートの基底クラス
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

	float m_ShootDelay = 60.0f;//発射間隔
	float m_Counter = 0.0f;	   //発射間隔のカウンター
	bool m_IsShooting = false;//発射硬直中かどうか

protected:

	Player* GetPlayer() { return m_Player; }

	XMFLOAT3 m_Pos = {};
	XMFLOAT3 m_OldPos = {};
	XMFLOAT3 m_Vel = {};

	bool m_Active = false;//そのステートがアクティブかどうか

	PlayerStateName m_StateName;//ステートをenumで探す用

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