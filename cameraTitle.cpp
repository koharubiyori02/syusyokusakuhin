#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "CameraTitle.h"
#include "player.h"
#include "input.h"
#include "ImGUI/ImGui.h"

void CameraTitle::Init()
{
	//タイトル用の座標
	m_Position = DEFAULT_POSITION;
	m_Target = DEFAULT_TARGET;
}

void CameraTitle::Uninit()
{
	
}

void CameraTitle::Update()
{

}

void CameraTitle::Draw()
{
	XMFLOAT3 up{ 0.0f,1.0f,0.0f };
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_Target), XMLoadFloat3(&up));

	Renderer::SetViewMatrix(viewMatrix);

	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	//プロジェクションマトリクス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projectionMatrix);

	//カメラ座標設定
	Renderer::GetInstance()->SetCameraPosition(m_Position);
}

void CameraTitle::ImGuiDebug()
{
	static XMFLOAT3 pos = DEFAULT_POSITION;
	static XMFLOAT3 target = DEFAULT_TARGET;

	if (ImGui::TreeNode("CameraPosition"))
	{
		ImGui::SliderFloat("posx", &pos.x, -5.0f, 5.0f);
		ImGui::SliderFloat("posy", &pos.y, -5.0f, 5.0f);
		ImGui::SliderFloat("posz", &pos.z, -5.0f, 5.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("CameraTarget"))
	{
		ImGui::SliderFloat("targetx", &target.x, -5.0f, 5.0f);
		ImGui::SliderFloat("targety", &target.y, -5.0f, 5.0f);
		ImGui::SliderFloat("targetz", &target.z, -5.0f, 5.0f);
		ImGui::TreePop();
	}

	if (ImGui::Button("ResetCamera"))
	{
		pos = DEFAULT_POSITION;
		target = DEFAULT_TARGET;
	}

	m_Position = pos;
	m_Target = target;
}

