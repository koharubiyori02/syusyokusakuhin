#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <dwrite.h>
#include <d2d1.h>
#include <string>
#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

struct VERTEX_3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};



struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2];
};



struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	XMFLOAT4	Direction;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Ambient;
};



class Renderer
{
private:
	//static Renderer*				m_renderer;

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device* m_Device;
	static ID3D11DeviceContext* m_DeviceContext;
	static IDXGISwapChain* m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer* m_WorldBuffer;
	static ID3D11Buffer* m_ViewBuffer;
	static ID3D11Buffer* m_ProjectionBuffer;
	static ID3D11Buffer* m_MaterialBuffer;
	static ID3D11Buffer* m_LightBuffer;
	static ID3D11Buffer* m_CameraBuffer;
	static ID3D11Buffer* m_IsHitCameraBuffer;

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static ID3D11BlendState* m_BlendState;
	static ID3D11BlendState* m_BlendStateAdd;//加算豪税ブレンドステート
	static ID3D11BlendState* m_BlendStateATC;
	static ID3D11BlendState* m_BlendStateMask;

	static ID2D1Factory* m_D2DFactory;
	static IDWriteFactory* m_DWriteFactory;
	static ID2D1RenderTarget* m_D2DRenderTarget;
	static IDWriteTextFormat* m_TextFormat;

	static ID3D11DepthStencilState* m_StencilWrite;
	static ID3D11DepthStencilState* m_StencilRead;

	static ID3D11RasterizerState* m_RasterizerStateCullBack;
	static ID3D11RasterizerState* m_RasterizerStateCullNone;

	static float m_ZoomFactor;
	static float m_TargetZoomFactor;
	static float m_ZoomDuration;
	static float m_ZoomTimer;
	static bool m_IsZooming;

public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetBlendAddEnable(bool Enable);
	static void SetBlendMaskEnable(bool Enable);
	static void SetCullEnable(bool Enable);

	static void SetATCEnable(bool Enable);
	static void SetStencilEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(XMMATRIX WorldMatrix);
	static void SetViewMatrix(XMMATRIX ViewMatrix);
	static void SetProjectionMatrix(XMMATRIX ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetCameraPosition(XMFLOAT3 CameraPosition);
	static void SetHitCamera(float alpha);

	static ID3D11Device* GetDevice(void) { return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext(void) { return m_DeviceContext; }



	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	//void SetCameraPosition(XMFLOAT3 CameraPosition);

	static void TextDrawFont(const std::wstring& text, float x, float y, float width, float height, const std::wstring& fontName, float fontSize, XMFLOAT4 color);

	static Renderer* GetInstance()
	{
		static Renderer renderer;
		return &renderer;
	}

};
