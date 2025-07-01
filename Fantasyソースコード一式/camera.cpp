#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "enemyHpwaku.h"
#include "enemyHp.h"
#include "frustum.h"
#include "input.h"
#include "result.h"

void Camera::Init()
{
	m_Position = XMFLOAT3(0.0f, 5.0f, -10.0f);
	m_Target = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_At = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Len = 6.0f; // ������
	isEnemyVisible = false;
	isEnemyHpDisplayed = false;

}

void Camera::Uninit()
{
}


void Camera::Update()
{
	Scene* scene;
	scene = Manager::GetScene();
	Player* player;
	player = scene->GetGameObject<Player>();
	Enemy* enemy = scene->GetGameObject<Enemy>();
	XMFLOAT3 enemyPos = enemy->GetPostion(); // �G�l�~�[�̈ʒu���擾

	m_Target = player->GetPostion();
	m_Target.y += 2.0f;

	// �}�E�X�z�C�[���ł̃Y�[������
	// GetMouseWheelDelta() �̎���������΂�����g��
	// ����ł� Input::Update() �Ń}�E�X�̏�Ԃ��X�V���Ă��邽�߁A�����ɒ��ڋL�q
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(GetWindow(), &mousePos);

	static int lastMouseWheelDelta = 0;
	int currentMouseWheelDelta = GET_WHEEL_DELTA_WPARAM(Input::GetMouseWheelDelta()); // main.h�Œ�`����Ă���GET_WHEEL_DELTA_WPARAM���g�p
	if (currentMouseWheelDelta != lastMouseWheelDelta) {
		m_Len -= (float)(currentMouseWheelDelta / WHEEL_DELTA) * 0.5f; // �z�C�[���̉�]�ʂɉ����ċ����𒲐�
		if (m_Len < 2.0f) m_Len = 2.0f; // �ŏ�����
		if (m_Len > 20.0f) m_Len = 20.0f; // �ő勗��
		lastMouseWheelDelta = currentMouseWheelDelta;
	}

	m_Position.x = m_Target.x - sinf(m_Rotation.y) * m_Len; // m_Len���g�p
	m_Position.y = m_Target.y + 2.0f;
	m_Position.z = m_Target.z - cosf(m_Rotation.y) * m_Len; // m_Len���g�p

	// ������̌v�Z
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_Target), XMLoadFloat3(&up));
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(0.5f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.5f, 100.0f);
	XMMATRIX viewProjMatrix = viewMatrix * projectionMatrix;

	// �t���X�^�����쐬
	Frustum frustum;
	frustum.CreateFrustum(viewProjMatrix);

	// �G�l�~�[�̔���
	XMFLOAT3 min = XMFLOAT3(enemyPos.x - 1.0f, enemyPos.y - 1.0f, enemyPos.z - 1.0f); // AABB�̍ŏ��_
	XMFLOAT3 max = XMFLOAT3(enemyPos.x + 1.0f, enemyPos.y + 1.0f, enemyPos.z + 1.0f); // AABB�̍ő�_

	bool isCurrentlyVisible = frustum.IsIntersecting(min, max);

	if (isCurrentlyVisible)
	{
		if (!isEnemyHpDisplayed)
		{
			scene->AddGameObject<EnemyHpwaku>(2);
			scene->AddGameObject<EnemyHpBar>(2);
			isEnemyHpDisplayed = true;
		}
		isEnemyVisible = true;
	}
	else
	{
		isEnemyVisible = false; // ���E����������烊�Z�b�g
	}

	if (Input::GetKeyPress(VK_RIGHT))
	{
		m_Rotation.y += 0.02f;
	}

	if (Input::GetKeyPress(VK_LEFT))
	{
		m_Rotation.y -= 0.02f;
	}
}

void Camera::Draw()
{
	// �r���[�}�g���N�X�ݒ�
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_Target), XMLoadFloat3(&up));

	// �r���[�s��������_���[�ɐݒ�
	Renderer::SetViewMatrix(viewMatrix);

	// �r���[�s���ۑ�
	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	// �v���W�F�N�V�����}�g���N�X�ݒ�
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	// �v���W�F�N�V�����s��������_���[�ɐݒ�
	Renderer::SetProjectionMatrix(projectionMatrix);

	// Create a window called "My First Tool", with a menu bar.
	ImGui::Begin("Camera");
	ImGui::InputFloat3("PlayerCameraPos", (float*)&m_Position);
	ImGui::End();
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
}

XMVECTOR Camera::GetPosition()
{
	Camera* camera = Manager::GetScene()->GetGameObject<Camera>();
	if (camera)
	{
		return XMLoadFloat3(&camera->m_Position);
	}
	return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}