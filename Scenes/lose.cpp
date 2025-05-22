#include "../manager.h"
#include "../polygon2D.h"
#include "../input.h"
#include "../fade.h"
#include "game.h"
#include "title.h"
#include "Lose.h"

void Lose::Init()
{
	//�Q�[���I�[�o�[�摜
	AddGameObject<Polygon2D>(LayerType::Type2D, L"asset\\texture\\GameOver.png", XMFLOAT3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0.0f),
		1280.0f, 725.0f, false, "bg");

	AddGameObject<Fade>(LayerType::Type2D, FadeType::FadeIn);//�V�[���J�ڎ��Ƀt�F�[�h�C������

	AudioManager* audio = Manager::GetAudio();
	audio->StopAll();
	audio->Play(AudioName::BGM_GameOver, true);
}

void Lose::Update()
{
	Scene::Update();

	Fade* fade = GetGameObject<Fade>();

	if (Input::GetKeyTrigger(VK_RETURN))//�X�y�[�X�Ń^�C�g����
	{
		fade->SetFade(FadeType::FadeOut);//�t�F�[�h�A�E�g
	}
	if (fade->GetFinishFade())//�t�F�[�h�A�E�g������������
	{
		Manager::SetScene<Title>();//�^�C�g���֑J��
		fade->SetDestroy();//�t�F�[�h������
	}
}
