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
    m_Speed = 0.3f; // �e�̈ړ����x���������߂ɒ���
    m_Component = new ModelRenderer(this);
    ((ModelRenderer*)m_Component)->Load("asse\\model\\bullet.obj");

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\unlitTextureVS.cso");
    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\unlitTexturePS.cso");
}

void Bullet::Uninit()
{
    if (m_Component) { // null�`�F�b�N��ǉ�
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

    // �����x�N�g����p���Ĉʒu���X�V
    m_Position.x += m_Direction.x * m_Speed;
    m_Position.y += m_Direction.y * m_Speed;
    m_Position.z += m_Direction.z * m_Speed;

    //�e�ۂ��X�|�[���n�_�����苗�����ꂽ�����
    float distanceToSpawnSquared =
        (m_Position.x - m_SpawnPosition.x) * (m_Position.x - m_SpawnPosition.x) +
        (m_Position.y - m_SpawnPosition.y) * (m_Position.y - m_SpawnPosition.y) +
        (m_Position.z - m_SpawnPosition.z) * (m_Position.z - m_SpawnPosition.z);

    if (distanceToSpawnSquared > 50.0f * 50.0f) // �����̓��Ŕ�r�B�Ⴆ��50�P�ʗ��ꂽ��
    {
        // �e��������Ƃ��ɔ����G�t�F�N�g���o���ꍇ
        Explosion* explosion = scene->AddGameObject<Explosion>(1);
        if (explosion) { // Null�`�F�b�N
            explosion->SetPostion(m_Position);
        }
        SetDestroy();
        return; // �j�󂳂ꂽ��ȍ~�̏����͕s�v
    }

    auto boxList = scene->GetGameObjects<Box>();

    for (Box* box : boxList)
    {
        XMFLOAT3 boxPosition = box->GetPostion();
        XMFLOAT3 boxObjectScale = box->GetScale();

        float boxHalfWidthX = boxObjectScale.x;
        float boxHalfWidthZ = boxObjectScale.z;


        float bulletRadius = 0.5f; // �e�̓����蔻�蔼�a

        // OBB (Oriented Bounding Box) �Փ˔���
        // Box�̒��S����e�̒��S�ւ̃x�N�g��
        XMFLOAT3 dirFromBoxToBullet;
        dirFromBoxToBullet.x = m_Position.x - boxPosition.x;
        dirFromBoxToBullet.y = m_Position.y - boxPosition.y;
        dirFromBoxToBullet.z = m_Position.z - boxPosition.z;

        // �{�b�N�X�̃��[�J�����W�n�ւ̕ϊ�
        // Box�̉�]���l��
        XMMATRIX boxRotationMatrix = XMMatrixRotationRollPitchYaw(box->GetRotation().x, box->GetRotation().y, box->GetRotation().z);
        XMMATRIX boxInverseRotationMatrix = XMMatrixInverse(nullptr, boxRotationMatrix);

        XMVECTOR localDirVec = XMVector3TransformNormal(XMLoadFloat3(&dirFromBoxToBullet), boxInverseRotationMatrix);
        XMFLOAT3 localDir;
        XMStoreFloat3(&localDir, localDirVec);

        // �e���ɉ���������
        float absLocalDirX = fabsf(localDir.x);
        float absLocalDirY = fabsf(localDir.y);
        float absLocalDirZ = fabsf(localDir.z);

        // OBB�Ƌ��̔���
        // ���̒��S��OBB�̊e���ɂǂꂾ���߂Â��Ă��邩
        // �����Ƃ̏Փ˔���
        if (absLocalDirX < (boxHalfWidthX + bulletRadius) &&
            absLocalDirY < (boxObjectScale.y + bulletRadius) && // Box��Y�X�P�[���ƒe�̔��a
            absLocalDirZ < (boxHalfWidthZ + bulletRadius))
        {
            // �Փˎ��̏���
            Explosion* explosion = scene->AddGameObject<Explosion>(1);
            if (explosion) { // Null�`�F�b�N
                explosion->SetPostion(m_Position);
            }
            SetDestroy();
            return; // �j�󂳂ꂽ��ȍ~�̏����͕s�v
        }
    }

    auto enemyList = scene->GetGameObjects<Enemy>();
    for (Enemy* enemy : enemyList)
    {
        XMFLOAT3 enemyPosition = enemy->GetPostion();
        // �G�l�~�[�̃X�P�[����GameObject::m_Scale����擾
        XMFLOAT3 enemyObjectScale = enemy->GetScale();

        
        float collisionScaleFactor = 3.0f; // ���̒l�𒲐����ē����蔻��̑傫����ύX

        // OBB�̔���/�����s���ɃX�P�[���t�@�N�^�[��K�p
        float enemyHalfWidthX = enemyObjectScale.x * collisionScaleFactor;
        float enemyHalfWidthZ = enemyObjectScale.z * collisionScaleFactor;
        float enemyHalfHeightY = enemyObjectScale.y * collisionScaleFactor; // Y���������l��

        // �e�̔��a
        float bulletRadius = 0.5f; 

        // �G�l�~�[�̒��S����e�̒��S�ւ̃x�N�g��
        XMFLOAT3 dirFromEnemyToBullet;
        dirFromEnemyToBullet.x = m_Position.x - enemyPosition.x;
        dirFromEnemyToBullet.y = m_Position.y - enemyPosition.y;
        dirFromEnemyToBullet.z = m_Position.z - enemyPosition.z;

        // �G�l�~�[�̃��[�J�����W�n�ւ̕ϊ�
        // �G�l�~�[�̉�]���l��
        XMMATRIX enemyRotationMatrix = XMMatrixRotationRollPitchYaw(enemy->GetRotation().x, enemy->GetRotation().y, enemy->GetRotation().z);
        XMMATRIX enemyInverseRotationMatrix = XMMatrixInverse(nullptr, enemyRotationMatrix);

        XMVECTOR localDirVec = XMVector3TransformNormal(XMLoadFloat3(&dirFromEnemyToBullet), enemyInverseRotationMatrix);
        XMFLOAT3 localDir;
        XMStoreFloat3(&localDir, localDirVec);

        // �e���ɉ���������
        float absLocalDirX = fabsf(localDir.x);
        float absLocalDirY = fabsf(localDir.y);
        float absLocalDirZ = fabsf(localDir.z);

        // OBB�Ƌ��̔���
        if (absLocalDirX < (enemyHalfWidthX + bulletRadius) &&
            absLocalDirY < (enemyHalfHeightY + bulletRadius) &&
            absLocalDirZ < (enemyHalfWidthZ + bulletRadius))
        {
            // �Փˎ��̏���
            Explosion* explosion = scene->AddGameObject<Explosion>(1);
            if (explosion) { // Null�`�F�b�N
                explosion->SetPostion(m_Position);
            }

            // �G�l�~�[�Ƀ_���[�W��^����
            enemy->TakeDamage(20.0f); // �_���[�W��

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
    // ���K�����Ēe�̑��x�����ɂȂ�悤�ɂ���
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
