#pragma once
#include "gameobject.h"
#include "component.h"

class FireEffect : public GameObject
{
private:

	Component* m_Component{};

	ID3D11Buffer* m_VertexBuffer;
	ID3D11ShaderResourceView* m_Texture;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	int m_Count{};

	struct FIREEFFECT
	{
		bool		Enable;
		int			Life;
		XMFLOAT3	Position;
		XMFLOAT3	Velocity;
	};
	static const int FIREEFFECT_MAX = 1000;
	FIREEFFECT g_FireEffect[FIREEFFECT_MAX]{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};

