#include "../manager.h"
#include "../input.h"
#include "game.h"
#include "Tutorial.h"
#include "../fade.h"
#include "StageSelect.h"

void StageSelect::Init()
{
	//ステージ選択のチュートリアルの画像
	m_StageTexture[0] = AddGameObject<Polygon2D>
		(LayerType::Type2D, L"asset\\texture\\Tutorial.png", XMFLOAT3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0.0f),
			900.0f, 500.0f, true, "tutorial");

	//ステージ選択のステージ1の画像
	m_StageTexture[1] = AddGameObject<Polygon2D>
		(LayerType::Type2D, L"asset\\texture\\Stage1.png", XMFLOAT3(SCREEN_WIDTH / 4 + 500.0f, SCREEN_HEIGHT / 4, 0.0f),
			900.0f, 500.0f, true, "stage1");

	AddGameObject<Fade>(LayerType::Type2D, FadeType::FadeIn);//シーン遷移時にフェードインする
	
	AudioManager* audio = Manager::GetAudio();
	audio->StopAll();
	audio->Play(AudioName::BGM_StageSelect, true);
}

void StageSelect::Update()
{
	Scene::Update();

	if (!m_MoveRight && !m_MoveLeft)//動いていないとき
	{
		if (Input::GetKeyTrigger(VK_RIGHT))//選択を右に動かす
		{
			if (m_SelectStage != TEXTURE_NUM - 1)
			{
				m_MoveRight = true;
				m_SelectStage++;
			}
		}
		if (Input::GetKeyTrigger(VK_LEFT))//選択を左に動かす
		{
			if (m_SelectStage != 0)
			{
				m_MoveLeft = true;
				m_SelectStage--;
			}
		}		
	}

	if (m_MoveRight)//右に動いているなら
	{
		m_FrameCount++;
		float t = m_FrameCount / MOVE_FRAME;

		for (int i = 0; i < TEXTURE_NUM; i++)
		{
			//線形補間で選択を動かす
			XMFLOAT3 startPos = m_StageTexture[i]->GetPosition();
			XMFLOAT3 endPos = XMFLOAT3(startPos.x - 1000.0f, startPos.y, startPos.z);
			m_StageTexture[i]->SetVertexPos(startPos, endPos, t);
		}

		if (t >= 1.0f)//補間が完了したら
		{
			m_MoveRight = false;
			m_FrameCount = 0.0f;
		}
	}
	if (m_MoveLeft)//左に動いているなら
	{
		m_FrameCount++;
		float t = m_FrameCount / MOVE_FRAME;

		for (int i = 0; i < TEXTURE_NUM; i++)
		{
			//線形補間で選択を動かす
			XMFLOAT3 startPos = m_StageTexture[i]->GetPosition();
			XMFLOAT3 endPos = XMFLOAT3(startPos.x + 1000.0f, startPos.y, startPos.z);
			m_StageTexture[i]->SetVertexPos(startPos, endPos, t);
		}

		if (t >= 1.0f)//補間が完了したら
		{
			m_MoveLeft = false;
			m_FrameCount = 0.0f;
		}
	}

	Fade* fade = GetGameObject<Fade>();

	if (Input::GetKeyTrigger(VK_RETURN))//スペースで選択
	{
		fade->SetFade(FadeType::FadeOut);//フェードアウトする
	}

	if (fade->GetFinishFade())//フェードアウトが完了したら
	{
		//選択しているステージに遷移する
		switch (m_SelectStage)
		{
		case 0:
			Manager::SetScene<Tutorial>();
			break;

		case 1:
			Manager::SetScene<Game>();
			break;
		}
		fade->SetDestroy();//フェードを消す
	}
}

void StageSelect::Draw()
{
	Scene::Draw();

	Renderer::TextDrawFont(
		L"十字キーで選択、ENTERキーで決定",
		300.0f, 650.0f, 1000.0f, 100.0f, L"Meiryo", 40.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}
