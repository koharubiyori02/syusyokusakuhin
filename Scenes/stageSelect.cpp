#include "../manager.h"
#include "../input.h"
#include "game.h"
#include "Tutorial.h"
#include "../fade.h"
#include "StageSelect.h"

void StageSelect::Init()
{
	//�X�e�[�W�I���̃`���[�g���A���̉摜
	m_StageTexture[0] = AddGameObject<Polygon2D>
		(LayerType::Type2D, L"asset\\texture\\Tutorial.png", XMFLOAT3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0.0f),
			900.0f, 500.0f, true, "tutorial");

	//�X�e�[�W�I���̃X�e�[�W1�̉摜
	m_StageTexture[1] = AddGameObject<Polygon2D>
		(LayerType::Type2D, L"asset\\texture\\Stage1.png", XMFLOAT3(SCREEN_WIDTH / 4 + 500.0f, SCREEN_HEIGHT / 4, 0.0f),
			900.0f, 500.0f, true, "stage1");

	AddGameObject<Fade>(LayerType::Type2D, FadeType::FadeIn);//�V�[���J�ڎ��Ƀt�F�[�h�C������
	
	AudioManager* audio = Manager::GetAudio();
	audio->StopAll();
	audio->Play(AudioName::BGM_StageSelect, true);
}

void StageSelect::Update()
{
	Scene::Update();

	if (!m_MoveRight && !m_MoveLeft)//�����Ă��Ȃ��Ƃ�
	{
		if (Input::GetKeyTrigger(VK_RIGHT))//�I�����E�ɓ�����
		{
			if (m_SelectStage != TEXTURE_NUM - 1)
			{
				m_MoveRight = true;
				m_SelectStage++;
			}
		}
		if (Input::GetKeyTrigger(VK_LEFT))//�I�������ɓ�����
		{
			if (m_SelectStage != 0)
			{
				m_MoveLeft = true;
				m_SelectStage--;
			}
		}		
	}

	if (m_MoveRight)//�E�ɓ����Ă���Ȃ�
	{
		m_FrameCount++;
		float t = m_FrameCount / MOVE_FRAME;

		for (int i = 0; i < TEXTURE_NUM; i++)
		{
			//���`��ԂőI���𓮂���
			XMFLOAT3 startPos = m_StageTexture[i]->GetPosition();
			XMFLOAT3 endPos = XMFLOAT3(startPos.x - 1000.0f, startPos.y, startPos.z);
			m_StageTexture[i]->SetVertexPos(startPos, endPos, t);
		}

		if (t >= 1.0f)//��Ԃ�����������
		{
			m_MoveRight = false;
			m_FrameCount = 0.0f;
		}
	}
	if (m_MoveLeft)//���ɓ����Ă���Ȃ�
	{
		m_FrameCount++;
		float t = m_FrameCount / MOVE_FRAME;

		for (int i = 0; i < TEXTURE_NUM; i++)
		{
			//���`��ԂőI���𓮂���
			XMFLOAT3 startPos = m_StageTexture[i]->GetPosition();
			XMFLOAT3 endPos = XMFLOAT3(startPos.x + 1000.0f, startPos.y, startPos.z);
			m_StageTexture[i]->SetVertexPos(startPos, endPos, t);
		}

		if (t >= 1.0f)//��Ԃ�����������
		{
			m_MoveLeft = false;
			m_FrameCount = 0.0f;
		}
	}

	Fade* fade = GetGameObject<Fade>();

	if (Input::GetKeyTrigger(VK_RETURN))//�X�y�[�X�őI��
	{
		fade->SetFade(FadeType::FadeOut);//�t�F�[�h�A�E�g����
	}

	if (fade->GetFinishFade())//�t�F�[�h�A�E�g������������
	{
		//�I�����Ă���X�e�[�W�ɑJ�ڂ���
		switch (m_SelectStage)
		{
		case 0:
			Manager::SetScene<Tutorial>();
			break;

		case 1:
			Manager::SetScene<Game>();
			break;
		}
		fade->SetDestroy();//�t�F�[�h������
	}
}

void StageSelect::Draw()
{
	Scene::Draw();

	Renderer::TextDrawFont(
		L"�\���L�[�őI���AENTER�L�[�Ō���",
		300.0f, 650.0f, 1000.0f, 100.0f, L"Meiryo", 40.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}
