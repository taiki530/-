#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Frustum
{
public:
    XMFLOAT4 planes[6]; // 6つの平面（左, 右, 上, 下, 近距離, 遠距離）

    // 視錐台の平面を計算する
    void CreateFrustum(XMMATRIX viewProjMatrix)
    {
        // 行列の逆転を使って視錐台の平面を計算
        XMMATRIX mat = XMMatrixTranspose(viewProjMatrix);
        // 左
        planes[0] = XMFLOAT4(mat.r[3].m128_f32[0] + mat.r[0].m128_f32[0], mat.r[3].m128_f32[1] + mat.r[0].m128_f32[1], mat.r[3].m128_f32[2] + mat.r[0].m128_f32[2], mat.r[3].m128_f32[3] + mat.r[0].m128_f32[3]);
        // 右
        planes[1] = XMFLOAT4(mat.r[3].m128_f32[0] - mat.r[0].m128_f32[0], mat.r[3].m128_f32[1] - mat.r[0].m128_f32[1], mat.r[3].m128_f32[2] - mat.r[0].m128_f32[2], mat.r[3].m128_f32[3] - mat.r[0].m128_f32[3]);
        // 上
        planes[2] = XMFLOAT4(mat.r[3].m128_f32[0] - mat.r[1].m128_f32[0], mat.r[3].m128_f32[1] - mat.r[1].m128_f32[1], mat.r[3].m128_f32[2] - mat.r[1].m128_f32[2], mat.r[3].m128_f32[3] - mat.r[1].m128_f32[3]);
        // 下
        planes[3] = XMFLOAT4(mat.r[3].m128_f32[0] + mat.r[1].m128_f32[0], mat.r[3].m128_f32[1] + mat.r[1].m128_f32[1], mat.r[3].m128_f32[2] + mat.r[1].m128_f32[2], mat.r[3].m128_f32[3] + mat.r[1].m128_f32[3]);
        // 近距離
        planes[4] = XMFLOAT4(mat.r[3].m128_f32[0] + mat.r[2].m128_f32[0], mat.r[3].m128_f32[1] + mat.r[2].m128_f32[1], mat.r[3].m128_f32[2] + mat.r[2].m128_f32[2], mat.r[3].m128_f32[3] + mat.r[2].m128_f32[3]);
        // 遠距離
        planes[5] = XMFLOAT4(mat.r[3].m128_f32[0] - mat.r[2].m128_f32[0], mat.r[3].m128_f32[1] - mat.r[2].m128_f32[1], mat.r[3].m128_f32[2] - mat.r[2].m128_f32[2], mat.r[3].m128_f32[3] - mat.r[2].m128_f32[3]);
    }

    // AABBとの交差判定
    bool IsIntersecting(const XMFLOAT3& min, const XMFLOAT3& max)
    {
        for (int i = 0; i < 6; ++i)
        {
            XMFLOAT4 plane = planes[i];
            // AABBの8つの頂点をチェック
            XMFLOAT3 vertices[8] = {
                XMFLOAT3(min.x, min.y, min.z),
                XMFLOAT3(max.x, min.y, min.z),
                XMFLOAT3(min.x, max.y, min.z),
                XMFLOAT3(max.x, max.y, min.z),
                XMFLOAT3(min.x, min.y, max.z),
                XMFLOAT3(max.x, min.y, max.z),
                XMFLOAT3(min.x, max.y, max.z),
                XMFLOAT3(max.x, max.y, max.z)
            };
            bool inside = false;
            for (int j = 0; j < 8; ++j)
            {
                XMFLOAT3 vertex = vertices[j];
                float result = vertex.x * plane.x + vertex.y * plane.y + vertex.z * plane.z + plane.w;
                if (result >= 0)
                {
                    inside = true;
                    break;
                }
            }
            if (!inside)
            {
                return false;
            }
        }
        return true;
    }
};