#pragma once

#include "gameobject.h"
#include "component.h"
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;

class MeshField : public GameObject
{
private:
    Component* m_Component{};

    ID3D11Buffer* m_VertexBuffer = nullptr;
    ID3D11Buffer* m_IndexBuffer = nullptr;

    ID3D11ShaderResourceView* m_Texture = nullptr;

    ID3D11VertexShader* m_VertexShader = nullptr;
    ID3D11PixelShader* m_PixelShader = nullptr;
    ID3D11InputLayout* m_VertexLayout = nullptr;

    VERTEX_3D m_Vertex[21][21]{};

    // ファイルパスの定数
    const char* FIELD_HEIGHT_FILE = "field_height.bin";

    // 地形編集モードのフラグ
    bool m_EditModeEnabled = false;

public:
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw() override;

    float GetHeight(XMFLOAT3 Position);

    void UpdateVertexBuffer();

private:
    bool RayPick(XMFLOAT3& outHitPos, XMFLOAT3& outHitNormal);

    // 地形データをファイルに保存する関数
    void SaveFieldHeight();
    // ファイルから地形データを読み込む関数
    bool LoadFieldHeight();
};