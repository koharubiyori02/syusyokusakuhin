#include "../polygon2D.h"
#include "../Enemy.h"
#include "../Items/speedUpItem.h"
#include "../fade.h"
#include "../input.h"
#include "StageSelect.h"
#include "../field.h"
#include "../sky.h"
#include "../bulletManager.h"
#include "Tutorial.h"

void Tutorial::Init()
{
	AddGameObject<Camera>(LayerType::TypeCamera);
	AddGameObject<Field>(LayerType::Type3D);
	AddGameObject<Sky>(LayerType::Type3D);
	AddGameObject<Player>(LayerType::Type3D, PLAYER_DEFAULT_HP);

	AddGameObject<Fade>(LayerType::Type2D, FadeType::FadeIn);//�`���[�g���A���ɑJ�ڂ����Ƃ��t�F�[�h�C������

	AudioManager* audio = Manager::GetAudio();
	audio->Stop(AudioName::BGM_StageSelect);
	audio->Play(AudioName::BGM_Tutorial);

	m_Mission = Mission::Move;//�ŏ��̃~�b�V�������Z�b�g
}

void Tutorial::Uninit()
{
	Scene::Uninit();
}

void Tutorial::Update()
{
	Scene::Update();

	Fade* fade = GetGameObject<Fade>();
	
	switch (m_Mission)
	{
	case Mission::Move:
		if (Input::GetKeyTrigger('Z'))//�`���[�g���A�����X�L�b�v
		{
			m_Mission = Mission::Shoot;
			m_FadeFinish = false;
			m_TextFadeFlag = false;
			m_FadeFrame = 0;

			AddGameObject<Polygon2D>(LayerType::Type2D, L"asset\\texture\\target.png", XMFLOAT3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0.0f),
				400.0f, 400.0f, false, "target");
		}
		break;

	case Mission::Shoot:
		if (GetGameObject<Enemy>() == nullptr)
		{
			AddGameObject<Enemy>(LayerType::Type3D, ENEMY_MAXHP, true)->SetPosition(XMFLOAT3(0.0f, 0.0f, 10.0f));//�`���[�g���A���p�̓G���o��
		}

		if (Input::GetKeyTrigger('Z'))//�`���[�g���A�����X�L�b�v
		{
			m_Mission = Mission::Item;
			m_FadeFinish = false;
			m_TextFadeFlag = false;
			m_FadeFrame = 0;
		}
		break;

	case Mission::Item:
		if (GetGameObject<Enemy>() == nullptr)
		{
			AddGameObject<Enemy>(LayerType::Type3D, ENEMY_MAXHP, true)->SetPosition(XMFLOAT3(0.0f, 0.0f, 10.0f));//�`���[�g���A���p�̓G���o��
		}

		if (GetGameObject<SpeedUpItem>() == nullptr)//�`���[�g���A���p�̃A�C�e������������Ă��Ȃ�������
		{
			m_SpawnItemFrame++;
			if (m_SpawnItemFrame > SPAWN_DELAY)
			{
				AddGameObject<SpeedUpItem>(LayerType::Type3D)->SetPosition(XMFLOAT3(-6.0f, 0.0f, 6.0f));//�`���[�g���A���p�̃A�C�e���𐶐�
				m_SpawnItemFrame = 0;
			}
		}

		if (Input::GetKeyTrigger('Z'))//�`���[�g���A�����I��
		{
			fade->SetFade(FadeType::FadeOut);//�t�F�[�h�A�E�g����
			m_Alpha = 0.0f;
		}

		if (fade->GetFinishFade())//�t�F�[�h�A�E�g���I�������
		{
			Manager::SetScene<StageSelect>();//�X�e�[�W�Z���N�g�V�[���֑J��
			fade->SetDestroy();//�t�F�[�h������
		}
		break;
	}	
}

void Tutorial::Draw()
{
	Scene::Draw();

	std::wstring text;
	float x = 0.0f;
	float y = 0.0f;

	switch (m_Mission)
	{
	case Mission::Move:
		text = L"�`���[�g���A��";
		x = 400.0f;
		y = 100.0f;

		if (m_FadeFinish)
		{
			Renderer::TextDrawFont(L"�����ł͂��̃Q�[���̊�{���w�ׂ܂��B\n�܂��͑�������Ă݂܂��傤�B", 750.0f, 30.0f, 1000.0f, 100.0f, L"Meiryo", 30.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		break;

	case Mission::Shoot:
		text = L"�e������";
		x = 500.0f;
		y = 100.0f;

		if (m_FadeFinish)
		{
			Renderer::TextDrawFont(L"���N���b�N�Œe�𔭎˂ł��܂��B\n�G�Ɍ����Č����Ă݂܂��傤�B", 750.0f, 30.0f, 1000.0f, 100.0f, L"Meiryo", 30.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		break;

	case Mission::Item:
		text = L"�A�C�e��";
		x = 500.0f;
		y = 100.0f;

		if (m_FadeFinish)
		{
			Renderer::TextDrawFont(L"�A�C�e���ɐG��邱�ƂŎ擾�ł��܂��B\n�A�C�e���̎�ނɂ���Ēe����������܂��B\n�A�C�e���͂T�܂Ŏ擾�ł��܂��B\n�擾���Č����Ă݂܂��傤�B", 680.0f, 30.0f, 1000.0f, 100.0f, L"Meiryo", 30.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		break;
	}
	
	//�������
	Renderer::TextDrawFont(
		L"      �������\n�ړ�          : WASD\n�J�������� : �}�E�X\n�e�𔭎�    : ���N���b�N\n\n\n���̃`���[�g���A�� : Z",
		50.0f, 300.0f, 1000.0f, 100.0f, L"Meiryo", 40.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	
	if (!m_FadeFinish)//�e�L�X�g�̃t�F�[�h�����Ă��Ȃ�������
	{
		TextFade();
		//�~�b�V�������߂̃^�C�g���e�L�X�g
		Renderer::TextDrawFont(text, x, y, 1000.0f, 100.0f, L"Meiryo", 70.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, m_Alpha));
	}
}