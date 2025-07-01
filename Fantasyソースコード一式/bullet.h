#pragma once
#include "gameobject.h"
#include "component.h"

class Bullet : public GameObject
{
private:
    Component* m_Component{};
    ID3D11VertexShader* m_VertexShader;
    ID3D11PixelShader* m_PixelShader;
    ID3D11InputLayout* m_VertexLayout;
    XMFLOAT3 m_Direction; // 移動方向
    XMFLOAT3 m_SpawnPosition; // スポーン位置
    float m_Speed; // 移動速度

public:
    void Init();
    void Uninit();
    void Update();
    void Draw();
    void SetDirection(XMFLOAT3 direction);
    void SetSpawnPosition(XMFLOAT3 position) { m_SpawnPosition = position; } // <= 追加
};
