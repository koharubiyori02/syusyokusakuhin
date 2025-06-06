
#include "main.h"
#include "renderer.h"
#include <io.h>


D3D_FEATURE_LEVEL       Renderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device* Renderer::m_Device{};
ID3D11DeviceContext* Renderer::m_DeviceContext{};
IDXGISwapChain* Renderer::m_SwapChain{};
ID3D11RenderTargetView* Renderer::m_RenderTargetView{};
ID3D11DepthStencilView* Renderer::m_DepthStencilView{};

ID3D11Buffer* Renderer::m_WorldBuffer{};
ID3D11Buffer* Renderer::m_ViewBuffer{};
ID3D11Buffer* Renderer::m_ProjectionBuffer{};
ID3D11Buffer* Renderer::m_MaterialBuffer{};
ID3D11Buffer* Renderer::m_LightBuffer{};
ID3D11Buffer* Renderer::m_CameraBuffer{};
ID3D11Buffer* Renderer::m_IsHitCameraBuffer{};

ID3D11DepthStencilState* Renderer::m_DepthStateEnable{};
ID3D11DepthStencilState* Renderer::m_DepthStateDisable{};


ID3D11BlendState* Renderer::m_BlendState{};
ID3D11BlendState* Renderer::m_BlendStateAdd{};
ID3D11BlendState* Renderer::m_BlendStateATC{};
ID3D11BlendState* Renderer::m_BlendStateMask;

ID2D1Factory* Renderer::m_D2DFactory{};
IDWriteFactory* Renderer::m_DWriteFactory{};
ID2D1RenderTarget* Renderer::m_D2DRenderTarget{};
IDWriteTextFormat* Renderer::m_TextFormat{};

ID3D11DepthStencilState* Renderer::m_StencilWrite;
ID3D11DepthStencilState* Renderer::m_StencilRead;

ID3D11RasterizerState* Renderer::m_RasterizerStateCullBack;
ID3D11RasterizerState* Renderer::m_RasterizerStateCullNone;

float Renderer::m_ZoomFactor = 1.0f;
float Renderer::m_TargetZoomFactor = 1.0f;
float Renderer::m_ZoomDuration = 0.0f;
float Renderer::m_ZoomTimer = 0.0f;
bool Renderer::m_IsZooming = false;


void Renderer::Init()
{
	HRESULT hr = S_OK;

	IDXGIAdapter* pAdapter;
	IDXGIFactory* pFactory;

	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	pFactory->EnumAdapters(0, &pAdapter);


	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetWindow();
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(pAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_SwapChain,
		&m_Device,
		&m_FeatureLevel,
		&m_DeviceContext);






	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget{};
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	m_Device->CreateRenderTargetView(renderTarget, NULL, &m_RenderTargetView);
	renderTarget->Release();


	// デプスステンシルバッファ作成
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	m_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Flags = 0;
	m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
	depthStencile->Release();


	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);





	// ビューポート設定
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)SCREEN_WIDTH;
	viewport.Height = (FLOAT)SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &viewport);



	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	ID3D11RasterizerState* rs;
	m_Device->CreateRasterizerState(&rasterizerDesc, &rs);

	m_DeviceContext->RSSetState(rs);




	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			//前景*A
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	//背景*(1-A)
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_Device->CreateBlendState(&blendDesc, &m_BlendState);

	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;//背景*1
	m_Device->CreateBlendState(&blendDesc, &m_BlendStateAdd);
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	blendDesc.AlphaToCoverageEnable = TRUE;
	m_Device->CreateBlendState(&blendDesc, &m_BlendStateATC);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);





	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//深度無効ステート

	m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);




	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = 10.0f;
	samplerDesc.MipLODBias = 0.0f;

	ID3D11SamplerState* samplerState{};
	m_Device->CreateSamplerState(&samplerDesc, &samplerState);

	m_DeviceContext->PSSetSamplers(0, 1, &samplerState);



	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_WorldBuffer);
	m_DeviceContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ViewBuffer);
	m_DeviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ProjectionBuffer);
	m_DeviceContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);

	bufferDesc.ByteWidth = sizeof(MATERIAL);
	m_Device->CreateBuffer(&bufferDesc, NULL, &m_CameraBuffer);
	m_DeviceContext->PSSetConstantBuffers(5, 1, &m_CameraBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_IsHitCameraBuffer);
	m_DeviceContext->PSSetConstantBuffers(6, 1, &m_IsHitCameraBuffer);

	bufferDesc.ByteWidth = sizeof(MATERIAL);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_MaterialBuffer);
	m_DeviceContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);
	m_DeviceContext->PSSetConstantBuffers(3, 1, &m_MaterialBuffer);


	bufferDesc.ByteWidth = sizeof(LIGHT);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_LightBuffer);
	m_DeviceContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);
	m_DeviceContext->PSSetConstantBuffers(4, 1, &m_LightBuffer);





	// ライト初期化
	LIGHT light{};
	light.Enable = true;
	light.Direction = XMFLOAT4(1.0f, -0.5f, 1.0f, 0.0f);
	light.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f);
	SetLight(light);



	// マテリアル初期化
	MATERIAL material{};
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	SetMaterial(material);


	//  DirectWriteのInit 
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_D2DFactory);
	if (FAILED(hr)) {
		std::cerr << "Failed to create D2D factory: " << hr << std::endl;
		return;
	}

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_DWriteFactory));
	if (FAILED(hr)) {
		std::cerr << "Failed to create DWrite factory: " << hr << std::endl;
		return;
	}

	IDXGISurface* dxgiSurface = nullptr;
	hr = m_SwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&dxgiSurface);
	if (FAILED(hr)) {
		std::cerr << "Failed to get IDXGISurface from swap chain: " << hr << std::endl;
		return;
	}

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
	props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	props.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);

	hr = m_D2DFactory->CreateDxgiSurfaceRenderTarget(dxgiSurface, &props, &m_D2DRenderTarget);
	dxgiSurface->Release();
	if (FAILED(hr)) {
		std::cerr << "Failed to create DxgiSurfaceRenderTarget: " << std::hex << hr << std::endl; // Output error code in hex
		return;
	}

	hr = m_DWriteFactory->CreateTextFormat(L"Arial", nullptr, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 36.0f, L"en-us", &m_TextFormat);
	if (FAILED(hr)) {
		std::cerr << "Failed to create text format: " << std::hex << hr << std::endl; // Output error code in hex
		return;
	}

}



void Renderer::Uninit()
{

	m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_MaterialBuffer->Release();


	m_DeviceContext->ClearState();
	m_RenderTargetView->Release();
	m_SwapChain->Release();
	m_DeviceContext->Release();
	m_Device->Release();

}




void Renderer::Begin()
{
	float clearColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}



void Renderer::End()
{
	m_SwapChain->Present(0, 0);
}




void Renderer::SetDepthEnable(bool Enable)
{
	if (Enable)
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
	else
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateDisable, NULL);

}

void Renderer::SetBlendAddEnable(bool Enable)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		m_DeviceContext->OMSetBlendState(m_BlendStateAdd, blendFactor, 0xffffffff);
	else
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);

}

void Renderer::SetBlendMaskEnable(bool Enable)
{
	float blendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };

	if (Enable)
		m_DeviceContext->OMSetBlendState(m_BlendStateMask, blendFactor, 0xffffffff);
	else
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);
}

void Renderer::SetCullEnable(bool Enable)
{
	if (Enable)
		m_DeviceContext->RSSetState(m_RasterizerStateCullBack);
	else
		m_DeviceContext->RSSetState(m_RasterizerStateCullNone);
}

//アルファトゥカバレッジ
void Renderer::SetATCEnable(bool Enable)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		m_DeviceContext->OMSetBlendState(m_BlendStateATC, blendFactor, 0xffffffff);
	else
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);

}

void Renderer::SetStencilEnable(bool Enable)
{
	if (Enable)
		m_DeviceContext->OMSetDepthStencilState(m_StencilWrite, NULL);
	else
		m_DeviceContext->OMSetDepthStencilState(m_StencilRead, NULL);
}

void Renderer::SetWorldViewProjection2D()
{
	SetWorldMatrix(XMMatrixIdentity());
	SetViewMatrix(XMMatrixIdentity());

	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);
}


void Renderer::SetWorldMatrix(XMMATRIX WorldMatrix)
{
	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, XMMatrixTranspose(WorldMatrix));
	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &worldf, 0, 0);
}

void Renderer::SetViewMatrix(XMMATRIX ViewMatrix)
{
	XMFLOAT4X4 viewf;
	XMStoreFloat4x4(&viewf, XMMatrixTranspose(ViewMatrix));
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &viewf, 0, 0);
}

void Renderer::SetProjectionMatrix(XMMATRIX ProjectionMatrix)
{
	XMFLOAT4X4 projectionf;
	XMStoreFloat4x4(&projectionf, XMMatrixTranspose(ProjectionMatrix));
	m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projectionf, 0, 0);

}



void Renderer::SetMaterial(MATERIAL Material)
{
	m_DeviceContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &Material, 0, 0);
}

void Renderer::SetLight(LIGHT Light)
{
	m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
}





void Renderer::CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName)
{

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_Device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}



void Renderer::CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}

void Renderer::SetCameraPosition(XMFLOAT3 CameraPosition)
{
	XMFLOAT4 cameraPos;
	cameraPos.x = CameraPosition.x;
	cameraPos.y = CameraPosition.y;
	cameraPos.z = CameraPosition.z;
	cameraPos.w = 0.0f;

	m_DeviceContext->UpdateSubresource(m_CameraBuffer, 0, NULL, &cameraPos, 0, 0);
}

void Renderer::SetHitCamera(float alpha)
{
	m_DeviceContext->UpdateSubresource(m_IsHitCameraBuffer, 0, NULL, &alpha, 0, 0);
}

void Renderer::TextDrawFont(const std::wstring& text, float x, float y, float width, float height, const std::wstring& fontName, float fontSize, XMFLOAT4 color)
{
	HRESULT hr;

	// テキストフォーマットの作成
	IDWriteTextFormat* textFormat;
	hr = m_DWriteFactory->CreateTextFormat(
		fontName.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_EXTRA_BLACK,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"en-us",
		&textFormat
	);
	if (FAILED(hr)) {
		std::cerr << "Failed to create text format: " << hr << std::endl;
		return;
	}

	D2D1_RECT_F layoutRect = D2D1::RectF(x, y, x + width, y + height);

	// colorのXMFLOAT4をD2D1::ColorFに変換してブラシを作成
	D2D1_COLOR_F brushColor = D2D1::ColorF(color.x, color.y, color.z, color.w);
	ID2D1SolidColorBrush* pBrush = nullptr;
	hr = m_D2DRenderTarget->CreateSolidColorBrush(brushColor, &pBrush);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create solid color brush: " << hr << std::endl;
		return;
	}

	// 描画の開始
	m_D2DRenderTarget->BeginDraw();

	// テキストを描画
	m_D2DRenderTarget->DrawText(
		text.c_str(),
		static_cast<UINT32>(text.length()),
		textFormat,
		layoutRect,
		pBrush
	);

	// 描画の終了
	hr = m_D2DRenderTarget->EndDraw();
	if (FAILED(hr)) {
		std::cerr << "Failed to draw text: " << hr << std::endl;
	}

	// リソースを解放
	textFormat->Release();
	pBrush->Release();
}