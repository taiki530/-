#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "fireeffect.h"
#include "sscamera.h"


void FireEffect::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC; //���_�̍��W������������ׂ�DEFAULT��DYNAMIC�ɕς���
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//�e�N�X�`���ǂݍ���
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asse\\Effect\\Fire_A_2.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

}

void FireEffect::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void FireEffect::Update()
{
	int count = 50;

	//�p�[�e�B�N������
	for (int i = 0; i < FIREEFFECT_MAX; i++)
	{
		if (g_FireEffect[i].Enable == false)
		{
			g_FireEffect[i].Enable = true;
			g_FireEffect[i].Life = 60;
			g_FireEffect[i].Position = m_Position;

			g_FireEffect[i].Velocity.x = (rand() % 100 - 50) / 500.0f;
			g_FireEffect[i].Velocity.y = (rand() % 100 + 50) / 500.0f;
			g_FireEffect[i].Velocity.z = (rand() % 100 - 50) / 500.0f;

			break;
		}
	}
	//�p�[�e�B�N���ړ�
	for (int i = 0; i < FIREEFFECT_MAX; i++)
	{
		if (g_FireEffect[i].Enable == true)
		{
			// �d��
			g_FireEffect[i].Velocity.y += -0.01f;

			// �ړ�
			g_FireEffect[i].Position.x += g_FireEffect[i].Velocity.x;
			g_FireEffect[i].Position.y += g_FireEffect[i].Velocity.y;
			g_FireEffect[i].Position.z += g_FireEffect[i].Velocity.z;

			// ���C�t
			g_FireEffect[i].Life--;
			if (g_FireEffect[i].Life == 0)
			{
				g_FireEffect[i].Enable = false;
			}
		}
	}
}

void FireEffect::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �J�����̃r���[�}�g���N�X�擾
	Scene* scene = Manager::GetScene();
	SsCamera* Sscamera = scene->GetGameObject<SsCamera>();
	XMMATRIX view = Sscamera->GetViewMatrix();

	//�@�r���[�̋t�s�� (�t�s��̊ȒP�ȃC���[�W�|���Z�̔��΂̊���Z�݂����ȃC���[�W)
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view); //�t�s��
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0, 1.0, 1.0, 1.0);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�{���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//Z�o�b�t�@����
	Renderer::SetDepthEnable(false);

	for (int i = 0; i < FIREEFFECT_MAX; i++)
	{
		if (g_FireEffect[i].Enable == true)
		{
			// ���[���h�}�g���N�X�ݒ�
			XMMATRIX world, scale, trans;
			scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
			trans = XMMatrixTranslation(g_FireEffect[i].Position.x,
				g_FireEffect[i].Position.y,
				g_FireEffect[i].Position.z);
			world = scale * invView * trans;
			Renderer::SetWorldMatrix(world);

			// �|���S���`��
			Renderer::GetDeviceContext()->Draw(4, 0);
		}
	}

	//Z�o�b�t�@����
	Renderer::SetDepthEnable(true);

	// ���Z��������
	Renderer::SetBlendAddEnable(false);
}