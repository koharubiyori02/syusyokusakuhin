//===========================================================
//
//　四角形の当たり判定
// 
//===========================================================

#pragma once

#include "main.h"
#include "gameObject.h"
#include "component.h"

class HitBox :public Component
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	XMFLOAT3 m_Position{};
	XMFLOAT3 m_Rotation{};
	XMFLOAT3 m_Scale{};

	const float POSITOIN_OFFSET = 4.0f;//プレイヤーの少し前の座標にするオフセット

	bool m_IsHit = false;		//当たったかどうか
	bool m_CanDamage = false;   //当たり判定を取っていいかどうか

	int m_ValidFrame = 0;		//何フレーム当たり判定があるか
	int m_Damage = 0;			//与えるダメージ量

public:
	using Component::Component;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void AddDamage();//ダメージを与える

	void SetPosition(const XMFLOAT3 pos) { m_Position = pos; }
	void SetScale(const XMFLOAT3 scale) { m_Scale = scale; }
	
	void SetIsHit(const bool hit) { m_IsHit = hit; }

	void SetCanDamage(const bool can) { m_CanDamage = can; }//ダメージを与えられるか

	void SetDamageSet(const int damage, const int frame)//与えるダメージと当たり判定を出すフレームをセット
	{
		m_Damage = damage;
		m_ValidFrame = frame;
	}
};