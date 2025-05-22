#pragma once

#include "Scenes//scene.h"
#include "Scenes//title.h"
#include "audioManager.h"

class Manager
{
private:
	static Scene* m_Scene;
	static Scene* m_NextScene;
	static AudioManager* m_Audio;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void LateUpdate();
	static void Draw();

	static Scene* GetScene() { return m_Scene; }
	static AudioManager* GetAudio() { return m_Audio; }
	
	template<typename T>
	static void SetScene()
	{
		m_NextScene = new T();
	}
};