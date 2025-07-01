#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "hpbar.h"
#include "input.h"
#include "result.h"
#include "gameover.h"
#include "player.h" 


void HpBar::Init()
{
	// ���������Ƀv���C���[��HP���擾���Đݒ�
	Player* player = Manager::GetScene()->GetGameObject<Player>();
	if (player) {
		m_MaxHp = player->GetMaxHp();
		m_CurrentHp = player->GetHp();
	}
	else {
		m_MaxHp = 100.0f; // �f�t�H���g�l
		m_CurrentHp = 100.0f; // �f�t�H���g�l
	}

	// HP�o�[�̍ő啝�i�s�N�Z���P�ʁj
	// �Ⴆ�΁A�t���[���̕�����}�[�W�����������l�Ȃ�
	m_MaxWidth = 388.0f; 

	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(6.0f, 6.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(6.0f + m_MaxWidth, 6.0f, 0.0f); // �����͍ő啝
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(6.0f, 44.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(6.0f + m_MaxWidth, 44.0f, 0.0f); // �����͍ő啝
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asse\\texture\\bar.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void HpBar::Uninit()
{
	if (m_VertexBuffer) m_VertexBuffer->Release();
	if (m_Texture) m_Texture->Release();
	// �ǉ�: �V�F�[�_�Ɠ��̓��C�A�E�g�̉��
	if (m_VertexLayout) m_VertexLayout->Release();
	if (m_VertexShader) m_VertexShader->Release();
	if (m_PixelShader) m_PixelShader->Release();
}

void HpBar::Update()
{
	// Player���猻�݂�HP���擾��������
	Player* player = Manager::GetScene()->GetGameObject<Player>();
	if (player) {
		m_CurrentHp = player->GetHp();
	}
}

void HpBar::Draw()
{
	// HP�̊����Ɋ�Â��ĕ����v�Z
	float hpRatio = m_CurrentHp / m_MaxHp;
	float currentWidth = m_MaxWidth * hpRatio;

	//���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = XMFLOAT3(6.0f, 6.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(6.0f + currentWidth, 6.0f, 0.0f); // �v�Z���ꂽ�����g�p
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(hpRatio, 0.0f); // �e�N�X�`�����W�����ɍ��킹�Ē���

	vertex[2].Position = XMFLOAT3(6.0f, 44.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(6.0f + currentWidth, 44.0f, 0.0f); // �v�Z���ꂽ�����g�p
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(hpRatio, 1.0f); // �e�N�X�`�����W�����ɍ��킹�Ē���

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

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
	material.Diffuse = XMFLOAT4(1.0, 1.0, 1.0, 1.0);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�{���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void HpBar::SetCurrentHp(float hp) 
{
	m_CurrentHp = hp;
}