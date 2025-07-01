#pragma once
#include "gameobject.h"

class EnemyCamera : public GameObject
{
protected:

	EnemyCamera* m_EnemyCamera;

private:
	
	XMFLOAT3	m_At{};
	float	 	m_Len{};
	XMFLOAT3	m_Target{};
	XMFLOAT4X4	m_ViewMatrix {};
	XMFLOAT3    up{ 0.0f, 1.0f, 0.0f };
	bool		isEnemyVisible;
	bool		isEnemyHpDisplayed;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMMATRIX GetViewMatrix()
	{ return XMLoadFloat4x4(&m_ViewMatrix);}

};

