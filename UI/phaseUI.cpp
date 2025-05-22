#include "..//manager.h"
#include "..//input.h"
#include "PhaseUI.h"
#include <sstream>

void PhaseUI::Init()
{
	
}

void PhaseUI::Uninit()
{
	
}

void PhaseUI::Update()
{
	Scene* scene = Manager::GetScene();
	scene->GetPhase();

	//�t�F�[�Y�ɉ����Đ�����ς���
	switch (scene->GetPhase())
	{
	case Phase::PHASE1:
		m_PhaseNum = 1;
		break;

	case Phase::PHASE2:
		m_PhaseNum = 2;
		break;

	case Phase::PHASE3:
		m_PhaseNum = 3;
		break;
	}
}

void PhaseUI::Draw()
{
	//�t�F�[�Y��`��
	std::wstring text = L"�t�F�[�Y";
	std::wstringstream wss;
	wss << m_PhaseNum;
	text = text + wss.str();
	Renderer::TextDrawFont(text, 30.0f, 30.0f, 1000.0f, 100.0f, L"Meiryo", 40.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}