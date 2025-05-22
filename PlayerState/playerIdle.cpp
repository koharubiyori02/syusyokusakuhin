#include "../manager.h"
#include "../input.h"
#include "../main.h"
#include "../player.h"
#include "../ImGUI/imgui.h"
#include "PlayerMove.h"
#include "playerIdle.h"
#include "../Scenes/title.h"

void PlayerIdle::Update()
{
	if (dynamic_cast<Title*>(Manager::GetScene()))
		return;

	PlayerState::Update();
	Player* player = GetPlayer();

	//�ړ��L�[����������
	if (Input::GetKeyTrigger('W') || Input::GetKeyTrigger('A') || Input::GetKeyTrigger('S') || Input::GetKeyTrigger('D'))
	{
		player->ChangeState(PlayerStateName::PlayerMove);
		m_Active = false;
	}

	if (Input::GetKeyTrigger(VK_LBUTTON))//�X�y�[�X�Œe������
	{
		if (!GetPlayerShoot()->GetIsShooting())
		{
			GetPlayerShoot()->Shoot(player->GetPosition());
		}
	}
}
