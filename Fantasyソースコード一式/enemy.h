#pragma once
#include "gameobject.h"
#include "component.h"
#include <string>

// エネミーの状態を定義
enum class EnemyState
{
    IDLE,          // 徘徊・待機
    CHASE,         // プレイヤー追跡
    ATTACK,        // 近距離攻撃
    JUMP_ATTACK,   // ジャンプ攻撃
    DODGE          // 回避 <= 追加
};

class Enemy : public GameObject
{
private:

    Component* m_Component{};
    Component* m_ChildModel{};
    ID3D11VertexShader* m_VertexShader;
    ID3D11PixelShader* m_PixelShader;
    ID3D11InputLayout* m_VertexLayout;

    std::string m_AnimationName1{};
    std::string m_AnimationName2{};
    float m_AnimationBlendRatio{};
    int m_AnimationFrame{};

    int m_EnemyMobCount = 0;   // スポーンしたEnemyMobの数
    const int MAX_ENEMY_MOB = 5; // スポーンできる最大数

    bool m_IsJumpAttacking;  // 条件を満たしているかどうかのフラグ
    bool m_IsDodging;        // 回避中かどうかのフラグ 

    float m_CollisionRadius = 1.0f; // 当たり判定の半径

    float rangeDegree;	    // 範囲(角度)
    float length;		    // 長さ
    float directionDegree;	// 方向(角度)

    XMFLOAT3 m_Target;   // エネミーが目指す位置
    float m_Speed;       // エネミーの移動速度

    // HP関連
    float m_Hp;
    float m_MaxHp;

    // エネミーの現在の状態
    EnemyState m_CurrentState;
    // 攻撃クールダウンタイマー
    float m_AttackCooldownTimer;
    const float ATTACK_COOLDOWN_MAX = 60.0f; // 1秒クールダウン

    // 回避クールダウンタイマー
    float m_DodgeCooldownTimer;
    const float DODGE_COOLDOWN_MAX = 120.0f; // 2秒クールダウン

    // 回避開始時の目標位置と時間 
    XMFLOAT3 m_DodgeStartPosition;
    XMFLOAT3 m_DodgeTargetPosition;
    int m_DodgeDurationFrames; // 回避にかかるフレーム数
    int m_DodgeCurrentFrame;   // 回避開始からの経過フレーム

    // 視界判定用のメンバ変数 
    bool m_IsPlayerVisible;

public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

    void SummonEnemyMob();
    void PlayerChase();
    void Wander();
    void JumpAttack();
    void Dodge(); 

    // ダメージを受ける関数
    void TakeDamage(float damage);
    // 現在のHPと最大HPを取得する関数
    float GetHp() const { return m_Hp; }
    float GetMaxHp() const { return m_MaxHp; }

    // 視界判定結果を設定する関数 
    void SetPlayerVisible(bool isVisible) { m_IsPlayerVisible = isVisible; }
};
