#include "main.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "manager.h"
#include "bullet.h"
#include "scene.h"
#include "explosion.h"
#include "enemy.h"
#include "enemyHp.h"
#include "box.h"


void Bullet::Init()
{
    m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f); 
    m_Scale = XMFLOAT3(2.0f, 2.0f, 2.0f); 
    m_Speed = 0.3f; // 弾の移動速度を少し速めに調整
    m_Component = new ModelRenderer(this);
    ((ModelRenderer*)m_Component)->Load("asse\\model\\bullet.obj");

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\unlitTextureVS.cso");
    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\unlitTexturePS.cso");
}

void Bullet::Uninit()
{
    if (m_Component) { // nullチェックを追加
        m_Component->Uninit();
        delete m_Component;
        m_Component = nullptr;
    }
    if (m_VertexLayout) {
        m_VertexLayout->Release();
        m_VertexLayout = nullptr;
    }
    if (m_VertexShader) {
        m_VertexShader->Release();
        m_VertexShader = nullptr;
    }
    if (m_PixelShader) {
        m_PixelShader->Release();
        m_PixelShader = nullptr;
    }
}
void Bullet::Update()
{
    Scene* scene = Manager::GetScene();

    // 方向ベクトルを用いて位置を更新
    m_Position.x += m_Direction.x * m_Speed;
    m_Position.y += m_Direction.y * m_Speed;
    m_Position.z += m_Direction.z * m_Speed;

    //弾丸がスポーン地点から一定距離離れたら消滅
    float distanceToSpawnSquared =
        (m_Position.x - m_SpawnPosition.x) * (m_Position.x - m_SpawnPosition.x) +
        (m_Position.y - m_SpawnPosition.y) * (m_Position.y - m_SpawnPosition.y) +
        (m_Position.z - m_SpawnPosition.z) * (m_Position.z - m_SpawnPosition.z);

    if (distanceToSpawnSquared > 50.0f * 50.0f) // 距離の二乗で比較。例えば50単位離れたら
    {
        // 弾が消えるときに爆発エフェクトを出す場合
        Explosion* explosion = scene->AddGameObject<Explosion>(1);
        if (explosion) { // Nullチェック
            explosion->SetPostion(m_Position);
        }
        SetDestroy();
        return; // 破壊されたら以降の処理は不要
    }

    auto boxList = scene->GetGameObjects<Box>();

    for (Box* box : boxList)
    {
        XMFLOAT3 boxPosition = box->GetPostion();
        XMFLOAT3 boxObjectScale = box->GetScale();

        float boxHalfWidthX = boxObjectScale.x;
        float boxHalfWidthZ = boxObjectScale.z;


        float bulletRadius = 0.5f; // 弾の当たり判定半径

        // OBB (Oriented Bounding Box) 衝突判定
        // Boxの中心から弾の中心へのベクトル
        XMFLOAT3 dirFromBoxToBullet;
        dirFromBoxToBullet.x = m_Position.x - boxPosition.x;
        dirFromBoxToBullet.y = m_Position.y - boxPosition.y;
        dirFromBoxToBullet.z = m_Position.z - boxPosition.z;

        // ボックスのローカル座標系への変換
        // Boxの回転を考慮
        XMMATRIX boxRotationMatrix = XMMatrixRotationRollPitchYaw(box->GetRotation().x, box->GetRotation().y, box->GetRotation().z);
        XMMATRIX boxInverseRotationMatrix = XMMatrixInverse(nullptr, boxRotationMatrix);

        XMVECTOR localDirVec = XMVector3TransformNormal(XMLoadFloat3(&dirFromBoxToBullet), boxInverseRotationMatrix);
        XMFLOAT3 localDir;
        XMStoreFloat3(&localDir, localDirVec);

        // 各軸に沿った距離
        float absLocalDirX = fabsf(localDir.x);
        float absLocalDirY = fabsf(localDir.y);
        float absLocalDirZ = fabsf(localDir.z);

        // OBBと球の判定
        // 球の中心がOBBの各軸にどれだけ近づいているか
        // 軸ごとの衝突判定
        if (absLocalDirX < (boxHalfWidthX + bulletRadius) &&
            absLocalDirY < (boxObjectScale.y + bulletRadius) && // BoxのYスケールと弾の半径
            absLocalDirZ < (boxHalfWidthZ + bulletRadius))
        {
            // 衝突時の処理
            Explosion* explosion = scene->AddGameObject<Explosion>(1);
            if (explosion) { // Nullチェック
                explosion->SetPostion(m_Position);
            }
            SetDestroy();
            return; // 破壊されたら以降の処理は不要
        }
    }

    auto enemyList = scene->GetGameObjects<Enemy>();
    for (Enemy* enemy : enemyList)
    {
        XMFLOAT3 enemyPosition = enemy->GetPostion();
        // エネミーのスケールをGameObject::m_Scaleから取得
        XMFLOAT3 enemyObjectScale = enemy->GetScale();

        
        float collisionScaleFactor = 3.0f; // この値を調整して当たり判定の大きさを変更

        // OBBの半幅/半奥行きにスケールファクターを適用
        float enemyHalfWidthX = enemyObjectScale.x * collisionScaleFactor;
        float enemyHalfWidthZ = enemyObjectScale.z * collisionScaleFactor;
        float enemyHalfHeightY = enemyObjectScale.y * collisionScaleFactor; // Y軸方向も考慮

        // 弾の半径
        float bulletRadius = 0.5f; 

        // エネミーの中心から弾の中心へのベクトル
        XMFLOAT3 dirFromEnemyToBullet;
        dirFromEnemyToBullet.x = m_Position.x - enemyPosition.x;
        dirFromEnemyToBullet.y = m_Position.y - enemyPosition.y;
        dirFromEnemyToBullet.z = m_Position.z - enemyPosition.z;

        // エネミーのローカル座標系への変換
        // エネミーの回転を考慮
        XMMATRIX enemyRotationMatrix = XMMatrixRotationRollPitchYaw(enemy->GetRotation().x, enemy->GetRotation().y, enemy->GetRotation().z);
        XMMATRIX enemyInverseRotationMatrix = XMMatrixInverse(nullptr, enemyRotationMatrix);

        XMVECTOR localDirVec = XMVector3TransformNormal(XMLoadFloat3(&dirFromEnemyToBullet), enemyInverseRotationMatrix);
        XMFLOAT3 localDir;
        XMStoreFloat3(&localDir, localDirVec);

        // 各軸に沿った距離
        float absLocalDirX = fabsf(localDir.x);
        float absLocalDirY = fabsf(localDir.y);
        float absLocalDirZ = fabsf(localDir.z);

        // OBBと球の判定
        if (absLocalDirX < (enemyHalfWidthX + bulletRadius) &&
            absLocalDirY < (enemyHalfHeightY + bulletRadius) &&
            absLocalDirZ < (enemyHalfWidthZ + bulletRadius))
        {
            // 衝突時の処理
            Explosion* explosion = scene->AddGameObject<Explosion>(1);
            if (explosion) { // Nullチェック
                explosion->SetPostion(m_Position);
            }

            // エネミーにダメージを与える
            enemy->TakeDamage(20.0f); // ダメージ量

            SetDestroy();
            return;
        }
    }
}

void Bullet::Draw()
{
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    XMMATRIX world, scale, rot, trans;
    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
    world = scale * rot * trans;

    Renderer::SetWorldMatrix(world);
    m_Component->Draw();
}

void Bullet::SetDirection(XMFLOAT3 direction)
{
    // 正規化して弾の速度が一定になるようにする
    float length = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (length > 0.0f)
    {
        m_Direction.x = direction.x / length;
        m_Direction.y = direction.y / length;
        m_Direction.z = direction.z / length;
    }
    else 
    {
        m_Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
    }
}
