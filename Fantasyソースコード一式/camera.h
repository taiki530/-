#pragma once
#include "gameobject.h"

class Camera : public GameObject
{
protected:

	Camera* m_Camera;

private:

	XMFLOAT3	m_At{};
	float	 	m_Len{}; 
	XMFLOAT3	m_Target{};
	XMFLOAT4X4	m_ViewMatrix{};
	XMFLOAT3    up{ 0.0f, 1.0f, 0.0f };
	bool		isEnemyVisible;
	bool		isEnemyHpDisplayed;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	
	
	XMMATRIX GetViewMatrix()
	{
		return XMLoadFloat4x4(&m_ViewMatrix);
	}
	// プロジェクション行列を取得する関数
	static XMMATRIX GetProjectionMatrix();
	// カメラの位置を取得する関数
	static XMVECTOR GetPosition();

};