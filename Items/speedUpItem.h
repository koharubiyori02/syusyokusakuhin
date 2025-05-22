//===========================================================
//
//　スピードUPアイテム
// 
//===========================================================

#pragma once
#include "item.h"
#include "../PlayerBullet/speedUpAbility.h"

class SpeedUpItem : public Item
{
private:
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

public:
	SpeedUpItem() { m_IsDuplication = 0; }

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	Item* GetClone() override { return new SpeedUpItem; }//自分を生成して返す
	BulletAbility* GetAbility() override 
	{   
		//スピードアップの効果を返す
		Scene* scene = Manager::GetScene();
		Player* player = scene->GetGameObject<Player>();

		player->PlusShootDelayCount();//発射間隔を短くする
		return new SpeedUpAbility; 
	};
};
