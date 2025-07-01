#include "main.h"
#include "renderer.h"
#include "sky.h"
#include "modelRenderer.h"


void Sky::Init()
{

	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_Scale = XMFLOAT3(100.0f, 100.0f, 100.0f);

	m_Component = new ModelRenderer(this);

	((ModelRenderer*)m_Component)->Load("asse\\model\\sky.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void Sky::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Sky::Update()
{
	m_Component->Update();
}

void Sky::Draw()
{
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

