//===========================================================
//
//�@�X�s�[�hUP�A�C�e��
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

	Item* GetClone() override { return new SpeedUpItem; }//�����𐶐����ĕԂ�
	BulletAbility* GetAbility() override 
	{   
		//�X�s�[�h�A�b�v�̌��ʂ�Ԃ�
		Scene* scene = Manager::GetScene();
		Player* player = scene->GetGameObject<Player>();

		player->PlusShootDelayCount();//���ˊԊu��Z������
		return new SpeedUpAbility; 
	};
};
