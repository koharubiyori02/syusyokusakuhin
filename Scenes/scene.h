//===========================================================
//
//　シーンの基底クラス
// 
//===========================================================

#pragma once

#include "..//main.h"
#include "..//gameObject.h"
#include "..//camera.h"
#include "..//player.h"
#include "..//enemy.h"
#include "..//bullet.h"
#include "..//explosion.h"
#include <list>
#include <vector>

#define LAYER_MAX 3 

class Title;

//描画の順番を付けるためのレイヤー
enum class LayerType : unsigned int
{
	TypeCamera,//カメラ
	Type3D,    //3Dオブジェクト
	Type2D,    //2Dオブジェクト
};

//ゲーム中のフェーズ
enum class Phase : unsigned int
{
	NONE,
	PHASE1,
	PHASE2,
	PHASE3
};

//基底クラス
class Scene
{
protected:
	std::list<GameObject*>m_GameObject[LAYER_MAX];
	Phase m_Phase = Phase::NONE;

	float m_Alpha = 0.0f;//文字のアルファ値
	float m_FadeFrame = 0.0f;
	bool m_TextFadeFlag = false;//文字フェード用のフラグ
	bool m_FadeFinish = false;
public:
	virtual ~Scene() {}

	virtual void Init(){}

	GameObject* Add(GameObject* obj)
	{ 
		obj->Init();
		m_GameObject[static_cast<int>(LayerType::Type3D)].push_back(obj);
		return obj;
	}

	GameObject* AddNoInit(GameObject* obj)
	{
		m_GameObject[static_cast<int>(LayerType::Type3D)].push_back(obj);
		return obj;
	}

	template <typename T, typename... Args>
	T* AddGameObject(LayerType layer, Args&&... args)
	{
		T* gameObject = new T(std::forward<Args>(args)...);
		gameObject->Init();
		m_GameObject[static_cast<int>(layer)].push_back(gameObject);

		return gameObject;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (auto p : m_GameObject[i])
			{
				T* ret = dynamic_cast<T*>(p);
				if (ret != nullptr)
					return ret;
			}
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;

		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (auto p : m_GameObject[i])
			{
				T* ret = dynamic_cast<T*>(p);
				if (ret != nullptr)
					objects.push_back(ret);
			}
		}

		return objects;
	}

	virtual void Uninit()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (auto p : m_GameObject[i])
			{
				if (!p->GetRemove())
				{
					p->Uninit();
					delete p;
				}
			}

			/*m_GameObject[i].remove_if([](GameObject* object) {
				bool frag = object->GetRemove();

				BulletDecorator* decorator = dynamic_cast<BulletDecorator*>(object);
				if (frag && decorator)
				{
					decorator->ResetDecorator();
				}

				object->SetRemove(false);
				return frag;
				});*/

			m_GameObject[i].clear();
		}		
	}

	virtual void Update()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (auto p : m_GameObject[i])
			{
				p->Update();
			}

			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });

			//デコレーターの弾はラップしている全てを非アクティブにする
			m_GameObject[i].remove_if([](GameObject* object) {
				bool frag = object->GetRemove();

				BulletDecorator* decorator = dynamic_cast<BulletDecorator*>(object);
				if (frag && decorator)
				{
					decorator->ResetDecorator();
				}

				object->SetRemove(false);
				return frag; 
			});
		}	

		ImGui::Text("objects:%d", m_GameObject[1].size());
	}

	virtual void Draw()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (auto p : m_GameObject[i])
			{
				p->Draw();
			}
		}
	}

	virtual void ImGuiDebug()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (auto p : m_GameObject[i])
			{
				p->ImGuiDebug();
			}
		}
	}

	Phase GetPhase() const { return m_Phase; }

	void TextFade()//テキストのフェード
	{
		if (m_FadeFrame == 120)
		{
			m_TextFadeFlag = true;
		}

		if (!m_TextFadeFlag)
		{
			m_FadeFrame++;
		}
		else
		{
			m_FadeFrame--;
		}

		float t = m_FadeFrame / 60.0f;

		m_Alpha = t;

		if (t == 0.0f)
		{
			m_FadeFinish = true;
			m_TextFadeFlag = false;
			m_FadeFrame = 0;
		}
	}
};