#pragma once
#include "gameobject.h"
#include "component.h"
#include <string>

// �G�l�~�[�̏�Ԃ��`
enum class EnemyState
{
    IDLE,          // �p�j�E�ҋ@
    CHASE,         // �v���C���[�ǐ�
    ATTACK,        // �ߋ����U��
    JUMP_ATTACK,   // �W�����v�U��
    DODGE          // ��� <= �ǉ�
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

    int m_EnemyMobCount = 0;   // �X�|�[������EnemyMob�̐�
    const int MAX_ENEMY_MOB = 5; // �X�|�[���ł���ő吔

    bool m_IsJumpAttacking;  // �����𖞂����Ă��邩�ǂ����̃t���O
    bool m_IsDodging;        // ��𒆂��ǂ����̃t���O 

    float m_CollisionRadius = 1.0f; // �����蔻��̔��a

    float rangeDegree;	    // �͈�(�p�x)
    float length;		    // ����
    float directionDegree;	// ����(�p�x)

    XMFLOAT3 m_Target;   // �G�l�~�[���ڎw���ʒu
    float m_Speed;       // �G�l�~�[�̈ړ����x

    // HP�֘A
    float m_Hp;
    float m_MaxHp;

    // �G�l�~�[�̌��݂̏��
    EnemyState m_CurrentState;
    // �U���N�[���_�E���^�C�}�[
    float m_AttackCooldownTimer;
    const float ATTACK_COOLDOWN_MAX = 60.0f; // 1�b�N�[���_�E��

    // ����N�[���_�E���^�C�}�[
    float m_DodgeCooldownTimer;
    const float DODGE_COOLDOWN_MAX = 120.0f; // 2�b�N�[���_�E��

    // ����J�n���̖ڕW�ʒu�Ǝ��� 
    XMFLOAT3 m_DodgeStartPosition;
    XMFLOAT3 m_DodgeTargetPosition;
    int m_DodgeDurationFrames; // ����ɂ�����t���[����
    int m_DodgeCurrentFrame;   // ����J�n����̌o�߃t���[��

    // ���E����p�̃����o�ϐ� 
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

    // �_���[�W���󂯂�֐�
    void TakeDamage(float damage);
    // ���݂�HP�ƍő�HP���擾����֐�
    float GetHp() const { return m_Hp; }
    float GetMaxHp() const { return m_MaxHp; }

    // ���E���茋�ʂ�ݒ肷��֐� 
    void SetPlayerVisible(bool isVisible) { m_IsPlayerVisible = isVisible; }
};
