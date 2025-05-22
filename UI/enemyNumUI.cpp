#include "..//main.h"
#include "..//renderer.h"
#include "..//manager.h"
#include "EnemyNumUI.h"
#include "..//player.h"
#include "..//enemy.h"
#include "..//EnemySpawner.h"
#include <sstream>

void EnemyNumUI::Init()
{
	
}

void EnemyNumUI::Uninit()
{

}

void EnemyNumUI::Update()
{
	
}

void EnemyNumUI::Draw()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	int enemyNum = 0;

	switch (scene->GetPhase())
	{
	case Phase::PHASE1:
		enemyNum = ENEMY_MAX1-player->GetKillCount();
		break;

	case Phase::PHASE2:
		enemyNum = ENEMY_MAX2 - player->GetKillCount();
		break;

	case Phase::PHASE3:
		break;
	}

	std::wstring text = L"c‚è“G”:";
	std::wstringstream wss;
	wss << enemyNum;
	text = text + wss.str();
	Renderer::TextDrawFont(text, 550.0f, 30.0f, 1000.0f, 100.0f, L"Meiryo", 40.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}
