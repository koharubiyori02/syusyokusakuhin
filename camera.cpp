#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "input.h"
#include "boss1.h"
#include <algorithm>
#include <random>
#include "ImGUI/imgui.h"

void Camera::Init()
{
	ShowCursor(FALSE);							//カーソルを非表示
	m_Position = XMFLOAT3(0.0f, 3.0f, -5.0f);	//初期座標

#ifdef _DEBUG
	ShowCursor(TRUE);
	m_Debug = true;
#endif
}

void Camera::Uninit()
{
	
}

void Camera::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	bool flag = false;	//Rを押してデバッグになってすぐデバッグじゃなくなるのを防ぐフラグ

	Boss1* boss = scene->GetGameObject<Boss1>();

	if (!boss || !boss->GetIsDeath())
	{
		//デバッグ状態じゃなければマウスでカメラ操作
		if (!m_Debug)
		{
			if (Input::GetKeyTrigger('R'))
			{
				m_Debug = true;
				flag = true;
				ShowCursor(TRUE);
			}

			// ウィンドウ中央を計算
			POINT windowCenter;
			windowCenter.x = SCREEN_WIDTH / 2;
			windowCenter.y = SCREEN_HEIGHT / 2;

			static POINT lastMousePos = { 0, 0 };
			POINT currentMousePos;
			HWND hWnd = GetWindow();
			GetCursorPos(&currentMousePos);          // スクリーン座標で取得
			ScreenToClient(hWnd, &currentMousePos);

			float deltaX = (float)(currentMousePos.x - windowCenter.x);
			float deltaY = (float)(currentMousePos.y - windowCenter.y);

			// 次回のために位置を保存
			lastMousePos = currentMousePos;

			// 移動量を回転に反映
			m_Yaw += deltaX * m_Sensitivity;
			m_Pitch += deltaY * m_Sensitivity;
			player->SetRotation(XMFLOAT3(m_Pitch, m_Yaw, 0));
			// ピッチを制限（上下の範囲を制御）
			const float maxPitch = XM_PIDIV2 - 0.5f; // 90度に近い範囲
			m_Pitch = std::clamp(m_Pitch, -0.5f, 1.0f);

			ClientToScreen(hWnd, &windowCenter); // クライアント座標 → スクリーン座標
			SetCursorPos(windowCenter.x, windowCenter.y);
		}

		XMFLOAT3 target;
		target = player->GetPosition();
		target.y += CAMERATARGET_OFFSET_Y;
		m_Target = target;
	}
	else
	{
		XMFLOAT3 target;
		target = boss->GetPosition();
		target.y += CAMERATARGET_OFFSET_Y;
		m_Target = target;

		CAMERATARGET_OFFSET_X = 15.0f;
		CAMERATARGET_OFFSET_Y = 5.0f;
		CAMERATARGET_OFFSET_Z = 15.0f;
	}
		XMFLOAT3 oldPos = m_Position;

		m_Velocity.x = m_Target.x - CAMERATARGET_OFFSET_X * sinf(m_Yaw);
		m_Velocity.y = m_Target.y + CAMERATARGET_OFFSET_Y * tanf(m_Pitch);
		m_Velocity.z = m_Target.z - CAMERATARGET_OFFSET_Z * cosf(m_Yaw);

		m_Position.x = m_Velocity.x;
		m_Position.y = m_Velocity.y;
		m_Position.z = m_Velocity.z;

		if (m_Position.x >= FIELD_MAX || m_Position.x <= -FIELD_MAX)
		{
			m_Position.x = oldPos.x;
		}
		if (m_Position.z >= FIELD_MAX || m_Position.z <= -FIELD_MAX)
		{
			m_Position.z = oldPos.z;
		}
	

	if (m_ShakeActive)
	{
		m_ShakeCount++;

		ShakeCamera();

		if (m_ShakeCount == SHAKE_FRAME)
		{
			m_ShakeCount = 0;
			m_ShakeActive = false;
		}
	}

#ifdef _DEBUG
	if (m_Debug)
	{
		if(!flag)
		{
			if (Input::GetKeyTrigger('R'))
			{
				m_Debug = false;
				ShowCursor(FALSE);
			}
		}
	}
#endif
}

void Camera::Draw()
{
	XMFLOAT3 up{ 0.0f,1.0f,0.0f };
	XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR upp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// 回転行列を計算
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, 0.0f);

	// カメラの方向を更新
	XMVECTOR lookAt = XMVector3TransformCoord(forward, rotationMatrix);
	XMVECTOR camPos = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f); // カメラ位置
	XMVECTOR camTarget = XMLoadFloat3(&m_Position) + lookAt;
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), camTarget, upp);

	Renderer::SetViewMatrix(viewMatrix);

	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	//プロジェクションマトリクス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projectionMatrix);

	//カメラ座標設定
	Renderer::SetCameraPosition(m_Position);
}

void Camera::ImGuiDebug()
{
	ImGui::SliderFloat("CameraSpeed", &m_Sensitivity, 0.001f, 0.01f);
}

XMFLOAT3 Camera::RotateCamera(XMFLOAT3 target, XMFLOAT3 direction, float distance)
{
	float length = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
	XMFLOAT3 normal = XMFLOAT3(direction.x / length, direction.y / length, direction.z / length);

	XMFLOAT3 pos;
	pos.x = target.x + normal.x * distance;
	pos.y = target.y + normal.y * distance;
	pos.z = target.z + normal.z * distance;

	return XMFLOAT3(pos);
}

void Camera::SetCamera(XMFLOAT3 vec)
{
	XMFLOAT3 vector;
	vector.x = -vec.x;
	vector.y = -vec.y;
	vector.z = -vec.z;

	m_Position.x = m_Target.x + vector.x * 5;
	m_Position.z = m_Target.z + vector.z * 5;
}

void Camera::ShakeCamera()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	float offsetX = dist(gen) * m_ShakeValue;
	float offsetY = dist(gen) * m_ShakeValue;

	m_Position.x += offsetX;
	m_Position.y += offsetY;
}

//カメラにオブジェクトが映っているかどうか
bool Camera::CheckView(XMFLOAT3 Position, XMFLOAT3 Size)
{
	XMFLOAT3 up{ 0.0f,1.0f,0.0f };

	XMMATRIX viewMatrix = GetViewMatrix();
	//プロジェクションマトリクス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	XMMATRIX vp, invvp;

	vp = viewMatrix * projectionMatrix;
	invvp = XMMatrixInverse(nullptr, vp);

	XMFLOAT3 vpos[4];
	XMFLOAT3 wpos[4];

	vpos[0] = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	vpos[1] = XMFLOAT3(1.0f, 1.0f, 1.0f);
	vpos[2] = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	vpos[3] = XMFLOAT3(1.0f, -1.0f, 1.0f);

	XMVECTOR vposv[4];
	XMVECTOR wposv[4];

	vposv[0] = XMLoadFloat3(&vpos[0]);
	vposv[1] = XMLoadFloat3(&vpos[1]);
	vposv[2] = XMLoadFloat3(&vpos[2]);
	vposv[3] = XMLoadFloat3(&vpos[3]);

	wposv[0] = XMVector3TransformCoord(vposv[0], invvp);
	wposv[1] = XMVector3TransformCoord(vposv[1], invvp);
	wposv[2] = XMVector3TransformCoord(vposv[2], invvp);
	wposv[3] = XMVector3TransformCoord(vposv[3], invvp);

	XMStoreFloat3(&wpos[0], wposv[0]);
	XMStoreFloat3(&wpos[1], wposv[1]);
	XMStoreFloat3(&wpos[2], wposv[2]);
	XMStoreFloat3(&wpos[3], wposv[3]);

	XMFLOAT3 v, v1, v2, n;

	v.x = Position.x - m_Position.x;
	v.y = Position.y - m_Position.y;
	v.z = Position.z - m_Position.z;

	//左面判定
	{
		v1.x = wpos[0].x - m_Position.x;
		v1.y = wpos[0].y - m_Position.y;
		v1.z = wpos[0].z - m_Position.z;

		v2.x = wpos[2].x - m_Position.x;
		v2.y = wpos[2].y - m_Position.y;
		v2.z = wpos[2].z - m_Position.z;

		//外積
		n.x = v1.y * v2.z - v1.z * v2.y;
		n.y = v1.z * v2.x - v1.x * v2.z;
		n.z = v1.x * v2.y - v1.y * v2.x;

		//nの正規化
		float len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
		n.x /= len;
		n.y /= len;
		n.z /= len;

		//内積
		float dot = n.x * v.x + n.y * v.y + n.z * v.z;
		if (dot < -Size.x * 2)
		{
			return false;
		}
	}

	//右面判定
	{
		v1.x = wpos[3].x - m_Position.x;
		v1.y = wpos[3].y - m_Position.y;
		v1.z = wpos[3].z - m_Position.z;

		v2.x = wpos[1].x - m_Position.x;
		v2.y = wpos[1].y - m_Position.y;
		v2.z = wpos[1].z - m_Position.z;

		//外積
		n.x = v1.y * v2.z - v1.z * v2.y;
		n.y = v1.z * v2.x - v1.x * v2.z;
		n.z = v1.x * v2.y - v1.y * v2.x;

		//nの正規化
		//nの正規化
		float len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
		n.x /= len;
		n.y /= len;
		n.z /= len;

		//内積
		float dot = n.x * v.x + n.y * v.y + n.z * v.z;
		if (dot < -Size.x * 2)
		{
			return false;
		}
	}

	//上面判定
	{
		v1.x = wpos[1].x - m_Position.x;
		v1.y = wpos[1].y - m_Position.y;
		v1.z = wpos[1].z - m_Position.z;

		v2.x = wpos[0].x - m_Position.x;
		v2.y = wpos[0].y - m_Position.y;
		v2.z = wpos[0].z - m_Position.z;

		//外積
		n.x = v1.y * v2.z - v1.z * v2.y;
		n.y = v1.z * v2.x - v1.x * v2.z;
		n.z = v1.x * v2.y - v1.y * v2.x;

		//nの正規化
		//nの正規化
		float len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
		n.x /= len;
		n.y /= len;
		n.z /= len;

		//内積
		float dot = n.x * v.x + n.y * v.y + n.z * v.z;

		if (dot < 0.0f - Size.y)
		{
			return false;
		}
	}

	//下面判定
	{
		v1.x = wpos[2].x - m_Position.x;
		v1.y = wpos[2].y - m_Position.y;
		v1.z = wpos[2].z - m_Position.z;

		v2.x = wpos[3].x - m_Position.x;
		v2.y = wpos[3].y - m_Position.y;
		v2.z = wpos[3].z - m_Position.z;

		//外積
		n.x = v1.y * v2.z - v1.z * v2.y;
		n.y = v1.z * v2.x - v1.x * v2.z;
		n.z = v1.x * v2.y - v1.y * v2.x;

		//nの正規化
		//nの正規化
		float len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
		n.x /= len;
		n.y /= len;
		n.z /= len;

		//内積
		float dot = n.x * v.x + n.y * v.y + n.z * v.z;

		if (dot < -Size.y * 2)
		{
			return false;
		}
	}

	return true;
}