#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "meshField.h"
#include "manager.h"
#include <float.h>


// 地形高さデータ
float g_FieldHeight[21][21] = //
{
    { 0.0f, 1.5f, 2.0f, 3.2f, 1.8f, 2.4f, 0.5f, 1.1f, 2.3f, 3.5f, 3.8f, 4.0f, 2.2f, 1.9f, 3.4f, 2.8f, 1.7f, 2.6f, 1.3f, 2.5f, 2.9f },
    { 0.0f, 0.8f, 1.2f, 2.7f, 3.6f, 1.1f, 4.0f, 3.3f, 2.1f, 0.9f, 1.5f, 3.5f, 4.1f, 3.9f, 1.4f, 0.7f, 3.5f, 4.0f, 2.6f, 2.7f, 3.9f },
    { 0.0f, 3.4f, 1.0f, 4.5f, 4.2f, 3.9f, 4.8f, 0.9f, 3.9f, 4.5f, 4.2f, 2.7f, 2.4f, 3.9f, 4.0f, 4.3f, 4.6f, 3.8f, 3.5f, 3.4f, 3.1f },
    { 0.0f, 1.3f, 0.5f, 3.7f, 2.9f, 0.6f, 3.5f, 4.1f, 4.4f, 3.8f, 4.8f, 2.9f, 4.1f, 4.0f, 4.3f, 0.8f, 2.6f, 3.8f, 4.2f, 3.9f, 4.0f },
    { 0.0f, 2.8f, 3.6f, 2.7f, 4.2f, 3.5f, 4.2f, 4.3f, 3.1f, 4.4f, 3.7f, 2.3f, 3.9f, 4.5f, 3.6f, 3.8f, 4.4f, 2.9f, 4.3f, 2.5f, 4.2f },
    { 0.0f, 4.1f, 0.7f, 3.8f, 4.3f, 4.6f, 4.8f, 3.7f, 4.4f, 3.9f, 4.5f, 4.3f, 3.9f, 3.8f, 4.1f, 3.5f, 3.2f, 4.0f, 3.8f, 4.6f, 3.8f },
    { 0.0f, 2.9f, 3.1f, 4.5f, 3.9f, 3.0f, 3.8f, 2.7f, 4.4f, 3.1f, 4.6f, 4.2f, 4.5f, 4.3f, 3.9f, 2.9f, 4.0f, 3.7f, 4.8f, 3.5f, 4.2f },
    { 0.0f, 3.3f, 4.0f, 4.1f, 3.9f, 4.5f, 4.7f, 3.6f, 4.3f, 4.1f, 3.8f, 4.9f, 4.0f, 4.4f, 4.9f, 3.7f, 2.6f, 4.2f, 4.5f, 2.4f, 4.7f },
    { 0.0f, 4.5f, 4.8f, 4.1f, 3.0f, 4.3f, 3.9f, 3.7f, 4.0f, 4.4f, 4.2f, 4.5f, 3.8f, 3.7f, 4.2f, 4.1f, 4.6f, 4.3f, 4.3f, 4.5f, 4.9f },
    { 0.0f, 4.9f, 4.0f, 4.3f, 4.6f, 4.9f, 4.8f, 4.5f, 4.3f, 4.6f, 4.5f, 4.8f, 4.7f, 4.9f, 4.5f, 4.0f, 4.1f, 4.3f, 4.7f, 4.4f, 4.8f },
    { 0.0f, 0.5f, 1.4f, 4.3f, 2.7f, 4.3f, 4.1f, 4.0f, 4.5f, 4.9f, 4.7f, 4.8f, 4.6f, 4.9f, 4.2f, 4.5f, 4.2f, 4.9f, 4.9f, 4.7f, 4.8f },
    { 0.0f, 3.1f, 2.6f, 4.7f, 4.9f, 4.8f, 4.9f, 4.1f, 4.8f, 4.3f, 4.8f, 4.8f, 4.9f, 4.9f, 4.8f, 4.6f, 4.9f, 4.7f, 4.8f, 4.9f, 4.8f },
    { 0.0f, 4.3f, 4.1f, 4.9f, 4.8f, 4.9f, 4.7f, 4.4f, 4.8f, 4.6f, 4.9f, 4.7f, 4.2f, 4.9f, 4.8f, 4.9f, 4.5f, 4.9f, 4.9f, 4.7f, 4.8f },
    { 0.0f, 4.9f, 4.7f, 4.9f, 4.9f, 4.9f, 4.8f, 4.2f, 4.7f, 4.3f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.8f, 4.9f, 4.8f, 4.9f, 4.9f, 4.9f },
    { 0.0f, 4.5f, 4.6f, 4.8f, 4.7f, 4.9f, 4.5f, 4.4f, 4.9f, 4.3f, 4.9f, 4.9f, 4.9f, 4.9f, 4.8f, 4.9f, 4.5f, 4.8f, 4.8f, 4.9f, 4.9f },
    { 0.0f, 4.8f, 4.9f, 4.5f, 4.9f, 4.9f, 4.8f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.8f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f },
    { 0.0f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f, 4.9f }
};

float MeshField::GetHeight(XMFLOAT3 Position)
{
    int x = Position.x / 5.0f + 10.0f;
    int z = Position.z / -5.0f + 10.0f;

    if (x < 0 || x >= 20 || z < 0 || z >= 20)
        return 0.0f;

    XMFLOAT3 pos0 = m_Vertex[x][z].Position;
    XMFLOAT3 pos1 = m_Vertex[x + 1][z].Position;
    XMFLOAT3 pos2 = m_Vertex[x][z + 1].Position;
    XMFLOAT3 pos3 = m_Vertex[x + 1][z + 1].Position;

    XMFLOAT3 v12 = { pos2.x - pos1.x, pos2.y - pos1.y, pos2.z - pos1.z };
    XMFLOAT3 v1p = { Position.x - pos1.x, Position.y - pos1.y, Position.z - pos1.z };

    float cy = v12.z * v1p.x - v12.x * v1p.z;

    XMFLOAT3 n;
    if (cy > 0.0f)
    {
        XMFLOAT3 v10 = { pos0.x - pos1.x, pos0.y - pos1.y, pos0.z - pos1.z };
        n.x = v10.y * v12.z - v10.z * v12.y;
        n.y = v10.z * v12.x - v10.x * v12.z;
        n.z = v10.x * v12.y - v10.y * v12.x;
    }
    else
    {
        XMFLOAT3 v13 = { pos3.x - pos1.x, pos3.y - pos1.y, pos3.z - pos1.z };
        n.x = v12.y * v13.z - v12.z * v13.y;
        n.y = v12.z * v13.x - v12.x * v13.z;
        n.z = v12.x * v13.y - v12.y * v13.x;
    }

    return -((Position.x - pos1.x) * n.x + (Position.z - pos1.z) * n.z) / n.y + pos1.y;
}

void MeshField::Init()
{
    // 編集モードの初期化
    m_EditModeEnabled = false; // 最初は無効にしておく

    // 地形データを読み込む
    bool loadedFromFile = LoadFieldHeight();

    if (!loadedFromFile) // ファイルからの読み込みに失敗した場合
    {
        // 既存の初期化処理を実行
        for (int x = 0; x < 21; x++)
        {
            for (int z = 0; z < 21; z++)
            {
                m_Vertex[x][z].Position = XMFLOAT3((x - 10.0f) * 5.0f, g_FieldHeight[x][z], (z - 10.0f) * -5.0f);
                m_Vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
                m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                m_Vertex[x][z].TexCoord = XMFLOAT2(x, z);
            }
        }
    }
    else {
        // ファイルから読み込みが成功した場合は、m_Vertex の Position.y が既に更新されているので、
        // Position.x と Position.z、その他の要素をここで設定する必要がある。
        for (int x = 0; x < 21; x++)
        {
            for (int z = 0; z < 21; z++)
            {
                // Position.yはLoadFieldHeightで設定済み
                m_Vertex[x][z].Position.x = (x - 10.0f) * 5.0f;
                m_Vertex[x][z].Position.z = (z - 10.0f) * -5.0f;
                m_Vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); // 後で法線は再計算される
                m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                m_Vertex[x][z].TexCoord = XMFLOAT2(x, z);
            }
        }
    }

    // 法線ベクトルの算出 (読み込み成功/失敗どちらでも必要)
    for (int x = 1; x < 20; x++)
    {
        for (int z = 1; z < 20; z++)
        {
            XMFLOAT3 vx, vz, vn;
            vx.x = m_Vertex[x + 1][z].Position.x - m_Vertex[x - 1][z].Position.x;
            vx.y = m_Vertex[x + 1][z].Position.y - m_Vertex[x - 1][z].Position.y;
            vx.z = m_Vertex[x + 1][z].Position.z - m_Vertex[x - 1][z].Position.z;

            vz.x = m_Vertex[x][z - 1].Position.x - m_Vertex[x][z + 1].Position.x;
            vz.y = m_Vertex[x][z - 1].Position.y - m_Vertex[x][z + 1].Position.y;
            vz.z = m_Vertex[x][z - 1].Position.z - m_Vertex[x][z + 1].Position.z;

            // 外積
            vn.x = vz.y * vx.z - vz.z * vx.y;
            vn.y = vz.z * vx.x - vz.x * vx.z;
            vn.z = vz.x * vx.y - vz.y * vx.x;

            // 正規化（長さ1にする）
            float len = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
            if (len != 0) { // lenが0でないことを確認してから割る
                vn.x /= len;
                vn.y /= len;
                vn.z /= len;
            }
            else {
                vn = XMFLOAT3(0.0f, 1.0f, 0.0f); // ゼロ除算を避けるためデフォルト値を設定
            }

            m_Vertex[x][z].Normal = vn;
        }
    }
    //頂点バッファ生成
    D3D11_BUFFER_DESC bd{};
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * 21 * 21;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA sd{};
    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = m_Vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


    // インデックスバッファ生成
    {
        unsigned int index[((21 + 1) * 2) * 20 - 2];

        int i = 0;
        for (int x = 0; x < 20; x++)
        {
            for (int z = 0; z < 21; z++)
            {
                index[i] = x * 21 + z;
                i++;

                index[i] = (x + 1) * 21 + z;
                i++;
            }

            if (x == 19)
                break;

            index[i] = (x + 1) * 21 + 20;
            i++;

            index[i] = (x + 1) * 21;
            i++;
        }

        //頂点バッファ生成
        D3D11_BUFFER_DESC bd{};
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(unsigned int) * ((22 * 2) * 20 - 2);
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA sd{};
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = index;

        Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
    }
    // テクスチャ読み込み
    TexMetadata metadata;
    ScratchImage image;
    LoadFromWICFile(L"asse\\texture\\maguma.jpg", WIC_FLAGS_NONE, &metadata, image);
    CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
    assert(m_Texture);

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\vertexLightingVS.cso");
    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\vertexLightingPS.cso");
}

void MeshField::Uninit()
{
    if (m_IndexBuffer) m_IndexBuffer->Release();
    if (m_Texture) m_Texture->Release();
    if (m_VertexLayout) m_VertexLayout->Release();
    if (m_VertexShader) m_VertexShader->Release();
    if (m_PixelShader) m_PixelShader->Release();
    if (m_VertexBuffer) m_VertexBuffer->Release();
}

void MeshField::Update()
{
    //  'T' キーで編集モードのオン/オフを切り替える
    if (Input::GetKeyTrigger('T')) {
        m_EditModeEnabled = !m_EditModeEnabled;
        // モード変更のフィードバック
        if (m_EditModeEnabled) {
            MessageBox(NULL, "MeshField Edit Mode: ON", "Info", MB_OK | MB_ICONINFORMATION);
        }
        else {
            MessageBox(NULL, "MeshField Edit Mode: OFF", "Info", MB_OK | MB_ICONINFORMATION);
        }
    }

    // 地形編集モードが有効な場合のみ、以下の編集処理を実行
    if (m_EditModeEnabled)
    {
        // 左クリックで高さを下げる
        if (Input::IsMousePressed(MOUSE_LEFT))
        {
            XMFLOAT3 hitPos;
            XMFLOAT3 hitNormal;
            if (RayPick(hitPos, hitNormal))
            {
                float affectRadius = 5.0f;
                float changeAmount = -0.1f;

                for (int x = 0; x < 21; ++x)
                {
                    for (int z = 0; z < 21; ++z)
                    {
                        XMVECTOR vertexPos = XMLoadFloat3(&m_Vertex[x][z].Position);
                        XMVECTOR centerPos = XMLoadFloat3(&hitPos);

                        XMFLOAT2 vxz = { XMVectorGetX(vertexPos), XMVectorGetZ(vertexPos) };
                        XMFLOAT2 cxz = { XMVectorGetX(centerPos), XMVectorGetZ(centerPos) };

                        XMVECTOR diff = XMVectorSubtract(XMLoadFloat2(&vxz), XMLoadFloat2(&cxz));
                        float distance = XMVectorGetX(XMVector2Length(diff));

                        if (distance < affectRadius)
                        {
                            float factor = 1.0f - (distance / affectRadius);
                            m_Vertex[x][z].Position.y += changeAmount * factor;
                            g_FieldHeight[x][z] = m_Vertex[x][z].Position.y;
                        }
                    }
                }
                UpdateVertexBuffer();
            }
        }
        // 右クリックで高さを上げる
        else if (Input::IsMousePressed(MOUSE_RIGHT))
        {
            XMFLOAT3 hitPos;
            XMFLOAT3 hitNormal;
            if (RayPick(hitPos, hitNormal))
            {
                float affectRadius = 5.0f;
                float changeAmount = 0.1f;

                for (int x = 0; x < 21; ++x)
                {
                    for (int z = 0; z < 21; ++z)
                    {
                        XMVECTOR vertexPos = XMLoadFloat3(&m_Vertex[x][z].Position);
                        XMVECTOR centerPos = XMLoadFloat3(&hitPos);

                        XMFLOAT2 vxz = { XMVectorGetX(vertexPos), XMVectorGetZ(vertexPos) };
                        XMFLOAT2 cxz = { XMVectorGetX(centerPos), XMVectorGetZ(centerPos) };

                        XMVECTOR diff = XMVectorSubtract(XMLoadFloat2(&vxz), XMLoadFloat2(&cxz));
                        float distance = XMVectorGetX(XMVector2Length(diff));

                        if (distance < affectRadius)
                        {
                            float factor = 1.0f - (distance / affectRadius);
                            m_Vertex[x][z].Position.y += changeAmount * factor;
                            g_FieldHeight[x][z] = m_Vertex[x][z].Position.y;
                        }
                    }
                }
                UpdateVertexBuffer();
            }
        }

        // 'P' キーで地形を保存
        if (Input::GetKeyTrigger('P'))
        {
            SaveFieldHeight();
        }
    }

    
}

void MeshField::Draw()
{
    // 入力レイアウト設定
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    // シェーダ設定
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    // ワールドマトリクス設定
    XMMATRIX world, scale, rot, trans;
    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
    world = scale * rot * trans;
    Renderer::SetWorldMatrix(world);


    // 頂点バッファ設定
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    // インデックスバッファ設定
    Renderer::GetDeviceContext()->IASetIndexBuffer
    (m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    //マテリアル設定
    MATERIAL material;
    ZeroMemory(&material, sizeof(material));
    material.Diffuse = XMFLOAT4(1.0, 1.0, 1.0, 1.0);
    material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); // AmbientをDiffuseと同じに設定
    material.TextureEnable = true;
    Renderer::SetMaterial(material);

    // テクスチャ設定
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

    // プリミティブトボロジ設定
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // ポリゴン描画
    Renderer::GetDeviceContext()->DrawIndexed
    ((22 * 2) * 20 - 2, 0, 0);
}

    // レイと三角形の交差判定関数
bool IntersectRayTriangle(FXMVECTOR RayOrigin, FXMVECTOR RayDirection,
    FXMVECTOR V0, FXMVECTOR V1, FXMVECTOR V2,
    float* pDistance)
{
    XMVECTOR E1 = XMVectorSubtract(V1, V0);
    XMVECTOR E2 = XMVectorSubtract(V2, V0);
    XMVECTOR P = XMVector3Cross(RayDirection, E2);
    float det = XMVectorGetX(XMVector3Dot(E1, P));

    // レイが三角形と平行な場合
    if (det > -FLT_EPSILON && det < FLT_EPSILON)
        return false;

    float invDet = 1.0f / det;

    XMVECTOR T = XMVectorSubtract(RayOrigin, V0);
    float u = XMVectorGetX(XMVector3Dot(T, P)) * invDet;
    if (u < 0.0f || u > 1.0f)
        return false;

    XMVECTOR Q = XMVector3Cross(T, E1);
    float v = XMVectorGetX(XMVector3Dot(RayDirection, Q)) * invDet;
    if (v < 0.0f || u + v > 1.0f)
        return false;

    *pDistance = XMVectorGetX(XMVector3Dot(E2, Q)) * invDet;

    return *pDistance > FLT_EPSILON;
}


bool MeshField::RayPick(XMFLOAT3& outHitPos, XMFLOAT3& outHitNormal)
{
    float mouseX = (float)Input::GetMouseX();
    float mouseY = (float)Input::GetMouseY();
    float screenWidth = (float)Renderer::GetScreenWidth();
    float screenHeight = (float)Renderer::GetScreenHeight();

    // プロジェクション行列とビュー行列を取得
    // ここでCameraクラスのインスタンスを取得して呼び出す
    Camera* camera = Manager::GetScene()->GetGameObject<Camera>();
    if (!camera) // カメラがない場合は処理を中断
    {
        return false;
    }

    XMMATRIX matProj = camera->GetProjectionMatrix();
    XMMATRIX matView = camera->GetViewMatrix();

    // スクリーン座標を-1～1のNDCに変換
    float px = (2.0f * mouseX / screenWidth - 1.0f);
    float py = -(2.0f * mouseY / screenHeight - 1.0f);

    // 逆行列を計算
    XMMATRIX invProj = XMMatrixInverse(nullptr, matProj);
    XMMATRIX invView = XMMatrixInverse(nullptr, matView);

    // スクリーン座標のレイの開始点と終了点を計算
    XMVECTOR rayOriginNDC = XMVectorSet(px, py, 0.0f, 1.0f);
    XMVECTOR rayEndNDC = XMVectorSet(px, py, 1.0f, 1.0f);

    // NDCからビュー空間へ
    XMVECTOR rayOriginView = XMVector3TransformCoord(rayOriginNDC, invProj);
    XMVECTOR rayEndView = XMVector3TransformCoord(rayEndNDC, invProj);

    // ビュー空間からワールド空間へ
    XMVECTOR rayOriginWorld = XMVector3TransformCoord(rayOriginView, invView);
    XMVECTOR rayEndWorld = XMVector3TransformCoord(rayEndView, invView);

    // レイの方向ベクトルを計算
    XMVECTOR rayDirWorld = XMVector3Normalize(XMVectorSubtract(rayEndWorld, rayOriginWorld));

    float minDistance = FLT_MAX;
    bool hit = false;
    XMFLOAT3 currentHitPos;
    XMFLOAT3 currentHitNormal;

    // メッシュの各セルに対してレイと三角形の交差判定を行う
    for (int x = 0; x < 20; ++x)
    {
        for (int z = 0; z < 20; ++z)
        {
            XMVECTOR v0 = XMLoadFloat3(&m_Vertex[x][z].Position);
            XMVECTOR v1 = XMLoadFloat3(&m_Vertex[x + 1][z].Position);
            XMVECTOR v2 = XMLoadFloat3(&m_Vertex[x][z + 1].Position);

            float distance1;
            if (IntersectRayTriangle(rayOriginWorld, rayDirWorld, v0, v1, v2, &distance1))
            {
                if (distance1 < minDistance)
                {
                    minDistance = distance1;
                    XMVECTOR hitPosVec = XMVectorAdd(rayOriginWorld, XMVectorScale(rayDirWorld, distance1));
                    XMStoreFloat3(&currentHitPos, hitPosVec);
                    XMVECTOR normalVec = XMVector3Normalize(XMVector3Cross(XMVectorSubtract(v1, v0), XMVectorSubtract(v2, v0)));
                    XMStoreFloat3(&currentHitNormal, normalVec);
                    hit = true;
                }
            }

            XMVECTOR v3 = XMLoadFloat3(&m_Vertex[x + 1][z].Position);
            XMVECTOR v4 = XMLoadFloat3(&m_Vertex[x + 1][z + 1].Position);
            XMVECTOR v5 = XMLoadFloat3(&m_Vertex[x][z + 1].Position);

            float distance2;
            if (IntersectRayTriangle(rayOriginWorld, rayDirWorld, v3, v4, v5, &distance2))
            {
                if (distance2 < minDistance)
                {
                    minDistance = distance2;
                    XMVECTOR hitPosVec = XMVectorAdd(rayOriginWorld, XMVectorScale(rayDirWorld, distance2));
                    XMStoreFloat3(&currentHitPos, hitPosVec);
                    XMVECTOR normalVec = XMVector3Normalize(XMVector3Cross(XMVectorSubtract(v4, v3), XMVectorSubtract(v5, v3)));
                    XMStoreFloat3(&currentHitNormal, normalVec);
                    hit = true;
                }
            }
        }
    }

    if (hit)
    {
        outHitPos = currentHitPos;
        outHitNormal = currentHitNormal;
        return true;
    }

    return false;
}

void MeshField::UpdateVertexBuffer()
{
    // 頂点バッファをCPUメモリから更新
    Renderer::GetDeviceContext()->UpdateSubresource(
        m_VertexBuffer, 0, nullptr, m_Vertex, 0, 0);

    // 法線ベクトルの再計算
    for (int x = 1; x < 20; x++)
    {
        for (int z = 1; z < 20; z++)
        {
            XMFLOAT3 vx, vz, vn;
            vx.x = m_Vertex[x + 1][z].Position.x - m_Vertex[x - 1][z].Position.x;
            vx.y = m_Vertex[x + 1][z].Position.y - m_Vertex[x - 1][z].Position.y;
            vx.z = m_Vertex[x + 1][z].Position.z - m_Vertex[x - 1][z].Position.z;

            vz.x = m_Vertex[x][z - 1].Position.x - m_Vertex[x][z + 1].Position.x;
            vz.y = m_Vertex[x][z - 1].Position.y - m_Vertex[x][z + 1].Position.y;
            vz.z = m_Vertex[x][z - 1].Position.z - m_Vertex[x][z + 1].Position.z;

            // 外積
            vn.x = vz.y * vx.z - vz.z * vx.y;
            vn.y = vz.z * vx.x - vz.x * vx.z;
            vn.z = vz.x * vx.y - vz.y * vx.x;

            // 正規化（長さ1にする）
            float len = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
            if (len != 0) {
                vn.x /= len;
                vn.y /= len;
                vn.z /= len;
            }
            else {
                vn = XMFLOAT3(0.0f, 1.0f, 0.0f);
            }

            m_Vertex[x][z].Normal = vn;
        }
    }
}

// 地形データをファイルに保存する関数
void MeshField::SaveFieldHeight()
{
    // バイナリ書き込みモードでファイルを開く
    std::ofstream ofs(FIELD_HEIGHT_FILE, std::ios::binary);
    if (!ofs.is_open()) 
    {
        // エラー処理
        MessageBox(NULL, "Failed to open file for writing field height!", "Error", MB_OK | MB_ICONERROR); //
        return;
    }

    // g_FieldHeight 配列の内容をそのまま書き込む
    ofs.write(reinterpret_cast<const char*>(g_FieldHeight), sizeof(g_FieldHeight));
    ofs.close(); 

    MessageBox(NULL, "Field height saved successfully!", "Success", MB_OK | MB_ICONINFORMATION); //
}

// ファイルから地形データを読み込む関数
bool MeshField::LoadFieldHeight()
{
    // バイナリ読み込みモードでファイルを開く
    std::ifstream ifs(FIELD_HEIGHT_FILE, std::ios::binary);
    if (!ifs.is_open()) //
    {
        // ファイルが存在しない場合は読み込み失敗として false を返す
        return false;
    }

    // ファイルから g_FieldHeight 配列に読み込む
    ifs.read(reinterpret_cast<char*>(g_FieldHeight), sizeof(g_FieldHeight));
    ifs.close();

    // 読み込んだデータでm_VertexのPosition.yを更新
    for (int x = 0; x < 21; x++)
    {
        for (int z = 0; z < 21; z++)
        {
            m_Vertex[x][z].Position.y = g_FieldHeight[x][z];
        }
    }
    return true; 
}

