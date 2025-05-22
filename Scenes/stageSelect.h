//===========================================================
//
//�@�X�e�[�W�Z���N�g�V�[��
// 
//===========================================================

#pragma once
#include "scene.h"
#include "..//polygon2D.h"

#define TEXTURE_NUM 2
  
class StageSelect : public Scene
{
private:
	Polygon2D* m_StageTexture[TEXTURE_NUM];//�X�e�[�W�I���̉摜
	bool m_MoveRight = false;//�X�e�[�W�I���ŉE�ɓ����Ă��邩�ǂ���
	bool m_MoveLeft = false;//�X�e�[�W�I���ō��ɓ����Ă��邩�ǂ���
	float m_FrameCount = 0.0f;//���`��Ԃ̃J�E���g
	const float MOVE_FRAME = 6.0f;//���t���[���Ő��`��Ԃ����������邩

	int m_SelectStage = 0;//�ǂ���I�����Ă��邩
public:
	void Init() override;
	void Update() override;
	void Draw() override;
};