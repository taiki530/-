#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "enemymob.h"
#include "player.h"
#include "scene.h"
#include "meshField.h"
#include "modelRenderer.h"
#include "animationModel.h"

void EnemyMob::Init()
{
    m_Scale = XMFLOAT3(0.01f, 0.01f, 0.01f);

    m_Component = new AnimationModel(this);
    ((AnimationModel*)m_Component)->Load("asse\\model\\Vampire A Lusth.fbx");
    ((AnimationModel*)m_Component)->LoadAnimation("asse\\model\\Vampire Idle.fbx", "Idle");
    ((AnimationModel*)m_Component)->LoadAnimation("asse\\model\\Vampire Run.fbx", "Run");
    ((AnimationModel*)m_Component)->LoadAnimation("asse\\model\\Vampire Attack.fbx","Attack");

    m_AnimationName1 = "Idle";
    m_AnimationName2 = "Idle";

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\unlitTextureVS.cso");
    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\unlitTexturePS.cso");

}

void EnemyMob::Update()
{
    m_AnimationName1 = "Idle";

    Scene* scene = Manager::GetScene();
    Player* player = scene->GetGameObject<Player>();
    XMFLOAT3 playerPosition = player->GetPostion();

    // メッシュフィールドから高さを取得
    MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>();
    float groundHeight = meshField->GetHeight(m_Position);

    XMFLOAT3 rota = m_Rotation;

    // プレイヤー方向のベクトルを計算
    XMFLOAT3 direction = 
    {
        playerPosition.x - m_Position.x,
        0.0f, // 水平方向のみ計算
        playerPosition.z - m_Position.z
    };


    XMFLOAT3 rotation;
    rotation.x = rota.x - m_Rotation.x;
    rotation.z = rota.z - m_Rotation.z;

    // プレイヤーへの角度を計算
    float angle = atan2(m_Rotation.x + direction.x, m_Rotation.z + direction.z);

    // プレイヤーのいる方向へ回転 (Y軸回転)
    m_Rotation.y = angle;


    if (player)
    {
        XMFLOAT3 playerPosition = player->GetPostion();

        // プレイヤーとの距離を計算
        float distance = sqrtf(powf(m_Position.x - playerPosition.x, 2) +
            powf(m_Position.z - playerPosition.z, 2));  // 2Dでの距離
    }

    // 全てのEnemyMobと衝突判定を行う
    auto gameObjects = scene->GetGameObjects<EnemyMob>();

    for (GameObject* obj : gameObjects)
    {
        EnemyMob* otherEnemyMob = dynamic_cast<EnemyMob*>(obj);
        if (otherEnemyMob && otherEnemyMob != this) // 自分自身は無視
        {
            XMFLOAT3 otherPosition = otherEnemyMob->GetPostion();
            float distance = sqrtf(powf(m_Position.x - otherPosition.x, 2) +
                                   powf(m_Position.z - otherPosition.z, 2)); // 2Dでの距離

            float combinedRadius = m_CollisionRadius + otherEnemyMob->m_CollisionRadius;

            // 衝突している場合
            if (distance < combinedRadius)
            {
                // 押し戻す方向を計算
                XMFLOAT3 pushDirection;
                pushDirection.x = m_Position.x - otherPosition.x;
                pushDirection.z = m_Position.z - otherPosition.z;

                // 方向ベクトルを正規化
                float length = sqrtf(pushDirection.x * pushDirection.x + pushDirection.z * pushDirection.z);
                if (length != 0)
                {
                    pushDirection.x /= length;
                    pushDirection.z /= length;
                }

                // 押し戻し量を計算
                float overlap = combinedRadius - distance;

                // 自分を押し戻す
                m_Position.x += pushDirection.x * overlap * 0.5f; // 0.5fでお互いを均等に押し返す
                m_Position.z += pushDirection.z * overlap * 0.5f;

                // 相手も押し戻す
                otherEnemyMob->m_Position.x -= pushDirection.x * overlap * 0.5f;
                otherEnemyMob->m_Position.z -= pushDirection.z * overlap * 0.5f;
            }
        }
    }

    PlayerChase();

    m_AnimationBlendRatio += 0.1f;
    if (m_AnimationBlendRatio > 1.0f)
        m_AnimationBlendRatio = 1.0f;

    m_Component->Update();

    // 地形の高さに合わせてY座標を調整
    if (m_Position.y < groundHeight)
    {
        m_Position.y = groundHeight;
    }
    else
    {
        if (m_Position.y > groundHeight)
        {
            //m_Position.y -= 0.1f; // 自然落下
        }
    }
}

void EnemyMob::Uninit()
{
    delete m_Component;

    m_VertexLayout->Release();
    m_VertexShader->Release();
    m_PixelShader->Release();
}

void EnemyMob::Draw()
{
    ((AnimationModel*)m_Component)->Update(m_AnimationName1.c_str(), m_AnimationFrame,
        m_AnimationName2.c_str(), m_AnimationFrame,
        m_AnimationBlendRatio);
    m_AnimationFrame++;

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

    m_Component->Draw();
}

void EnemyMob::PlayerChase()
{
    Scene* scene = Manager::GetScene();

    Player* player = scene->GetGameObject<Player>();
    XMFLOAT3 playerPosition = player->GetPostion();

    // プレイヤー方向のベクトルを計算
    XMFLOAT3 direction =
    {
        playerPosition.x - m_Position.x,
        0.0f, // 水平方向のみ計算
        playerPosition.z - m_Position.z
    };

    // ベクトルの長さを計算
    float length = sqrtf(direction.x * direction.x + direction.z * direction.z);

    // プレイヤーへの角度を計算
    float angle = atan2(direction.x, direction.z);

    // プレイヤーとの距離が3以下のとき
    if (length <= 3.0f)
    {
        if (m_AnimationName2 != "Attack")
        {
            m_AnimationName1 = m_AnimationName2;
            m_AnimationName2 = "Attack";
            m_AnimationBlendRatio = 0.0f;
        }
    }
    // プレイヤーが一定距離離れたら再び追いかける
    else if (length > 6.0f)
    {
        // 方向ベクトルを正規化
        direction.x /= length;
        direction.z /= length;

        // 一定の速度で移動
        float speed = 0.05f;
        m_Position.x += direction.x * speed;
        m_Position.z += direction.z * speed;

        // プレイヤーのいる方向への回転 (Y軸回転)
        m_Rotation.y = angle;

        if (m_AnimationName2 != "Run")
        {
            m_AnimationName1 = m_AnimationName2;
            m_AnimationName2 = "Run";
            m_AnimationBlendRatio = 0.0f;
        }
    }
    else
    {
        // Idleアニメーションを設定
        if (m_AnimationName2 != "Idle")
        {
            m_AnimationName1 = m_AnimationName2;
            m_AnimationName2 = "Idle";
            m_AnimationBlendRatio = 0.0f;
        }
    }
}
