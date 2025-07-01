#pragma once
#include "gameobject.h"

class SsCamera : public GameObject
{
protected:

	SsCamera* m_SsCamera;

private:

	XMFLOAT3	m_At{};
	float	 	m_Len{};
	XMFLOAT3	m_Target{};
	XMFLOAT4X4	m_ViewMatrix{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMMATRIX GetViewMatrix()
	{
		return XMLoadFloat4x4(&m_ViewMatrix);
	}

};
