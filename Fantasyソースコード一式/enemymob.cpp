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

    // ���b�V���t�B�[���h���獂�����擾
    MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>();
    float groundHeight = meshField->GetHeight(m_Position);

    XMFLOAT3 rota = m_Rotation;

    // �v���C���[�����̃x�N�g�����v�Z
    XMFLOAT3 direction = 
    {
        playerPosition.x - m_Position.x,
        0.0f, // ���������̂݌v�Z
        playerPosition.z - m_Position.z
    };


    XMFLOAT3 rotation;
    rotation.x = rota.x - m_Rotation.x;
    rotation.z = rota.z - m_Rotation.z;

    // �v���C���[�ւ̊p�x���v�Z
    float angle = atan2(m_Rotation.x + direction.x, m_Rotation.z + direction.z);

    // �v���C���[�̂�������։�] (Y����])
    m_Rotation.y = angle;


    if (player)
    {
        XMFLOAT3 playerPosition = player->GetPostion();

        // �v���C���[�Ƃ̋������v�Z
        float distance = sqrtf(powf(m_Position.x - playerPosition.x, 2) +
            powf(m_Position.z - playerPosition.z, 2));  // 2D�ł̋���
    }

    // �S�Ă�EnemyMob�ƏՓ˔�����s��
    auto gameObjects = scene->GetGameObjects<EnemyMob>();

    for (GameObject* obj : gameObjects)
    {
        EnemyMob* otherEnemyMob = dynamic_cast<EnemyMob*>(obj);
        if (otherEnemyMob && otherEnemyMob != this) // �������g�͖���
        {
            XMFLOAT3 otherPosition = otherEnemyMob->GetPostion();
            float distance = sqrtf(powf(m_Position.x - otherPosition.x, 2) +
                                   powf(m_Position.z - otherPosition.z, 2)); // 2D�ł̋���

            float combinedRadius = m_CollisionRadius + otherEnemyMob->m_CollisionRadius;

            // �Փ˂��Ă���ꍇ
            if (distance < combinedRadius)
            {
                // �����߂��������v�Z
                XMFLOAT3 pushDirection;
                pushDirection.x = m_Position.x - otherPosition.x;
                pushDirection.z = m_Position.z - otherPosition.z;

                // �����x�N�g���𐳋K��
                float length = sqrtf(pushDirection.x * pushDirection.x + pushDirection.z * pushDirection.z);
                if (length != 0)
                {
                    pushDirection.x /= length;
                    pushDirection.z /= length;
                }

                // �����߂��ʂ��v�Z
                float overlap = combinedRadius - distance;

                // �����������߂�
                m_Position.x += pushDirection.x * overlap * 0.5f; // 0.5f�ł��݂����ϓ��ɉ����Ԃ�
                m_Position.z += pushDirection.z * overlap * 0.5f;

                // ����������߂�
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

    // �n�`�̍����ɍ��킹��Y���W�𒲐�
    if (m_Position.y < groundHeight)
    {
        m_Position.y = groundHeight;
    }
    else
    {
        if (m_Position.y > groundHeight)
        {
            //m_Position.y -= 0.1f; // ���R����
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

    // ���̓��C�A�E�g�ݒ�
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    // �V�F�[�_�ݒ�
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    // ���[���h�}�g���N�X�ݒ�
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

    // �v���C���[�����̃x�N�g�����v�Z
    XMFLOAT3 direction =
    {
        playerPosition.x - m_Position.x,
        0.0f, // ���������̂݌v�Z
        playerPosition.z - m_Position.z
    };

    // �x�N�g���̒������v�Z
    float length = sqrtf(direction.x * direction.x + direction.z * direction.z);

    // �v���C���[�ւ̊p�x���v�Z
    float angle = atan2(direction.x, direction.z);

    // �v���C���[�Ƃ̋�����3�ȉ��̂Ƃ�
    if (length <= 3.0f)
    {
        if (m_AnimationName2 != "Attack")
        {
            m_AnimationName1 = m_AnimationName2;
            m_AnimationName2 = "Attack";
            m_AnimationBlendRatio = 0.0f;
        }
    }
    // �v���C���[����苗�����ꂽ��Ăђǂ�������
    else if (length > 6.0f)
    {
        // �����x�N�g���𐳋K��
        direction.x /= length;
        direction.z /= length;

        // ���̑��x�ňړ�
        float speed = 0.05f;
        m_Position.x += direction.x * speed;
        m_Position.z += direction.z * speed;

        // �v���C���[�̂�������ւ̉�] (Y����])
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
        // Idle�A�j���[�V������ݒ�
        if (m_AnimationName2 != "Idle")
        {
            m_AnimationName1 = m_AnimationName2;
            m_AnimationName2 = "Idle";
            m_AnimationBlendRatio = 0.0f;
        }
    }
}
