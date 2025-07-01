#pragma once
#include "gameobject.h"

class HpBar : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	float m_CurrentHp;
	float m_MaxHp;
	float m_MaxWidth; // HPƒo[‚ÌÅ‘å•

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// ŠO•”‚©‚çHP‚ğİ’è‚·‚é‚½‚ß‚ÌŠÖ”
	void SetCurrentHp(float hp);
};

