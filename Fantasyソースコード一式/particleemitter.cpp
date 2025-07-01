#include "main.h"
#include "renderer.h"
#include "particleemitter.h"
#include "camera.h"
#include "scene.h"
#include "manager.h"

void ParticleEmitter::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC; //頂点の座標を書き換える為にDEFAULTをDYNAMICに変える
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asse\\Effect\\particle.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

}

void ParticleEmitter::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void ParticleEmitter::Update()
{
	int count = 50;

	//パーティクル発射
	for (int i = 0; i < PARTICLE_MAX; i++) 
	{
		if (m_Particle[i].Enable == false)
		{
			m_Particle[i].Enable == true;
			m_Particle[i].Life = 60;
			m_Particle[i].Position = m_Position;

			m_Particle[i].Velocity.x = (rand() % 100 - 50) / 500.0f;
			m_Particle[i].Velocity.y = (rand() % 100 + 50) / 500.0f;
			m_Particle[i].Velocity.z = (rand() % 100 - 50) / 500.0f;

			break;
		}
	}
	//パーティクル移動
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			// 重力
			m_Particle[i].Velocity.y += -0.01f;

			// 移動
			m_Particle[i].Position.x += m_Particle[i].Velocity.x;
			m_Particle[i].Position.y += m_Particle[i].Velocity.y;
			m_Particle[i].Position.z += m_Particle[i].Velocity.z;//dtを使ってやった方がフレームレート更新が安定する

			// ライフ
			m_Particle[i].Life--;
			if (m_Particle[i].Life == 0)
			{
				m_Particle[i].Enable = false;
			}
		}
	}
}

void ParticleEmitter::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// カメラのビューマトリクス取得
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	XMMATRIX view = camera->GetViewMatrix();

	//　ビューの逆行列 (逆行列の簡単なイメージ掛け算の反対の割り算みたいなイメージ)
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view); //逆行列
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0, 1.0, 1.0, 1.0);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトボロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//Zバッファ無効
	Renderer::SetDepthEnable(false);

	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			// ワールドマトリクス設定
			XMMATRIX world, scale, trans;
			scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
			trans = XMMatrixTranslation(m_Particle[i].Position.x,
										m_Particle[i].Position.y,
										m_Particle[i].Position.z);
			world = scale * invView * trans;
			Renderer::SetWorldMatrix(world);

			// ポリゴン描画
			Renderer::GetDeviceContext()->Draw(4, 0);
		}
	}
	
	//Zバッファ無効
	Renderer::SetDepthEnable(true);

	// 加算合成無効
	//Renderer::SetBlendAddEnable(false);

}
