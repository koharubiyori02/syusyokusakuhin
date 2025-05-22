//===========================================================
//
//�@�A�C�e���}�l�[�W���[
// 
//===========================================================

#pragma once
#include "gameObject.h"
#include "Items//itemFactory.h"

#define SPAWN_DELAY 720

class Item;

class ItemManager : public GameObject
{
private:
	ItemFactory* m_ItemFactory;	//�A�C�e���𐶐�����t�@�N�g���[

	int m_SpawnFrame = 0;		//�����Ԋu

	int SPAWN_POSITION_MAX = 80;//�A�C�e���̃|�W�V�����̍ő�l
	int SPAWN_POSITION_MIN = 40;//�A�C�e���̃|�W�V�����̍Œ�l

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Spawn();				//�A�C�e������
};