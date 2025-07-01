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

	float m_BulletCooldown;  // クールダウン用の変数を追加
	float m_ShootFrameCounter; // フレームカウンタを初期化
	bool isGrounded = true; // プレイヤーが地面に着地しているかのフラグ

	// HP関連の追加
	float m_Hp;
	float m_MaxHp;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMFLOAT3 GetBonePosition(const std::string& boneName);

	float m_CollisionRadius = 1.0f;

	// ダメージを受ける関数
	void TakeDamage(float damage);
	float GetHp() const { return m_Hp; }
	float GetMaxHp() const { return m_MaxHp; }
};
