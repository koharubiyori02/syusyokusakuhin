#include "main.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "Scenes//game.h"
#include "Scenes//title.h"
#include "Scenes//stageSelect.h"
#include "Scenes//tutorial.h"
#include "Scenes//result.h"
#include "Scenes/Lose.h"
#include "player.h"
#include "bulletManager.h"
#include "decoratorManager.h"
#include "effectManager.h"
#include "bossManager.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"

//static�����o�ϐ��͐錾���K�v
Scene* Manager::m_Scene{};
Scene* Manager::m_NextScene{};
AudioManager* Manager::m_Audio{};

//�I�u�W�F�N�g�̏�����
void Manager::Init()
{
	Renderer::GetInstance()->Init();
	Input::Init();
	
	m_Audio = new AudioManager;
	m_Audio->Init();

	m_Scene = new Title();
	m_Scene->Init();

	BulletManager::GetInstance()->InitBullet();

	DecoratorManager::GetInstance()->InitDecorator();

	EffectManager::GetInstance()->InitEffect();
}

//�I�u�W�F�N�g�̏I������
void Manager::Uninit()
{
	BossManager::GetInstance()->UninitBoss();

	EffectManager::GetInstance()->UninitEffect();

	DecoratorManager::GetInstance()->UninitDecorator();

	BulletManager::GetInstance()->UninitBullet();

	m_Scene->Uninit();
	delete m_Scene;

	delete m_Audio;

	Input::Uninit();
	Renderer::GetInstance()->Uninit();
}

//�I�u�W�F�N�g�̍X�V
void Manager::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Window Name");
	Input::Update();

	m_Scene->Update();

#ifdef _DEBUG
	if (ImGui::Button("Title"))
	{
		SetScene<Title>();
	}
	if (ImGui::Button("StageSelect"))
	{
		SetScene<StageSelect>();
	}
	if (ImGui::Button("Tutorial"))
	{
		SetScene<Tutorial>();
	}
	if (ImGui::Button("Game"))
	{
		SetScene<Game>();
	}
	if (ImGui::Button("Clear"))
	{
		SetScene<Result>();
	}
	if (ImGui::Button("Lose"))
	{
		SetScene<Lose>();
	}
#endif
}

//�X�V�̌�Ɏ��s�����X�V����
void Manager::LateUpdate()
{
	if (m_NextScene != nullptr)
	{
		m_Scene->Uninit();
		delete m_Scene;

		m_Scene = m_NextScene;
		m_Scene->Init();

		m_NextScene = nullptr;
	}
}

//�I�u�W�F�N�g�̕`��
void Manager::Draw()
{
	Renderer::GetInstance()->Begin();

	m_Scene->Draw();

	m_Scene->ImGuiDebug();

	ImGui::End();

#ifdef _DEBUG
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

	Renderer::GetInstance()->End();
}
