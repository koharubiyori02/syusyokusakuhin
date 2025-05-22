#include "../manager.h"
#include "../input.h"
#include "../fade.h"
#include "title.h"
#include "../polygon2D.h"
#include "result.h"

void Result::Init()
{
	//ステージクリア画像
	AddGameObject<Polygon2D>(LayerType::Type2D, L"asset\\texture\\Clear.png", XMFLOAT3(SCREEN_WIDTH/4, SCREEN_HEIGHT/4, 0.0f),
		1280.0f, 725.0f, false, "bg");

	AddGameObject<Fade>(LayerType::Type2D, FadeType::FadeIn);//シーン遷移時にフェードインする

	AudioManager* audio = Manager::GetAudio();
	audio->StopAll();
	audio->Play(AudioName::BGM_GameClear, true);
}

void Result::Update()
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