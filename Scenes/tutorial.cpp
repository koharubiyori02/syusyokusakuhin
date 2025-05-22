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

	AddGameObject<Fade>(LayerType::Type2D, FadeType::FadeIn);//チュートリアルに遷移したときフェードインする

	AudioManager* audio = Manager::GetAudio();
	audio->Stop(AudioName::BGM_StageSelect);
	audio->Play(AudioName::BGM_Tutorial);

	m_Mission = Mission::Move;//最初のミッションをセット
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
		if (Input::GetKeyTrigger('Z'))//チュートリアルをスキップ
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
			AddGameObject<Enemy>(LayerType::Type3D, ENEMY_MAXHP, true)->SetPosition(XMFLOAT3(0.0f, 0.0f, 10.0f));//チュートリアル用の敵を出す
		}

		if (Input::GetKeyTrigger('Z'))//チュートリアルをスキップ
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
			AddGameObject<Enemy>(LayerType::Type3D, ENEMY_MAXHP, true)->SetPosition(XMFLOAT3(0.0f, 0.0f, 10.0f));//チュートリアル用の敵を出す
		}

		if (GetGameObject<SpeedUpItem>() == nullptr)//チュートリアル用のアイテムが生成されていなかったら
		{
			m_SpawnItemFrame++;
			if (m_SpawnItemFrame > SPAWN_DELAY)
			{
				AddGameObject<SpeedUpItem>(LayerType::Type3D)->SetPosition(XMFLOAT3(-6.0f, 0.0f, 6.0f));//チュートリアル用のアイテムを生成
				m_SpawnItemFrame = 0;
			}
		}

		if (Input::GetKeyTrigger('Z'))//チュートリアルを終了
		{
			fade->SetFade(FadeType::FadeOut);//フェードアウトする
			m_Alpha = 0.0f;
		}

		if (fade->GetFinishFade())//フェードアウトが終わったら
		{
			Manager::SetScene<StageSelect>();//ステージセレクトシーンへ遷移
			fade->SetDestroy();//フェードを消す
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
		text = L"チュートリアル";
		x = 400.0f;
		y = 100.0f;

		if (m_FadeFinish)
		{
			Renderer::TextDrawFont(L"ここではこのゲームの基本を学べます。\nまずは操作をしてみましょう。", 750.0f, 30.0f, 1000.0f, 100.0f, L"Meiryo", 30.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		break;

	case Mission::Shoot:
		text = L"弾を撃つ";
		x = 500.0f;
		y = 100.0f;

		if (m_FadeFinish)
		{
			Renderer::TextDrawFont(L"左クリックで弾を発射できます。\n敵に向けて撃ってみましょう。", 750.0f, 30.0f, 1000.0f, 100.0f, L"Meiryo", 30.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		break;

	case Mission::Item:
		text = L"アイテム";
		x = 500.0f;
		y = 100.0f;

		if (m_FadeFinish)
		{
			Renderer::TextDrawFont(L"アイテムに触れることで取得できます。\nアイテムの種類によって弾が強化されます。\nアイテムは５つまで取得できます。\n取得して撃ってみましょう。", 680.0f, 30.0f, 1000.0f, 100.0f, L"Meiryo", 30.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		break;
	}
	
	//操作説明
	Renderer::TextDrawFont(
		L"      操作説明\n移動          : WASD\nカメラ操作 : マウス\n弾を発射    : 左クリック\n\n\n次のチュートリアル : Z",
		50.0f, 300.0f, 1000.0f, 100.0f, L"Meiryo", 40.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	
	if (!m_FadeFinish)//テキストのフェードをしていなかったら
	{
		TextFade();
		//ミッション初めのタイトルテキスト
		Renderer::TextDrawFont(text, x, y, 1000.0f, 100.0f, L"Meiryo", 70.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, m_Alpha));
	}
}