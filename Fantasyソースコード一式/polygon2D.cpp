#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "polygon2D.h"
#include "camera.h"
#include "audio.h"


void Polygon2D::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void Polygon2D::Uninit()
{
	m_VertexBuffer->Release();
	//m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Polygon2D::Update()
{

}

void Polygon2D::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(0.0, 0.0, 0.0, 0.5f);
	material.TextureEnable = false;
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�{���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �X�e���V���Ǎ��L��
	Renderer::SetStencilEnable(false);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);

	// �X�e���V������
	Renderer::SetDepthEnable(true);

	Camera* camera = Manager::GetScene()->GetGameObject<Camera>();

	if (camera != nullptr)
		camera->Draw();
}
