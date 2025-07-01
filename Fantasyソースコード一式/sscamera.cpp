#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "sscamera.h"
#include "player.h"
#include "input.h"

void SsCamera::Init()
{
	m_Position = XMFLOAT3(0.0f, 5.0f, -10.0f);
	m_Target = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_At = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void SsCamera::Uninit()
{
}

void SsCamera::Update()
{
	Scene* scene;
	scene = Manager::GetScene();

	Box* box;
	box = scene->GetGameObject<Box>();

	m_Target = box->GetPostion();
	m_Target.y += 2.0f;


	m_Position.x = m_Target.x - sinf(m_Rotation.y) * 6.0f;
	m_Position.y = m_Target.y + 2.0f;
	m_Position.z = m_Target.z - cosf(m_Rotation.y) * 6.0f;

	m_Rotation.y += 0.002f;
}

void SsCamera::Draw()
{
	// ビューマトリクス設定
	XMFLOAT3 up{ 0.0f, 1.0f, 0.0f };
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_Target), XMLoadFloat3(&up));

	Renderer::SetViewMatrix(viewMatrix);

	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	// プロジェクションマトリクス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projectionMatrix);
}
