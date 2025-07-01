#include "main.h"
#include "manager.h"
#include "renderer.h"
#include <cstdlib>
#include <ctime>
#include "enemy.h"
#include "player.h"
#include "hpbar.h"
#include "enemyHp.h"
#include "scene.h"
#include "meshField.h"
#include "modelRenderer.h"
#include "animationModel.h"
#include "camera.h"
#include "enemyMob.h"
#include "enemyCamera.h"
#include "input.h"

void Enemy::Init()
{
    m_Scale = XMFLOAT3(0.03f, 0.03f, 0.03f);

    m_Component = new AnimationModel(this);
    ((AnimationModel*)m_Component)->Load("asse\\model\\Pumpkinhulk L Shaw.fbx");
    ((AnimationModel*)m_Component)->LoadAnimation("asse\\model\\bossWalking.fbx", "Run");
    ((AnimationModel*)m_Component)->LoadAnimation("asse\\model\\bossIdle.fbx", "Idle");
    ((AnimationModel*)m_Component)->LoadAnimation("asse\\model\\bossAttack.fbx", "Attack");
    ((AnimationModel*)m_Component)->LoadAnimation("asse\\model\\Mutant Jumping.fbx", "Jump");

    m_AnimationName1 = "Idle";
    m_AnimationName2 = "Idle";

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\unlitTextureVS.cso");
    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\unlitTexturePS.cso");


    rangeDegree = 90.0f;	    // 範囲(角度)
    length = 5.0f;              // 長さ
    directionDegree = 0.0f;	    // 方向(角度)
    m_Speed = 0.05f;
    m_IsJumpAttacking = false;
    m_IsDodging = false; // 初期化 <= 追加

    m_ChildModel = new ModelRenderer(this);
    ((ModelRenderer*)m_ChildModel)->Load("asse\\model\\box.obj");

    // HPの初期化
    m_MaxHp = 500.0f; // エネミーの最大HPを設定
    m_Hp = m_MaxHp;

    // エネミーの状態を初期化
    m_CurrentState = EnemyState::IDLE; // 初期状態は徘徊
    m_AttackCooldownTimer = 0.0f; // 攻撃クールダウンタイマー初期化
    m_DodgeCooldownTimer = 0.0f; // 回避クールダウンタイマー初期化 <= 追加
    m_IsPlayerVisible = false; // プレイヤーの視界判定初期化
}

void Enemy::Uninit()
{
    if (m_Component) {
        m_Component->Uninit();
        delete m_Component;
        m_Component = nullptr;
    }
    if (m_ChildModel) {
        ((ModelRenderer*)m_ChildModel)->Uninit();
        delete m_ChildModel;
        m_ChildModel = nullptr;
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

void Enemy::Update()
{
    Scene* scene = Manager::GetScene();
    MeshField* meshField = scene->GetGameObject<MeshField>();
    float groundHeight = 0.0f;
    if (meshField) {
        groundHeight = meshField->GetHeight(m_Position);
    }

    m_Component->Update();

    // 地形の高さに合わせてY座標を調整
    if (m_Position.y < groundHeight)
    {
        m_Position.y = groundHeight;
    }
    m_AnimationBlendRatio += 0.1f;
    if (m_AnimationBlendRatio > 1.0f)
        m_AnimationBlendRatio = 1.0f;

    // エネミーのHPが0になったら破壊
    if (m_Hp <= 0.0f) {
        Destroy();
        return; // 破壊されたら以降の処理は不要
    }

    // クールダウンタイマーの更新
    if (m_AttackCooldownTimer > 0.0f) {
        m_AttackCooldownTimer -= 1.0f;
    }
    if (m_DodgeCooldownTimer > 0.0f) { // 回避クールダウンタイマーの更新 <= 追加
        m_DodgeCooldownTimer -= 1.0f;
    }


    // エキスパートシステムによる行動決定
    Player* player = scene->GetGameObject<Player>();
    if (!player) return;

    XMFLOAT3 playerPos = player->GetPostion();
    float distanceToPlayer = XMVectorGetX(XMVector3Length(XMVectorSubtract(XMLoadFloat3(&playerPos), XMLoadFloat3(&m_Position))));

    // 状態遷移のロジック
    // 回避は他の行動より優先度を高くする (緊急回避)
    // プレイヤーが攻撃ボタンを押した時、かつ回避クールダウンが終了している場合
    if (Input::GetKeyTrigger('J') && m_DodgeCooldownTimer <= 0.0f && m_IsPlayerVisible && distanceToPlayer < 15.0f)
    {
        m_CurrentState = EnemyState::DODGE;
        m_DodgeCooldownTimer = DODGE_COOLDOWN_MAX; // 回避クールダウン開始
        m_IsDodging = true; // 回避開始フラグ
        // 回避開始時の情報設定
        m_DodgeStartPosition = m_Position;
        m_DodgeDurationFrames = 30; // 0.5秒で回避完了と仮定
        m_DodgeCurrentFrame = 0;

        // 回避方向の計算: プレイヤーからエネミーへの方向に対して垂直
        XMVECTOR dirFromPlayer = XMVectorSubtract(XMLoadFloat3(&m_Position), XMLoadFloat3(&playerPos));
        dirFromPlayer = XMVectorSetY(dirFromPlayer, 0.0f); // 水平方向のみ
        dirFromPlayer = XMVector3Normalize(dirFromPlayer);

        // 回避方向を決定 (ランダムに左右どちらか)
        XMVECTOR dodgeDirection;
        if (rand() % 2 == 0) { // 50%の確率で右に回避
            dodgeDirection = XMVector3Cross(dirFromPlayer, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)); // 右方向
        }
        else { // 左に回避
            dodgeDirection = XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), dirFromPlayer); // 左方向
        }
        dodgeDirection = XMVector3Normalize(dodgeDirection);

        // 回避目標位置: 現在位置から回避方向へ一定距離移動
        float dodgeDistance = 5.0f; // 回避距離
        XMStoreFloat3(&m_DodgeTargetPosition, XMVectorAdd(XMLoadFloat3(&m_Position), XMVectorScale(dodgeDirection, dodgeDistance)));
    }


    switch (m_CurrentState)
    {
    case EnemyState::IDLE:
        // プレイヤーが視界内に入ったら追跡状態へ
        if (m_IsPlayerVisible) {
            m_CurrentState = EnemyState::CHASE;
        }
        else {
            Wander(); // 徘徊
        }
        break;

    case EnemyState::CHASE:
        // プレイヤーが視界外に出たら徘徊状態へ
        if (!m_IsPlayerVisible) {
            m_CurrentState = EnemyState::IDLE;
        }
        // プレイヤーが攻撃範囲内に入ったら攻撃状態へ
        else if (distanceToPlayer <= 6.0f && m_AttackCooldownTimer <= 0.0f) {
            m_CurrentState = EnemyState::ATTACK;
            m_AttackCooldownTimer = ATTACK_COOLDOWN_MAX; // クールダウン開始
        }
        // プレイヤーがジャンプ攻撃範囲内（例: 10m以内）で、かつジャンプ攻撃クールダウンがなければジャンプ攻撃状態へ
        else if (distanceToPlayer > 6.0f && distanceToPlayer <= 20.0f && m_AttackCooldownTimer <= 0.0f) {
            if ((rand() % 100) < 5) { // 5%の確率でジャンプ攻撃に移行
                m_CurrentState = EnemyState::JUMP_ATTACK;
                m_AttackCooldownTimer = ATTACK_COOLDOWN_MAX * 2; // ジャンプ攻撃はクールダウン長め
            }
        }
        else {
            PlayerChase(); // 追跡
        }
        break;

    case EnemyState::ATTACK:
        // プレイヤーが攻撃範囲外に出たら追跡状態へ
        if (distanceToPlayer > 6.0f) {
            m_CurrentState = EnemyState::CHASE;
        }
        // 攻撃クールダウンが終了したら追跡状態へ
        else if (m_AttackCooldownTimer <= 0.0f) {
            m_CurrentState = EnemyState::CHASE;
        }
        PlayerChase(); // 攻撃アニメーションの再生とダメージ処理 (PlayerChase関数で攻撃も処理)
        break;

    case EnemyState::JUMP_ATTACK:
        JumpAttack(); // ジャンプ攻撃
        // ジャンプ攻撃が完了したらIDLEまたはCHASEに戻る
        if (!m_IsJumpAttacking && m_AttackCooldownTimer <= 0.0f) { // m_IsJumpAttackingがfalseになるまで待つ
            m_CurrentState = EnemyState::CHASE;
        }
        break;

    case EnemyState::DODGE: // 回避状態の処理 <= 追加
        Dodge();
        // 回避が完了したら前の状態に戻るか、追跡状態へ
        if (!m_IsDodging) {
            m_CurrentState = EnemyState::CHASE; // 回避後は追跡に戻る
        }
        break;
    }
}

void Enemy::Draw()
{
    ((AnimationModel*)m_Component)->Update(m_AnimationName1.c_str(), m_AnimationFrame,
        m_AnimationName2.c_str(), m_AnimationFrame,
        m_AnimationBlendRatio);
    m_AnimationFrame++;

    // ワールドマトリクス設定
    XMMATRIX world, scale, rot, trans;
    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
    world = scale * rot * trans;
    Renderer::SetWorldMatrix(world);

    m_Component->Draw();

    // --- ImGui UI ---
    ImGui::Begin("Enemy Behavior");

    if (ImGui::TreeNode("Root"))
    {
        // --- 状態ごとの色表示 ---
        const char* currentStateName;
        ImVec4 stateColor;
        switch (m_CurrentState) { // enumの値で分岐
        case EnemyState::IDLE:        currentStateName = "IDLE";         stateColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); break; // 緑色
        case EnemyState::CHASE:       currentStateName = "CHASE";        stateColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); break; // 黄色
        case EnemyState::ATTACK:      currentStateName = "ATTACK";       stateColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); break; // 赤色
        case EnemyState::JUMP_ATTACK: currentStateName = "JUMP_ATTACK";  stateColor = ImVec4(1.0f, 0.0f, 1.0f, 1.0f); break; // 紫色
        case EnemyState::DODGE:       currentStateName = "DODGE";        stateColor = ImVec4(0.0f, 1.0f, 1.0f, 1.0f); break; // 水色 <= 追加
        default:                      currentStateName = "UNKNOWN";      stateColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); break; // 灰色
        }

        ImGui::SameLine();
        ImGui::TextColored(stateColor, "%s", currentStateName); // enumの文字列を表示

        if (ImGui::TreeNode("Animation"))
        {
            ImGui::InputFloat3("Enemypos", &m_Position.x);
            ImGui::InputFloat3("Enemyrot", &m_Rotation.x);
            ImGui::TreePop();
        }

        // 追加: エネミーHPの表示
        ImGui::Text("Enemy HP: %.2f / %.2f", m_Hp, m_MaxHp);
        ImGui::Text("Attack Cooldown: %.0f", m_AttackCooldownTimer);
        ImGui::Text("Dodge Cooldown: %.0f", m_DodgeCooldownTimer); // 回避クールダウン表示 <= 追加
        ImGui::Text("Player Visible: %s", m_IsPlayerVisible ? "TRUE" : "FALSE");

        ImGui::TreePop();
    }

    ImGui::End();
}

//----------------------------------------------
// プレイヤーを追いかける関数
//----------------------------------------------
void Enemy::PlayerChase()
{
    Scene* scene = Manager::GetScene();
    Player* player = scene->GetGameObject<Player>();
    EnemyCamera* enemycamera = scene->GetGameObject<EnemyCamera>();

    if (!player) return;

    XMFLOAT3 playerPosition = player->GetPostion();

    // プレイヤー方向のベクトルを計算 (水平方向のみ)
    XMVECTOR directionVec = XMVectorSubtract(XMLoadFloat3(&playerPosition), XMLoadFloat3(&m_Position));
    directionVec = XMVectorSetY(directionVec, 0.0f); // Y成分を0にする

    float distanceToPlayer = XMVectorGetX(XMVector3Length(directionVec));

    // 進行方向の角度を計算
    float angleToPlayer = atan2(XMVectorGetX(directionVec), XMVectorGetZ(directionVec));
    m_Rotation.y = angleToPlayer; // エネミーのY軸回転をプレイヤーの方向に向ける

    if (enemycamera) {
        enemycamera->SetRotation(XMFLOAT3(m_Rotation.x, angleToPlayer, m_Rotation.z));
    }

    // ATTACK状態からの呼び出しの場合、アニメーションがAttackに設定されているはず
    if (m_CurrentState == EnemyState::ATTACK) {
        if (m_AnimationName2 != "Attack") {
            m_AnimationName1 = m_AnimationName2;
            m_AnimationName2 = "Attack";
            m_AnimationBlendRatio = 0.0f;
        }
        // 攻撃アニメーション中にダメージを与えるロジック（一度だけダメージを与える）
        // ここではシンプルにクールダウンタイマーが始まった瞬間にダメージを与える
        if (m_AttackCooldownTimer == ATTACK_COOLDOWN_MAX) { // 攻撃クールダウン開始時
            player->TakeDamage(20.0f); // ダメージ量
            SummonEnemyMob(); // 攻撃時に手下を召喚
        }
    }
    else if (m_CurrentState == EnemyState::CHASE) {
        // プレイヤーに向かって移動
        float moveSpeed = m_Speed;
        if (distanceToPlayer > FLT_EPSILON) {
            directionVec = XMVector3Normalize(directionVec);
            XMFLOAT3 normalizedDirection;
            XMStoreFloat3(&normalizedDirection, directionVec);

            m_Position.x += normalizedDirection.x * moveSpeed;
            m_Position.z += normalizedDirection.z * moveSpeed;
        }

        if (m_AnimationName2 != "Run") {
            m_AnimationName1 = m_AnimationName2;
            m_AnimationName2 = "Run";
            m_AnimationBlendRatio = 0.0f;
        }
    }
}

//----------------------------------------------
// プレイヤーが視界にいない際徘徊する関数
//----------------------------------------------
void Enemy::Wander()
{
    MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>();
    EnemyCamera* enemycamera = Manager::GetScene()->GetGameObject<EnemyCamera>();

    // 目標地点に向かう方向を計算
    XMFLOAT3 direction = { m_Target.x - m_Position.x, 0.0f, m_Target.z - m_Position.z };
    float length = sqrtf(direction.x * direction.x + direction.z * direction.z);

    // 進行方向の角度を計算
    float angle2 = atan2(direction.x, direction.z);

    if (length > 0.5f) { // ターゲット未到達 (許容誤差を小さくする)
        if (enemycamera) {
            enemycamera->SetRotation(XMFLOAT3(m_Rotation.x, angle2, m_Rotation.z));
        }

        m_Rotation.y = angle2; // エネミー自身のY軸回転も更新

        // 方向ベクトルを正規化
        if (length > FLT_EPSILON) {
            direction.x /= length;
            direction.z /= length;
        }

        // 移動
        float moveSpeed = m_Speed * 0.5f; // 徘徊は少し遅くする
        m_Position.x += direction.x * moveSpeed;
        m_Position.z += direction.z * moveSpeed;

        if (m_AnimationName2 != "Run") {
            m_AnimationName1 = m_AnimationName2;
            m_AnimationName2 = "Run";
            m_AnimationBlendRatio = 0.0f;
        }
        // メッシュフィールドの高さに調整
        if (meshField) {
            m_Position.y = meshField->GetHeight(m_Position);
        }
        else {
            m_Position.y = 0.0f;
        }
    }
    else {
        // 新しいターゲット地点を設定
        // C++11以降の乱数生成器を使用推奨
        // ここではsrandが一度しか呼ばれないことを前提
        m_Target.x = static_cast<float>((rand() % 41 - 20) * 5); // -100から100の範囲
        m_Target.z = static_cast<float>((rand() % 41 - 20) * 5); // -100から100の範囲
        if (meshField) {
            m_Target.y = meshField->GetHeight(m_Target);
        }
        else {
            m_Target.y = 0.0f;
        }
        // Idleアニメーションを設定
        if (m_AnimationName2 != "Idle") {
            m_AnimationName1 = m_AnimationName2;
            m_AnimationName2 = "Idle";
            m_AnimationBlendRatio = 0.0f;
        }
    }
}

//----------------------------------------------------
// プレイヤーに向かってジャンプして攻撃処理を行う関数
//----------------------------------------------------
void Enemy::JumpAttack()
{
    Scene* scene = Manager::GetScene();
    Player* player = scene->GetGameObject<Player>();
    MeshField* meshField = scene->GetGameObject<MeshField>();

    if (!player || !meshField) return;

    XMFLOAT3 playerPosition = player->GetPostion();

    // Y軸の影響を無視したプレイヤーへの方向ベクトル
    XMVECTOR dirToPlayerXZ = XMVectorSubtract(XMLoadFloat3(&playerPosition), XMLoadFloat3(&m_Position));
    dirToPlayerXZ = XMVectorSetY(dirToPlayerXZ, 0.0f);
    float distanceToPlayerXZ = XMVectorGetX(XMVector3Length(dirToPlayerXZ));

    // エネミーのY軸回転をプレイヤーに向ける
    float angleToPlayer = atan2(XMVectorGetX(dirToPlayerXZ), XMVectorGetZ(dirToPlayerXZ));
    m_Rotation.y = angleToPlayer;

    // ジャンプ攻撃アニメーション開始
    if (m_AnimationName2 != "Jump") {
        m_AnimationName1 = m_AnimationName2;
        m_AnimationName2 = "Jump";
        m_AnimationBlendRatio = 0.0f;
        m_IsJumpAttacking = true; // ジャンプ攻撃開始フラグを立てる
    }

    if (!m_IsJumpAttacking) return; // ジャンプ攻撃中でなければ何もしない

    // ジャンプターゲット地点を設定 (プレイヤーの少し奥)
    XMVECTOR jumpTargetVec = XMVectorSubtract(XMLoadFloat3(&playerPosition), XMVectorScale(XMVector3Normalize(dirToPlayerXZ), 5.0f));
    XMFLOAT3 m_JumpTargetPosition;
    XMStoreFloat3(&m_JumpTargetPosition, jumpTargetVec);
    m_JumpTargetPosition.y = meshField->GetHeight(m_JumpTargetPosition) + 0.1f; // 地面より少し上

    // ジャンプの移動処理
    XMVECTOR currentPos = XMLoadFloat3(&m_Position);
    XMVECTOR targetPos = XMLoadFloat3(&m_JumpTargetPosition);

    // XZ平面での移動
    XMVECTOR moveDirXZ = XMVectorSubtract(targetPos, currentPos);
    moveDirXZ = XMVectorSetY(moveDirXZ, 0.0f);
    float remainingDistanceXZ = XMVectorGetX(XMVector3Length(moveDirXZ));

    float jumpXZSpeed = 0.15f; // ジャンプ中の水平移動速度
    if (remainingDistanceXZ > FLT_EPSILON) {
        moveDirXZ = XMVector3Normalize(moveDirXZ);
        XMFLOAT3 normalizedMoveDirXZ;
        XMStoreFloat3(&normalizedMoveDirXZ, moveDirXZ);
        m_Position.x += normalizedMoveDirXZ.x * jumpXZSpeed;
        m_Position.z += normalizedMoveDirXZ.z * jumpXZSpeed;
    }

    // Y軸の移動 (目標のYに近づける)
    float jumpYSpeed = 0.1f; // ジャンプ中の垂直移動速度
    m_Position.y += (m_JumpTargetPosition.y - m_Position.y) * jumpYSpeed;

    // 着地判定
    if (abs(m_Position.y - m_JumpTargetPosition.y) < 0.2f && remainingDistanceXZ < 2.0f) { // Y座標とXZ距離が目標に近づいたら
        m_Position.y = m_JumpTargetPosition.y; // 目標地点に完全に着地

        // プレイヤーに攻撃判定 (着地時に一度だけ)
        float finalDistance = XMVectorGetX(XMVector3Length(XMVectorSubtract(XMLoadFloat3(&m_Position), XMLoadFloat3(&playerPosition))));
        if (finalDistance < 6.0f) {
            player->TakeDamage(30.0f); // ジャンプ攻撃のダメージ
        }

        m_IsJumpAttacking = false; // ジャンプ攻撃終了
        // ジャンプ攻撃後、クールダウンに入り、次の状態へ遷移
        m_AttackCooldownTimer = ATTACK_COOLDOWN_MAX * 2; // 長めのクールダウン
        m_CurrentState = EnemyState::IDLE; // ジャンプ攻撃後は一旦IDLEに戻る
    }
}

//----------------------------------------------
// 回避行動の関数 <= 追加
//----------------------------------------------
void Enemy::Dodge()
{
    MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>();
    if (!meshField) return;

    if (!m_IsDodging) return; // 回避中でなければ何もしない

    m_DodgeCurrentFrame++;

    // 線形補間を使って現在位置から目標位置へ滑らかに移動
    float t = (float)m_DodgeCurrentFrame / m_DodgeDurationFrames;
    if (t > 1.0f) t = 1.0f; // 補間が1.0fを超えないようにクランプ

    XMVECTOR startPos = XMLoadFloat3(&m_DodgeStartPosition);
    XMVECTOR targetPos = XMLoadFloat3(&m_DodgeTargetPosition);

    XMVECTOR currentDodgePos = XMVectorLerp(startPos, targetPos, t);
    XMStoreFloat3(&m_Position, currentDodgePos);

    // 回避アニメーションがあれば再生
    if (m_AnimationName2 != "Dodge") { // "Dodge"アニメーションを仮定
        m_AnimationName1 = m_AnimationName2;
        m_AnimationName2 = "Dodge";
        m_AnimationBlendRatio = 0.0f;
    }

    // 地面の高さに合わせる
    m_Position.y = meshField->GetHeight(m_Position);

    // 回避が完了したらフラグをリセット
    if (m_DodgeCurrentFrame >= m_DodgeDurationFrames)
    {
        m_IsDodging = false;
        // 回避アニメーションをIdleまたはRunに戻す
        m_AnimationName1 = "Dodge"; // 回避アニメーション終了
        m_AnimationName2 = "Idle"; // 追跡状態に戻ることを想定
        m_AnimationBlendRatio = 0.0f;
    }
}


//----------------------------------------------
// 手下を呼び出す関数
//----------------------------------------------
void Enemy::SummonEnemyMob()
{
    if (m_EnemyMobCount >= MAX_ENEMY_MOB)
    {
        return;
    }

    Scene* scene = Manager::GetScene();
    EnemyMob* enemyMob = scene->AddGameObject<EnemyMob>(1);

    if (enemyMob) {
        // srand((unsigned int)time(NULL)); // main関数の最初に一度だけ呼ぶべき
        float randomX = ((float)rand() / RAND_MAX) * 10.0f - 5.0f;
        float randomZ = ((float)rand() / RAND_MAX) * 10.0f - 5.0f;

        XMFLOAT3 spawnPosition = m_Position;
        spawnPosition.x += randomX;
        spawnPosition.z += randomZ;

        // 生成位置のY座標を地形の高さに合わせる
        MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>();
        if (meshField) {
            spawnPosition.y = meshField->GetHeight(spawnPosition);
        }
        else {
            spawnPosition.y = 0.0f; // デフォルト値
        }

        enemyMob->SetPostion(spawnPosition);
        m_EnemyMobCount++;
    }
}

// ダメージを受ける関数
void Enemy::TakeDamage(float damage)
{
    m_Hp -= damage;
    if (m_Hp < 0.0f) {
        m_Hp = 0.0f;
    }

    // EnemyHpBarにもダメージを通知
    EnemyHpBar* enemyHpBar = Manager::GetScene()->GetGameObject<EnemyHpBar>();
    if (enemyHpBar) {
        enemyHpBar->SetCurrentHp(m_Hp);
    }
}