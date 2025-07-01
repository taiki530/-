#pragma once

class GameObject
{
protected:
	bool		m_Destroy = false;		//�����폜����ׂ̃����o�ϐ�

	XMFLOAT3	m_Position = XMFLOAT3(0, 0, 0);
	XMFLOAT3	m_Rotation = XMFLOAT3(0, 0, 0);
	XMFLOAT3	m_Velocity = XMFLOAT3(0, 0, 0);
	XMFLOAT3	m_Scale = XMFLOAT3(1.0, 1.0, 1.0);

	int m_Hp = 394;           // HP�p�̃����o�ϐ�
	int m_enemyHp = 834.0f;

public:
	virtual ~GameObject() {}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	XMFLOAT3 GetPostion() { return m_Position; }
	XMFLOAT3 GetRotation() { return m_Rotation; }
	XMFLOAT3 GetScale() { return m_Scale; }

	void SetPostion(XMFLOAT3 Postion) { m_Position = Postion; }
	void SetRotation(XMFLOAT3 Rotation) { m_Rotation = Rotation; }
	void SetScale(XMFLOAT3 Scale) { m_Scale = Scale; }

	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;	//�������g������
			return true;
		}
		else
		{
			return false;
		}
	}

	XMFLOAT3 GetForward()//�O�����x�N�g���擾
	{
		//�I�C���[�����]�s����쐬
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 forward;
		XMStoreFloat3(&forward, rotationMatrix.r[2]);
		return forward;
	}

	XMFLOAT3 GetRight()//�E�����x�N�g���擾
	{
		//�I�C���[�����]�s����쐬
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 right;
		XMStoreFloat3(&right, rotationMatrix.r[0]);
		return right;
	}

	XMFLOAT3 GetLeft()//�E�����x�N�g���擾
	{
		//�I�C���[�����]�s����쐬
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 left;
		XMStoreFloat3(&left, rotationMatrix.r[0]);
		return left;
	}

	// HP����p�̃��\�b�h
	int GetHP() const { return m_Hp; }
	void SetHP(int hp) { m_Hp = hp; }

	void TakeDamage(int damage)
	{
		m_Hp -= damage;
		if (m_Hp <= 0)
		{
			m_Hp = 0;
			SetDestroy(); // HP��0�ɂȂ�����I�u�W�F�N�g���폜�Ώۂɂ���
		}
	}

	void TakeDamageEnemy(int damage)
	{
		m_enemyHp -= damage;
		if (m_enemyHp <= 0)
		{
			m_enemyHp = 0;
			SetDestroy(); // HP��0�ɂȂ�����I�u�W�F�N�g���폜�Ώۂɂ���
		}
	}

	void Heal(int amount)
	{
		m_Hp += amount;
		// �K�v�ɉ����ď����ݒ�
		if (m_Hp > 100)
		{
			m_Hp = 100;
		}
	}
};