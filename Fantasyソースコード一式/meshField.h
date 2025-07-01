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

    // �t�@�C���p�X�̒萔
    const char* FIELD_HEIGHT_FILE = "field_height.bin";

    // �n�`�ҏW���[�h�̃t���O
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

    // �n�`�f�[�^���t�@�C���ɕۑ�����֐�
    void SaveFieldHeight();
    // �t�@�C������n�`�f�[�^��ǂݍ��ފ֐�
    bool LoadFieldHeight();
};