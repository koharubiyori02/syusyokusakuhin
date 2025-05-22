#include "../manager.h"
#include "../input.h"
#include "../main.h"
#include "../player.h"
#include "../ImGUI/imgui.h"
#include "playerIdle.h"
#include "PlayerMove.h"
#include "../Scenes/title.h"

void PlayerMove::Update()
{	
	PlayerState::Update();
	Player* player = GetPlayer();

	float dt = 1.0f / 60.0f;

	XMFLOAT3 forward = player->GetForward();
	XMFLOAT3 forward2 = player->GetRight();

	m_Pos = player->GetPosition();

	XMFLOAT3 oldPos = m_Pos;

	float moveSpeed = player->GetSpeed();

	if (Input::GetKeyPress('W'))
	{
		m_Vel.x += forward.x * moveSpeed * dt;
		m_Vel.z += forward.z * moveSpeed * dt;
		moveCount++;
	}
	if (Input::GetKeyPress('A'))
	{
		m_Vel.x -= forward2.x * moveSpeed * dt;
		m_Vel.z -= forward2.z * moveSpeed * dt;
		moveCount++;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Vel.x -= forward.x * moveSpeed * dt;
		m_Vel.z -= forward.z * moveSpeed * dt;
		moveCount++;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Vel.x += forward2.x * moveSpeed * dt;
		m_Vel.z += forward2.z * moveSpeed * dt;
		moveCount++;
	}

	//���C
	m_Vel.x -= m_Vel.x * 5.0f * dt;
	m_Vel.z -= m_Vel.z * 5.0f * dt;

	//�d�͉����x
	m_Vel.y += -20.0f * dt;

	//�΂߈ړ����x������
	if (moveCount >= 2)
	{
		m_Vel.x *= 0.98f;
		m_Vel.y *= 0.98f;
		m_Vel.z *= 0.98f;
	}

	m_OldPos = m_Pos;

	//�ړ�
	
	m_Pos.x += m_Vel.x * dt;
	m_Pos.y += m_Vel.y * dt;
	m_Pos.z += m_Vel.z * dt;
	
	
	float groundHeight = 0.0f;
	if (m_Pos.y < groundHeight)
	{

		m_Vel.y = 0.0f;
		m_Pos.y = groundHeight;
	}

	if (m_Pos.z <= -FIELD_MAX)
	{
		m_Vel.z = 0.0f;
		m_Pos.z = -FIELD_MAX;
	}
	if (m_Pos.z >= FIELD_MAX)
	{
		m_Vel.z = 0.0f;
		m_Pos.z = FIELD_MAX;
	}
	if (m_Pos.x <= -FIELD_MAX)
	{
		m_Vel.x = 0.0f;
		m_Pos.x = -FIELD_MAX;
	}
	if (m_Pos.x >= FIELD_MAX)
	{
		m_Vel.x = 0.0f;
		m_Pos.x = FIELD_MAX;
	}

	player->SetPosition(m_Pos);

	//�O�t���[���̍��W�Ɠ�����������Idle��Ԃɂ���
	if (static_cast<double>(m_Pos.x) == static_cast<double>(oldPos.x) && static_cast<double>(m_Pos.z) == static_cast<double>(oldPos.z))
	{
		player->ChangeState(PlayerStateName::PlayerIdle);
	}

	if (!dynamic_cast<Title*>(Manager::GetScene()))
	{
		if (Input::GetKeyTrigger(VK_LBUTTON))//�X�y�[�X�Œe������
		{
			GetPlayerShoot()->Shoot(m_Pos);//�V���[�e�B���O�X�e�[�g���A�N�e�B�u�ɂ���
		}
	}

	moveCount = 0;
	
}