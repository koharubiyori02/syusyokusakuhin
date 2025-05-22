//===========================================================
//
// プレイヤーの弾のベース
// 
//===========================================================

#pragma once

#include "gameObject.h"
#include "component.h"

class Bullet : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	const int BULLET_DEFAULT_ATK = 30;		//初期攻撃力
	const float BULLET_DEFAULT_SPEED = 0.5f;//初期弾速

	int m_SpeedUpCount = 0;					//スピードアップアイテムの取得数
	int m_AtkUpCount = 0;					//攻撃力アップアイテムの取得数
	float m_SpeedUpValue = 0.5f;			//スピードアップの値
	float m_Speed = BULLET_DEFAULT_SPEED;	//弾の速度

	bool m_Penetrate = false;				//貫通するかどうか

	std::vector<GameObject*> m_PenetrateList; //貫通したオブジェクトを入れておく

protected:

	bool m_Active = false;

public:

	Bullet() {};

	void Init() override;
	void Uninit() override;
	virtual void Update() override;
	void Draw() override;

	void PlusSpeed() { m_SpeedUpCount += 1; }
	void PlusAtk() { m_AtkUpCount += 1; }

	virtual Bullet* GetBaseBullet() { return this; }

	void SetPenetrate(const bool frag) { m_Penetrate = frag; }

	void SetActive(const bool active) { m_Active = active; }
	bool const GetActive() { return m_Active; }

	void InitBulletStatus();
};