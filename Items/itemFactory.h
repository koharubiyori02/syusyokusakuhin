//===========================================================
//
//�@�A�C�e���t�@�N�g���[
// 
//===========================================================

#pragma once
#include "item.h"
#include "speedUpItem.h"
#include "penetrateItem.h"
#include "atkUpItem.h"
#include <vector>

class ItemFactory
{
private:
	std::vector<Item*> m_Items;//�A�C�e���̎�ރ��X�g

public:
	void RoadItem()//�A�C�e���̎�ނ�o�^
	{
		m_Items.push_back(new SpeedUpItem);
		m_Items.push_back(new PenetrateItem);
		m_Items.push_back(new AtkUpItem);
	}

	//�����_���ȃA�C�e���𐶐�
	Item* CreateItem()
	{
		int num = rand() % m_Items.size();

		if (m_Items[num]->GetDuplication() == 1)
		{
			m_Items[num]->SetDuplication(2);
		}
		else if (m_Items[num]->GetDuplication() == 2)//�d���ł��Ȃ����͓̂�ڂ͏o���Ȃ�
		{
			m_Items[num] = nullptr;
			m_Items.erase(std::remove(m_Items.begin(), m_Items.end(), nullptr), m_Items.end());
			num = rand() % m_Items.size();
		}

		return m_Items[num]->GetClone();
	}

};