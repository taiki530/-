#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "shadowvolum.h"
#include "modelRenderer.h"

void ShadowVolum::Init()
{
	m_Scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

	m_Component = new ModelRenderer(this);
	((ModelRenderer*)m_Component)->Load("asse\\model\\sphere_smooth.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\envmappingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\envmappingPS.cso");
}

void ShadowVolum::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void ShadowVolum::Update()
{	
	m_Rotation.x += 0.01f;
	m_Rotation.y += 0.01f;
}

void ShadowVolum::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_EnvTexture);

	Renderer::SetBlendMaskEnable(true);

	Renderer::SetStencilEnable(true);

	Renderer::SetCullEnable(false);


	m_Component->Draw();

	Renderer::SetBlendMaskEnable(false);

	Renderer::SetDepthEnable(true);

	Renderer::SetCullEnable(true);
}
