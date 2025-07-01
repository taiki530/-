#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "frustum.h"
#include "input.h"
#include "result.h"
#include "enemyCamera.h"

void EnemyCamera::Init()
{
	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Target = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_At = XMFLOAT3(0.0f, 0.0f, 0.0f);
	isEnemyVisible = false;
}

void EnemyCamera::Uninit()
{
}


void EnemyCamera::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Enemy* enemy = scene->GetGameObject<Enemy>();

	if (!player || !enemy) return; // �v���C���[���G�l�~�[�����݂��Ȃ��ꍇ�͏����𒆒f

	XMFLOAT3 playerPos = player->GetPostion();
	XMFLOAT3 enemyPos = enemy->GetPostion();

	m_Target = enemyPos;
	m_Target.y += 5.0f; // �G�l�~�[�̏�������^�[�Q�b�g
	m_Position.x = m_Target.x - sinf(m_Rotation.y) * 2.0f;
	m_Position.y = m_Target.y;
	m_Position.z = m_Target.z - cosf(m_Rotation.y) * 2.0f;

	// ������̌v�Z
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_Target), XMLoadFloat3(&up));
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.5f, 100.0f);
	XMMATRIX viewProjMatrix = viewMatrix * projectionMatrix;

	// �t���X�^�����쐬
	Frustum frustum;
	frustum.CreateFrustum(viewProjMatrix);

	// �v���C���[�̔���i�G�l�~�[�J�������猩���v���C���[�̎��E����j
	XMFLOAT3 min = XMFLOAT3(playerPos.x - 0.5f, playerPos.y - 1.0f, playerPos.z - 0.5f); // �v���C���[��AABB�̍ŏ��_
	XMFLOAT3 max = XMFLOAT3(playerPos.x + 0.5f, playerPos.y + 1.0f, playerPos.z + 0.5f); // �v���C���[��AABB�̍ő�_

	bool isCurrentlyVisible = frustum.IsIntersecting(min, max);

	// �G�l�~�[�Ɏ��E����̌��ʂ�ʒm
	enemy->SetPlayerVisible(isCurrentlyVisible);

}

void EnemyCamera::Draw()
{
	// �v���W�F�N�V�����}�g���N�X�ݒ�
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	// �v���W�F�N�V�����s��������_���[�ɐݒ�
	Renderer::SetProjectionMatrix(projectionMatrix);
	// Create a window called "My First Tool", with a menu bar.
	ImGui::Begin("Enemy");
	ImGui::InputFloat3("EnemyCameraPos", (float*)&m_Position);
	ImGui::InputFloat3("EnemyCameraRot", (float*)&m_Rotation);
	ImGui::End();
}
