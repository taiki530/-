#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "meshField.h"
#include "animationModel.h"
#include "modelRenderer.h"
#include "input.h"
#include "scene.h"
#include "hpbar.h"
#include "audio.h"
#include <iostream>
#include "enemy.h"
#include "bullet.h"
#include "shadowVolum.h"
#include "box.h"
#include "camera.h"
#include "gameover.h"


void Player::Init()
{
	m_Component = new AnimationModel(this);
	((AnimationModel*)m_Component)->Load("asse\\model\\Ch19_nonPBR.fbx");
	((AnimationModel*)m_Component)->LoadAnimation("asse\\model\\Idle.fbx", "Idle");
	((AnimationModel*)m_Component)->LoadAnimation("asse\\model\\Running.fbx", "Run");

	m_AnimationName1 = "Idle";
	m_AnimationName2 = "Idle";

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	//効果音ロード
	m_SE = new Audio(this);
	m_SE->Load("asse\\audio\\wan.wav");

	// クォータニオン初期化
	m_Quaternion.x = 0.0f;
	m_Quaternion.y = 0.0f;
	m_Quaternion.z = 0.0f;
	m_Quaternion.w = 1.0f; //クォータニオンの初期化状態はwは1である必要がある。

	m_Scale = { 0.01f, 0.01f, 0.01f };

	m_BulletCooldown = 60.0f;
	m_ShootFrameCounter = 0.0f;
	m_length = 0.0f;

	m_BGM = new Audio(this);
	m_BGM->Load("asse\\audio\\Sinking_Effect.wav");
	m_BGM->Play(true);

	// HPの初期化
	m_MaxHp = 100.0f;
	m_Hp = m_MaxHp;
}

void Player::Uninit()
{
	if (m_SE) {
		m_SE->Uninit();
		delete m_SE;
		m_SE = nullptr;
	}
	if (m_BGM) {
		m_BGM->Uninit();
		delete m_BGM;
		m_BGM = nullptr;
	}

	if (m_Component) {
		m_Component->Uninit();
		delete m_Component;
		m_Component = nullptr;
	}

	if (m_VertexLayout) {
		m_VertexLayout->Release();
		m_VertexLayout = nullptr;
	}
	if (m_VertexShader) {
		m_VertexShader->Release();
		m_VertexShader = nullptr;
	}
	if (m_PixelShader) {
		m_PixelShader->Release();
		m_PixelShader = nullptr;
	}
}

void Player::Update()
{
	XMFLOAT3 oldPosition = m_Position;

	Scene* scene = Manager::GetScene();

	m_Component->Update();

	Camera* camera = scene->GetGameObject<Camera>();
	if (!camera) {
		// エラー処理またはデフォルト動作
		return;
	}

	XMFLOAT3 forward = camera->GetForward();
	XMFLOAT3 right = camera->GetRight();
	XMFLOAT3 left = camera->GetLeft();

	m_AnimationName1 = "Idle";

	if (Input::GetKeyPress(VK_RIGHT))
	{
		XMVECTOR quat = XMQuaternionRotationRollPitchYaw(0.0f, +0.02f, 0.0f);
		quat = XMQuaternionMultiply(XMLoadFloat4(&m_Quaternion), quat);
		XMStoreFloat4(&m_Quaternion, quat);
	}

	if (Input::GetKeyPress(VK_LEFT))
	{
		XMVECTOR quat = XMQuaternionRotationRollPitchYaw(0.0f, -0.02f, 0.0f);
		quat = XMQuaternionMultiply(XMLoadFloat4(&m_Quaternion), quat);
		XMStoreFloat4(&m_Quaternion, quat);
	}

	bool isMoving = false;
	if (Input::GetKeyPress('W'))
	{
		m_Position.x += forward.x * 0.1f;
		m_Position.y += forward.y * 0.1f;
		m_Position.z += forward.z * 0.1f;
		isMoving = true;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Position.x -= forward.x * 0.1f;
		m_Position.y -= forward.y * 0.1f;
		m_Position.z -= forward.z * 0.1f;
		isMoving = true;
	}
	if (Input::GetKeyPress('A'))
	{
		m_Position.x -= right.x * 0.1f;
		m_Position.y -= right.y * 0.1f;
		m_Position.z -= right.z * 0.1f;
		isMoving = true;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Position.x += left.x * 0.1f;
		m_Position.y += left.y * 0.1f;
		m_Position.z += left.z * 0.1f;
		isMoving = true;
	}

	if (isMoving) {
		if (m_AnimationName2 != "Run")
		{
			m_AnimationName1 = m_AnimationName2;
			m_AnimationName2 = "Run";
			m_AnimationBlendRatio = 0.0f;
		}
	}
	else {
		if (m_AnimationName2 != "Idle")
		{
			m_AnimationName1 = m_AnimationName2;
			m_AnimationName2 = "Idle";
			m_AnimationBlendRatio = 0.0f;
		}
	}
	m_AnimationBlendRatio += 0.1f;
	if (m_AnimationBlendRatio > 1.0f)
		m_AnimationBlendRatio = 1.0f;


	// 重力
	m_Velocity.y -= 0.02f;

	// 移動
	m_Position.y += m_Velocity.y;

	MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>();
	float groundHeight = -FLT_MAX; // 初期値を十分に小さい値に設定

	if (meshField) { // meshFieldがnullでないことを確認
		groundHeight = meshField->GetHeight(m_Position);
	}
	else {
		// meshFieldが見つからない場合のエラー処理またはデフォルト動作
		// 例えば、地面の高さを0として扱うなど
		groundHeight = 0.0f;
	}


	// 地面より下にいる場合は地面に接地していると判定
	if (m_Position.y < groundHeight)
	{
		m_Position.y = groundHeight;
		isGrounded = true; // 地面に接地している
		m_Velocity.y = 0.0f; // 地面についたらY方向の速度をリセット
	}
	else
	{
		isGrounded = false; // 空中
	}

	// ジャンプ処理（空中ではジャンプできないように）
	if (Input::GetKeyTrigger(' ') && isGrounded) // 地面にいる場合のみジャンプ (GetKeyTriggerに変更)
	{
		m_Velocity.y = 0.4f; // ジャンプの初速度
		isGrounded = false;   // 空中にいる
	}

	auto boxList = scene->GetGameObjects<Box>();

	for (Box* box : boxList)
	{
		XMFLOAT3 boxPosition = box->GetPostion();
		XMFLOAT3 boxScale = box->GetScale();

		XMFLOAT3 direction;
		direction.x = boxPosition.x - m_Position.x;
		direction.y = boxPosition.y - m_Position.y;
		direction.z = boxPosition.z - m_Position.z;

		m_length = sqrtf(direction.x * direction.x
			+ direction.z * direction.z);

		if (m_length < boxScale.x)
		{
			if (m_Position.y < boxPosition.y + boxScale.y && oldPosition.y >= boxPosition.y + boxScale.y) {
				m_Position.y = boxPosition.y + boxScale.y;
				m_Velocity.y = 0.0f;
				isGrounded = true;
			}
			else {
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;
			}
		}
	}

	auto enemyList = scene->GetGameObjects<Enemy>();

	for (Enemy* enemy : enemyList)
	{
		XMFLOAT3 enemyPosition = enemy->GetPostion();
		XMFLOAT3 direction;
		direction.x = enemyPosition.x - m_Position.x;
		direction.y = enemyPosition.y - m_Position.y;
		direction.z = enemyPosition.z - m_Position.z;

		m_length = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);

		if (m_length < 3.0f) // 適切な衝突半径を設定
		{
			// PlayerのTakeDamageを呼び出す
			TakeDamage(10.0f);
		}
	}

	// フレームカウンタを更新
	m_ShootFrameCounter++;

	// 弾の発射
	if (Input::GetKeyPress('J'))
	{
		if (m_ShootFrameCounter >= m_BulletCooldown)
		{
			m_ShootFrameCounter = 0;

			Bullet* bullet = scene->AddGameObject<Bullet>(1);
			if (bullet) { // Nullチェック
				bullet->SetPostion(m_Position);
				bullet->SetSpawnPosition(m_Position);

				// クォータニオンから前方ベクトルを取得して渡す
				XMVECTOR forwardVector = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMLoadFloat4(&m_Quaternion));
				XMFLOAT3 direction;
				XMStoreFloat3(&direction, forwardVector);
				bullet->SetDirection(direction);  // 弾に前方ベクトルを渡す
			}
			m_SE->Play();
		}
	}

	// ShadowVolumの位置も更新
	ShadowVolum* shadowVolum = Manager::GetScene()->GetGameObject<ShadowVolum>();
	if (shadowVolum) {
		shadowVolum->SetPostion(m_Position);
	}

	// HPが0になったらゲームオーバー
	if (m_Hp <= 0.0f) {
		Manager::SetScene<GameOver>();
	}
}

void Player::Draw()
{

	Scene* scene = Manager::GetScene();
	AnimationModel* modelComponent = static_cast<AnimationModel*>(m_Component);

	if (!modelComponent) return; // Nullチェック

	// 描画を読み込む前にアニメーションのアップデートをDrawの中で呼ぶ
	modelComponent->Update(m_AnimationName1.c_str(), m_AnimationFrame,
		m_AnimationName2.c_str(), m_AnimationFrame,
		m_AnimationBlendRatio);
	m_AnimationFrame++;

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationQuaternion(XMLoadFloat4(&m_Quaternion));
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	modelComponent->Draw();

	// Create a window called "My First Tool", with a menu bar.
	ImGui::Begin("Player Debug"); // ウィンドウ名を変更して区別しやすく
	ImGui::InputFloat3("PlayerRotation", (float*)&m_Quaternion);
	ImGui::InputFloat3("PlayerPosition", (float*)&m_Position);
	ImGui::Text("Player HP: %.2f / %.2f", m_Hp, m_MaxHp); // HP表示を追加
	ImGui::End();
}

XMFLOAT3 Player::GetBonePosition(const std::string& boneName)
{
	if (!m_Component) return { 0.0f, 0.0f, 0.0f };

	AnimationModel* model = static_cast<AnimationModel*>(m_Component);
	return { 0.0f, 0.0f, 0.0f };
}

void Player::TakeDamage(float damage) {
	m_Hp -= damage;
	if (m_Hp < 0.0f) {
		m_Hp = 0.0f;
	}

	// HpBarにもダメージを通知
	HpBar* hpbar = Manager::GetScene()->GetGameObject<HpBar>();
	if (hpbar) {
		hpbar->SetCurrentHp(m_Hp); // HpBarに現在のHPをセットする関数を呼び出す
	}
}
