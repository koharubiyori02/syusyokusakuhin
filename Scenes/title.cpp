#include "../manager.h"
#include "../input.h"
#include "game.h"
#include "StageSelect.h"
#include "../fade.h"
#include "../cameraTitle.h"
#include "../Particle/particleTitleSelect.h"
#include "../Title/Polygon2DGameStart.h"
#include "../Title/Polygon2DExit.h"
#include "../Title/TitleCloth.h"
#include "../Title/Polygon2DEnter.h"
#include "../audioManager.h"
#include "../field.h"
#include "../sky.h"
#include "title.h"

void Title::Init()
{
	//タイトル画面に必要なものを生成する
	AddGameObject<CameraTitle>(LayerType::TypeCamera);
	AddGameObject<Field>(LayerType::Type3D);
	AddGameObject<Player>(LayerType::Type3D, 1);
	AddGameObject<Sky>(LayerType::Type3D)->SetRotation(XMFLOAT3(0.0f, 5.0f, 0.0f));
	AddGameObject<ParticleTitleSelect>(LayerType::Type2D);
	AddGameObject<Polygon2DGameStart>(LayerType::Type2D);
	AddGameObject<Polygon2DExit>(LayerType::Type2D);
	AddGameObject<TitleCloth>(LayerType::Type2D);
	AddGameObject<Polygon2DEnter>(LayerType::Type2D);
	AddGameObject<Fade>(LayerType::Type2D, FadeType::FadeIn);

	AudioManager* audio = Manager::GetAudio();
	audio->StopAll();
	audio->Play(AudioName::BGM_Title, true);
}

void Title::Update()
{
	Scene::Update();
	Fade* fade = GetGameObject<Fade>();

	TitleSelect select = GetGameObject<ParticleTitleSelect>()->GetTitleSelect();

	if (Input::GetKeyTrigger(VK_RETURN))//スペースを押したらスタート
	{
		if (select == TitleSelect::START)
		{
			fade->SetFade(FadeType::FadeOut);//フェードアウトする
		}
		else if (select == TitleSelect::EXIT)
		{
			PostQuitMessage(0);//アプリケーションの終了
		}
	}

	if (fade->GetFinishFade())//フェードアウトしたら
	{
		Manager::SetScene<StageSelect>();//ステージセレクトへ遷移
		fade->SetDestroy();//フェードを消す
	}
}