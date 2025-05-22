//===========================================================
//
//�@�`���[�g���A���V�[��
// 
//===========================================================

#pragma once
#include "scene.h"
#include "..//main.h"
#include "..//polygon2D.h"

//�`���[�g���A���̃~�b�V����
enum class Mission : unsigned int
{
	Move,
	Shoot,
	Item,
};

class Tutorial : public Scene
{
private:
	Mission m_Mission = Mission::Move;//�`���[�g���A���̃~�b�V����

	int m_SpawnItemFrame = 0;//�A�C�e���X�|�[���p�̃t���[��
	const int SPAWN_DELAY = 180;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};