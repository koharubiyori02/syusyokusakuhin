//===========================================================
//
// ボス1
// 
//===========================================================

#pragma once

#include "main.h"
#include "gameObject.h"
#include "BehaviorTree//behaviorTree.h"
#include "BehaviorTree//nodeRegistry.h"
#include "ShootType//shootType.h"

#define BOSS_MAXHP 1500

class Boss1 : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	std::string				m_AnimationName1{};
	std::string				m_AnimationName2{};
	float					m_AnimationBlendRatio = 0.0f;
	int						m_AnimationFrame = 0;

	XMFLOAT3 DEFAULT_POSITION = { 0.0f,0.0f,20.0f };
	XMFLOAT3 DEFAULT_SCALE = { 0.03f,0.03f,0.03f };
	float DEFAULT_SPEED = 0.2f;

	ShootType* shootType;				//単発

	int m_Frame = 0;					//移動以外のアニメーションフレーム
	bool m_Anim = false;				//アニメーション中かどうか
	bool m_IsHalfHP = false;			//HPが半分以下かどうか

	BehaviorTree* m_BehaviorTree;
	NodeRegistry* m_Register;

	std::string m_AttackNodeName = "";	//攻撃ノードの名前
	float m_AttackRange = 0.0f;			//攻撃範囲
	float m_Speed = DEFAULT_SPEED;		//移動スピード

	bool m_IsDeath = false;

	bool m_Active = true;

	//攻撃方法と必要な距離
	std::unordered_map<std::string, float> m_AttackRangeMap{
		{"Punch",			 5.0f},
		{"JumpingAttack",	10.0f},
		{"EnemySpawn",		10.0f},
		{"SlapAttack",		 5.0f}
	};

	//攻撃方法と判定のサイズ
	std::unordered_map<std::string, XMFLOAT3> m_AttackScaleMap{
		{"Punch",			 XMFLOAT3(1.0f,1.0f,1.0f)},
		{"JumpingAttack",	XMFLOAT3(10.0f,1.0f,10.0f)},
		{"EnemySpawn",		XMFLOAT3(1.0f,1.0f,1.0f)},
		{"SlapAttack",		 XMFLOAT3(7.0f,1.0f,7.0f)}
	};

	//攻撃の種類を入れて置く配列
	std::string m_AttackNodeNameArray[ATTACKTYPE_NUM] = {
		"Punch",				//近距離パンチ
		"JumpingAttack",		//少し遠くから飛び込み叩きつけ
		"EnemySpawn",			//雑魚敵召喚
		"SlapAttack"			//その場で叩きつけて弾を全方位に発射
	};
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ImGuiDebug() override;

	//ノーマライズしてXMFLOAT3にする
	XMFLOAT3 GetVec(XMVECTOR subtract)
	{
		XMVECTOR vec = XMVector3Normalize(subtract);
		XMFLOAT3 totalVec;
		XMStoreFloat3(&totalVec, vec);

		return totalVec;
	}

	//ノードの処理をここにまとめて初期化で呼ぶ
	void RegisterNodes();

	bool GetIsDeath() const { return m_IsDeath; }

	bool GetActive() const { return m_Active; }
	void SetActive(const bool active) { m_Active = active; }

	void ResetBoss();//ポジションなどの初期化
};