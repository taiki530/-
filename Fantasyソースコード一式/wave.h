#pragma once
#include "gameobject.h"
#include "component.h"

class Wave : public GameObject
{
private:

	Component* m_Component{};

	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;

	ID3D11ShaderResourceView* m_Texture;


	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	VERTEX_3D m_Vertex[21][21]{};

	float m_Time = 0.0f;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	float GetHeight(XMFLOAT3 Position);
};