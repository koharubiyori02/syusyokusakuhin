//===========================================================
//
// �J����
// 
//===========================================================

#pragma once
#include "gameObject.h"

class Camera :public GameObject
{
private:
	XMFLOAT3 m_Target{};

	float m_Len = 10.0f;
	float m_Pitch = 0.0f;
	float m_Yaw = 0.0f;
	bool m_Debug = false;		  //�}�E�X����ƃJ�[�\����؂�ւ���
	float m_Sensitivity = 0.001f; //���x����

	XMFLOAT4X4 m_ViewMatrix{};

	float CAMERATARGET_OFFSET_X = 4.0f;
	float CAMERATARGET_OFFSET_Y = 2.0f;
	float CAMERATARGET_OFFSET_Z = 4.0f;

	const int SHAKE_FRAME = 15;//���t���[���J������h�炷��
	int m_ShakeCount = 0;	   //���̃J�E���g
	float m_ShakeValue = 0.5f; //�h��̋���
	bool m_ShakeActive = false;//�h�炵�Ă��邩�ǂ���

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

	XMFLOAT3 RotateCamera(XMFLOAT3 target, XMFLOAT3 direction, float distance);

	void SetCamera(XMFLOAT3 vec);

	void ShakeCamera();
	void SetShakeActive(const bool active) { m_ShakeActive = active; }

	bool CheckView(XMFLOAT3 Position, XMFLOAT3 Size);
};