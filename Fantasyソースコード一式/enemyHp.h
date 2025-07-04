#pragma once
#include "gameobject.h"

class EnemyHpBar : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	
	float m_CurrentHp;
	float m_MaxHp;
	float m_MaxWidth; // HPバーの最大幅

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// 外部からHPを設定するための関数
	void SetCurrentHp(float hp);
};

