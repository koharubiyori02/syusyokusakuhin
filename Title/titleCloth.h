//===========================================================
//
// �^�C�g���ɂ����
// 
//===========================================================

#pragma once
#include "..//gameObject.h"
#include "..//renderer.h"

//----- �萔��`

#define AXIS_X_OFFSET			SCREEN_WIDTH/2	// �w���W�I�t�Z�b�g
#define AXIS_Y_OFFSET			SCREEN_HEIGHT/2	// �x���W�I�t�Z�b�g

#define PARTICLE_HORIZONTAL		20				// ���q�@�c��������
#define PARTICLE_VERTICAL		20				// ���q�@����������

#define SPRING_NUMS				(PARTICLE_HORIZONTAL*(PARTICLE_VERTICAL+1) + \
								 PARTICLE_VERTICAL*(PARTICLE_HORIZONTAL+1) + \
								 PARTICLE_VERTICAL*PARTICLE_HORIZONTAL*2)	// �X�v�����O����

#define SPRING_DEFAULT_LENGTH	40.0f			// �o�l���R��
#define SPRING_COEFFICIENT		40.0f			// �o�l�W��
#define SPRING_MASS				2.0f			// �o�l����

#define ATT_COEFFICIENT			0.5f			// �����W��(Attenuation)
#define DELTA_TIME				0.1f			// �ϕ��P�ʎ���

#define WIND_FORCE_DEF_X		50.0f			// ���͂w����
#define WIND_FORCE_DEF_Y		-20.0f			// ���͂x����
#define WIND_FORCE_DEF_Z		0.0f			// ���͂y����

struct VERTEX
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;

	int status;
	XMFLOAT3 velocity;
	XMFLOAT3 acceleration;
	XMFLOAT3 resultant;
	XMFLOAT3 gravity;
	bool onLock;
};

struct P_REF {
	int		horz;				// �������C���f�b�N�X
	int		vert;				// �c�����C���f�b�N�X
};

struct WINDFORCE {
	int		status;				// �X�e�[�^�X
	XMFLOAT3	force;				// ���̓x�N�g��
};

struct SPRING {
	P_REF	p1;					// �ڑ�����Ă��闱�q�P
	P_REF	p2;					// �ڑ�����Ă��闱�q�Q
	float	length;				// ���R��
};

class TitleCloth : public GameObject
{
private:
	ID3D11ShaderResourceView* m_Texture{};
	ID3D11ShaderResourceView* m_EnvTexture{};

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_IndexBuffer{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	VERTEX m_vertex[PARTICLE_HORIZONTAL+1][PARTICLE_VERTICAL+1];

	

	SPRING m_Spring[PARTICLE_HORIZONTAL * (PARTICLE_VERTICAL + 1) + \
		PARTICLE_VERTICAL * (PARTICLE_HORIZONTAL + 1) + \
		PARTICLE_VERTICAL * PARTICLE_HORIZONTAL * 2];

	int m_Status;

	WINDFORCE m_Wind;
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ImGuiDebug() override;
};