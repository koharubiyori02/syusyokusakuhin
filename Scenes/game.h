//===========================================================
//
//�@�Q�[���V�[��
// 
//===========================================================

#pragma once
#include "scene.h"

class Game : public Scene
{
private:
	//��Q���̍��W
	std::vector<XMFLOAT3> m_BoxPositions = {
		XMFLOAT3(-20.0f, 0, 10.0f),
		XMFLOAT3(20.0f, 0, 10.0f),
		XMFLOAT3(20.0f, 0, -10.0f),
		XMFLOAT3(-20.0f, 0, -10.0f),
		XMFLOAT3(10.0f, 0, 0.0f),
		XMFLOAT3(-10.0f, 0, 0.0f),
		XMFLOAT3(0.0f, 0, 20.0f),
		XMFLOAT3(20.0f, 0, -30.0f),
		XMFLOAT3(-20.0f, 0, -30.0f),
		XMFLOAT3(0.0f, 0, -30.0f)
	};

public:
	void Init() override;
	void Update() override;
	void Draw() override;

	void ResetGame();			//�t�F�[�Y���؂�ւ��Ƃ��ɌĂ�

};