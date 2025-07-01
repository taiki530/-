#pragma once
#include "scene.h"

class FadeOut2 : public Scene
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	float m_Alpha = 1.0f;
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};

