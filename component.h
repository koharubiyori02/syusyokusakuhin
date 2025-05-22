//===========================================================
//
// �R���|�[�l���g�̊��N���X
// 
//===========================================================

#pragma once

#include <list>

class Component
{
protected:
	class GameObject* m_GameObject = nullptr;	//������
	std::list<Component*> m_Component;			
		
	bool m_Active = true;						//�A�N�e�B�u��؂�ւ���

public:

	Component() = delete;
	Component(GameObject* Object) {	m_GameObject = Object; }
	virtual ~Component() {}

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

	template<typename T>
	T* CastObject()
	{
		T* gameObject = dynamic_cast<T*>(m_GameObject);
		if (gameObject != nullptr)
			return gameObject;
	}

	template <typename T>
	T* AddComponent()
	{
		T* gameObject = new T(m_GameObject);
		gameObject->Init();
		m_Component.push_back(gameObject);

		return gameObject;
	}

	template <typename T>//�e���v���[�g�֐�
	T* GetComponent()
	{
		for (auto p : m_Component)
		{
			T* ret = dynamic_cast<T*>(p);
			if (ret != nullptr)
				return ret;
		}

		return nullptr;
	}

	bool IsActive() const { return m_Active; }
	void SetActive(const bool flag) { m_Active = flag; }
};
