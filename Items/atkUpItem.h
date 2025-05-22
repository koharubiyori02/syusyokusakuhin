//===========================================================
//
// 攻撃力UPアイテム
// 
//===========================================================

#pragma once
#include "item.h"
#include "../PlayerBullet/atkUpAbility.h"

class AtkUpItem : public Item
{
private:
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

public:
	AtkUpItem() { m_IsDuplication = 0; }

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	Item* GetClone() override { return new AtkUpItem; }//自身を返す
	BulletAbility* GetAbility() override { return new AtkUpAbility; };//攻撃力アップ能力を返す
};
