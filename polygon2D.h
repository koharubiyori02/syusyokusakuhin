//===========================================================
//
//　2Dオブジェクト
// 
//===========================================================

#pragma once
#include "gameObject.h"
#include "main.h"
#include "renderer.h"

class Polygon2D : public GameObject
{
private:
	

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	VERTEX_3D m_Vertex[4]{};

	XMFLOAT3 m_StartPos;
	XMFLOAT3 m_EndPos;

	bool m_SetPos = false;		//start,endのPosがセットされているかどうか
	bool m_IsMove;				//動くオブジェクトかどうか
	bool m_Moving = false;		//今変化しているかどうか
	float m_OffsetX = 0.0f;		//頂点のXオフセット
	float m_OffsetY = 0.0f;		//頂点のYオフセット

	std::wstring m_TexturePath{};	//テクスチャパス

	float m_Alpha = 1.0f;		//アルファ値
	bool m_AlphaBool = false;	//アルファ値を足すか減らすかどうか

	std::string m_Tag;			//タグ


public:

	Polygon2D(std::wstring path, XMFLOAT3 pos, float setX, float setY, bool is, std::string name)
		:m_TexturePath(path), m_OffsetX(setX), m_OffsetY(setY), m_IsMove(is), m_Tag(name)
	{
		m_Position = pos;

		m_Vertex[0].Position = XMFLOAT3(m_Position.x - m_OffsetX / 2, m_Position.y - m_OffsetY / 2, 0.0f);
		m_Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		m_Vertex[1].Position = XMFLOAT3(m_Position.x + m_OffsetX / 2, m_Position.y - m_OffsetY / 2, 0.0f);
		m_Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

		m_Vertex[2].Position = XMFLOAT3(m_Position.x - m_OffsetX / 2, m_Position.y + m_OffsetY / 2, 0.0f);
		m_Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

		m_Vertex[3].Position = XMFLOAT3(m_Position.x + m_OffsetX / 2, m_Position.y + m_OffsetY / 2, 0.0f);
		m_Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);
	}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	bool GetMoving() const { return m_Moving; }
	void SetMoving(const bool is) { m_Moving = is; }

	bool GetTagName(const std::string name) 
	{ 
		if (m_Tag == name)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetVertexPos(const XMFLOAT3 start, const XMFLOAT3 end, const float t);

	void SetSubject();//チュートリアルとかの文字出す用
};