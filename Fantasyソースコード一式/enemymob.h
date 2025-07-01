#pragma once
#include "gameobject.h"
#include "component.h"
#include <string>

class EnemyMob : public GameObject
{
private:

    Component* m_Component{};

    ID3D11VertexShader* m_VertexShader;
    ID3D11PixelShader* m_PixelShader;
    ID3D11InputLayout* m_VertexLayout;

    std::string m_AnimationName1{};
    std::string m_AnimationName2{};
    float m_AnimationBlendRatio{};
    int m_AnimationFrame{};

    float m_CollisionRadius = 1.0f; // 当たり判定の半径
    bool m_IsMovable = true;         // プレイヤーが動けるかどうかを管理するフラグ

public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

    void SetMovable(bool movable) { m_IsMovable = movable; }
    void PlayerChase();
};
