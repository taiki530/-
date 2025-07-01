#pragma once

class GameObject
{
protected:
	bool		m_Destroy = false;		//物を削除する為のメンバ変数

	XMFLOAT3	m_Position = XMFLOAT3(0, 0, 0);
	XMFLOAT3	m_Rotation = XMFLOAT3(0, 0, 0);
	XMFLOAT3	m_Velocity = XMFLOAT3(0, 0, 0);
	XMFLOAT3	m_Scale = XMFLOAT3(1.0, 1.0, 1.0);

	int m_Hp = 394;           // HP用のメンバ変数
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
			delete this;	//自分自身を消す
			return true;
		}
		else
		{
			return false;
		}
	}

	XMFLOAT3 GetForward()//前方向ベクトル取得
	{
		//オイラーから回転行列を作成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 forward;
		XMStoreFloat3(&forward, rotationMatrix.r[2]);
		return forward;
	}

	XMFLOAT3 GetRight()//右方向ベクトル取得
	{
		//オイラーから回転行列を作成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 right;
		XMStoreFloat3(&right, rotationMatrix.r[0]);
		return right;
	}

	XMFLOAT3 GetLeft()//右方向ベクトル取得
	{
		//オイラーから回転行列を作成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 left;
		XMStoreFloat3(&left, rotationMatrix.r[0]);
		return left;
	}

	// HP操作用のメソッド
	int GetHP() const { return m_Hp; }
	void SetHP(int hp) { m_Hp = hp; }

	void TakeDamage(int damage)
	{
		m_Hp -= damage;
		if (m_Hp <= 0)
		{
			m_Hp = 0;
			SetDestroy(); // HPが0になったらオブジェクトを削除対象にする
		}
	}

	void TakeDamageEnemy(int damage)
	{
		m_enemyHp -= damage;
		if (m_enemyHp <= 0)
		{
			m_enemyHp = 0;
			SetDestroy(); // HPが0になったらオブジェクトを削除対象にする
		}
	}

	void Heal(int amount)
	{
		m_Hp += amount;
		// 必要に応じて上限を設定
		if (m_Hp > 100)
		{
			m_Hp = 100;
		}
	}
};