#pragma once
#include "gameobject.h"
#include "component.h"


class Box : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	Component*			m_Component{};

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;


public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};

