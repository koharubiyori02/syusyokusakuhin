//===========================================================
//
//　貫通アイテム
// 
//===========================================================

#pragma once
#include "item.h"
#include "../PlayerBullet/penetrateAbility.h"

class PenetrateItem : public Item
{
private:
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
public:
	PenetrateItem() { m_IsDuplication = 1; }//重複不可

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	Item* GetClone() override { return new PenetrateItem; }//自身を返す

	BulletAbility* GetAbility() override { return new PenetrateAbility; };//貫通能力を返す
};
