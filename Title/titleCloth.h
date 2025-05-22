//===========================================================
//
// タイトルにある旗
// 
//===========================================================

#pragma once
#include "..//gameObject.h"
#include "..//renderer.h"

//----- 定数定義

#define AXIS_X_OFFSET			SCREEN_WIDTH/2	// Ｘ座標オフセット
#define AXIS_Y_OFFSET			SCREEN_HEIGHT/2	// Ｙ座標オフセット

#define PARTICLE_HORIZONTAL		20				// 粒子　縦方向総数
#define PARTICLE_VERTICAL		20				// 粒子　横方向総数

#define SPRING_NUMS				(PARTICLE_HORIZONTAL*(PARTICLE_VERTICAL+1) + \
								 PARTICLE_VERTICAL*(PARTICLE_HORIZONTAL+1) + \
								 PARTICLE_VERTICAL*PARTICLE_HORIZONTAL*2)	// スプリング総数

#define SPRING_DEFAULT_LENGTH	40.0f			// バネ自然長
#define SPRING_COEFFICIENT		40.0f			// バネ係数
#define SPRING_MASS				2.0f			// バネ質量

#define ATT_COEFFICIENT			0.5f			// 減衰係数(Attenuation)
#define DELTA_TIME				0.1f			// 積分単位時間

#define WIND_FORCE_DEF_X		50.0f			// 風力Ｘ成分
#define WIND_FORCE_DEF_Y		-20.0f			// 風力Ｙ成分
#define WIND_FORCE_DEF_Z		0.0f			// 風力Ｚ成分

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
	int		horz;				// 横方向インデックス
	int		vert;				// 縦方向インデックス
};

struct WINDFORCE {
	int		status;				// ステータス
	XMFLOAT3	force;				// 風力ベクトル
};

struct SPRING {
	P_REF	p1;					// 接続されている粒子１
	P_REF	p2;					// 接続されている粒子２
	float	length;				// 自然長
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