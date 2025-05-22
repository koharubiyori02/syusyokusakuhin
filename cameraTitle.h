//===========================================================
//
// タイトルのカメラ
// 
//===========================================================

#pragma once
#include "gameObject.h"

class CameraTitle :public GameObject
{
private:
	XMFLOAT3 m_Target{};
	XMFLOAT4X4 m_ViewMatrix{};

	XMFLOAT3 DEFAULT_POSITION = { 0.785f, 1.0f, 1.97f };
	XMFLOAT3 DEFAULT_TARGET = { -1.615f, 2.04f, 0.1f };

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ImGuiDebug() override;

	XMMATRIX GetViewMatrix()
	{
		return XMLoadFloat4x4(&m_ViewMatrix);
	}
};