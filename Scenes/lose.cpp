#include "../manager.h"
#include "../polygon2D.h"
#include "../input.h"
#include "../fade.h"
#include "game.h"
#include "title.h"
#include "Lose.h"

void Lose::Init()
{
	//ゲームオーバー画像
	AddGameObject<Polygon2D>(LayerType::Type2D, L"asset\\texture\\GameOver.png", XMFLOAT3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0.0f),
		1280.0f, 725.0f, false, "bg");

	AddGameObject<Fade>(LayerType::Type2D, FadeType::FadeIn);//シーン遷移時にフェードインする

	AudioManager* audio = Manager::GetAudio();
	audio->StopAll();
	audio->Play(AudioName::BGM_GameOver, true);
}

void Lose::Update()
{
	Scene::Update();

	Fade* fade = GetGameObject<Fade>();

	if (Input::GetKeyTrigger(VK_RETURN))//スペースでタイトルへ
	{
		fade->SetFade(FadeType::FadeOut);//フェードアウト
	}
	if (fade->GetFinishFade())//フェードアウトが完了したら
	{
		Manager::SetScene<Title>();//タイトルへ遷移
		fade->SetDestroy();//フェードを消す
	}
}
