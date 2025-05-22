/////////////////////////////////////////////////////////////
//
//　ゲームオブジェクトの基底クラス
// 
/////////////////////////////////////////////////////////////

#pragma once

#define FIELD_MAX 50.0f
#define FIELD_SIZE 50.0f

#include "main.h"
#include <List>
#include <string>
#include "collision.h"

class Component;
class Scene;
class Manager;

class GameObject
{
private:

protected:
	std::list<Component*> m_Component;

	XMFLOAT3 m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMFLOAT3 m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	bool m_Destroy = false;						//削除フラグ
	bool m_Remove = false;						//リストから外すフラグ

	int m_Hp = {};								//HP
	int m_Atk = {};								//攻撃力

public:

	virtual ~GameObject() {}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void ImGuiDebug()  {};

	XMFLOAT3 GetPosition() const { return m_Position; }
	XMFLOAT3 GetRotation() const { return m_Rotation; }
	XMFLOAT3 GetScale()const { return m_Scale; }
	XMFLOAT3 GetVelocity() const { return m_Velocity; }

	void SetPosition(const XMFLOAT3 pos) { m_Position = pos; }
	void SetRotation(const XMFLOAT3 rot) { m_Rotation = rot; }
	void SetScale(const XMFLOAT3 scl)	 { m_Scale = scl; }
	void SetVelocity(const XMFLOAT3 vel) { m_Velocity = vel; }

	int GetHp() const { return m_Hp; }
	void SetHp(const int hp) { m_Hp = hp; }
	int GetAtk() const { return m_Atk; }
	void SetAtk(const int atk) { m_Atk = atk; }

	void SetDestroy() { m_Destroy = true; }
	bool const GetDestroy() { return m_Destroy; }

	void SetRemove(bool frag) { m_Remove = frag; }
	bool const GetRemove() { return m_Remove; }

	bool Destroy()					//削除フラグがtrueなら削除
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename T>//テンプレート関数
	T* AddComponent()
	{
		T* gameObject = new T(this);
		gameObject->Init();
		m_Component.push_back(gameObject);

		return gameObject;
	}

	template <typename T>//テンプレート関数
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

	XMFLOAT3 GetForward() const //前方向ベクトル取得
	{
		//オイラー角から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 forward;
		XMStoreFloat3(&forward, rotationMatrix.r[2]);
		return forward;
	}

	XMFLOAT3 GetRight() const //横方向ベクトル取得
	{
		//オイラー角から回転行列を生成
		XMMATRIX rotationMatrix;
 		rotationMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
		
		XMFLOAT3 forward;
		XMStoreFloat3(&forward, rotationMatrix.r[0]);
		return forward;
	}

	XMFLOAT3 GetUp() const //上方向ベクトル取得
	{
		//オイラー角から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 forward;
		XMStoreFloat3(&forward, rotationMatrix.r[1]);
		return forward;
	}

	void AddDamage(const int atk)		//このオブジェクトにダメージを与える
	{
		m_Hp -= atk;
		
		if (m_Hp <= 0)
			m_Hp = 0;
	}
};
