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
	float m_MaxWidth; // HP�o�[�̍ő啝

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// �O������HP��ݒ肷�邽�߂̊֐�
	void SetCurrentHp(float hp);
};

