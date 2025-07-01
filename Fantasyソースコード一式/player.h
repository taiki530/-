#pragma once
#include "gameobject.h"
#include "component.h"
#include <string>

class Player : public GameObject
{
private:

	Component* m_Component{};
	Component* m_ChildModel{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	XMFLOAT3			m_Velocity{};

	XMFLOAT4			m_Quaternion{};

	class Audio* m_SE{};
	class Audio* m_BGM{};

	std::string			m_AnimationName1{};
	std::string			m_AnimationName2{};
	float				m_AnimationBlendRatio{};
	int					m_AnimationFrame{};
	float m_length;

	float m_BulletCooldown;  // �N�[���_�E���p�̕ϐ���ǉ�
	float m_ShootFrameCounter; // �t���[���J�E���^��������
	bool isGrounded = true; // �v���C���[���n�ʂɒ��n���Ă��邩�̃t���O

	// HP�֘A�̒ǉ�
	float m_Hp;
	float m_MaxHp;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMFLOAT3 GetBonePosition(const std::string& boneName);

	float m_CollisionRadius = 1.0f;

	// �_���[�W���󂯂�֐�
	void TakeDamage(float damage);
	float GetHp() const { return m_Hp; }
	float GetMaxHp() const { return m_MaxHp; }
};
