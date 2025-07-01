#pragma once
#include "gameobject.h"
#include "component.h"


class ShadowVolum : public GameObject
{
private:

	Component*			m_Component{};

	ID3D11ShaderResourceView* m_EnvTexture{};

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};

