#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "enemyHp.h"
#include "input.h"
#include "result.h"
#include <iostream>
#include "enemy.h" 

void EnemyHpBar::Init()
{
	// 初期化時にエネミーのHPを取得して設定 
	m_MaxHp = 100.0f;
	m_CurrentHp = m_MaxHp;

	// HPバーの最大幅（ピクセル単位）
	m_MaxWidth = 388.0f; // (834.0f - 446.0f)

	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(446.0f, 666.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(446.0f + m_MaxWidth, 666.0f, 0.0f); // 初期は最大幅
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(446.0f, 694.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(446.0f + m_MaxWidth, 694.0f, 0.0f); // 初期は最大幅
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//頂点バッファ生成
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

	// テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asse\\texture\\bar.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void EnemyHpBar::Uninit()
{
	if (m_VertexBuffer) m_VertexBuffer->Release();
	if (m_Texture) m_Texture->Release();
	// 追加: シェーダと入力レイアウトの解放
	if (m_VertexLayout) m_VertexLayout->Release();
	if (m_VertexShader) m_VertexShader->Release();
	if (m_PixelShader) m_PixelShader->Release();
}

void EnemyHpBar::Update()
{
	
	Enemy* enemy = Manager::GetScene()->GetGameObject<Enemy>();
	if (enemy) {
		
	}

	// デバッグ用キー入力（削除予定）
	if (Input::GetKeyPress('M'))
	{
		m_CurrentHp -= 0.1f;
		if (m_CurrentHp < 0.0f) m_CurrentHp = 0.0f;
	}

	// エネミーのHPが0になったら結果画面へ (実際のゲームロジックに合わせる)
	if (m_CurrentHp <= 0.0f)
	{
		Manager::SetScene<Result>();
	}
}

void EnemyHpBar::Draw()
{
	// HPの割合に基づいて幅を計算
	float hpRatio = m_CurrentHp / m_MaxHp;
	float currentWidth = m_MaxWidth * hpRatio;

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = XMFLOAT3(446.0f, 666.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(446.0f + currentWidth, 666.0f, 0.0f); // 計算された幅を使用
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(hpRatio, 0.0f); // テクスチャ座標も幅に合わせて調整

	vertex[2].Position = XMFLOAT3(446.0f, 694.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(446.0f + currentWidth, 694.0f, 0.0f); // 計算された幅を使用
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(hpRatio, 1.0f); // テクスチャ座標も幅に合わせて調整

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

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

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void EnemyHpBar::SetCurrentHp(float hp) {
	m_CurrentHp = hp;
	if (m_CurrentHp < 0.0f) m_CurrentHp = 0.0f;
	if (m_CurrentHp > m_MaxHp) m_CurrentHp = m_MaxHp; // 最大HPを超えないように
}