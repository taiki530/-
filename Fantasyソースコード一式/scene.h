#pragma once

#include <list>
#include <vector>
#include "gameobject.h"
#include "polygon2D.h"
#include "camera.h"
#include "sscamera.h"
#include "enemyCamera.h"
#include "player.h"
#include "enemy.h"
#include "enemyHp.h"
#include "enemyHpwaku.h"
#include "enemymob.h"
#include "bullet.h"
#include "box.h"
#include "sky.h"
#include "shadowvolum.h"
#include "fireeffect.h"
#include "titleName.h"
#include "explosion.h"
#include "prominence.h"

class Scene
{
protected:

	std::list<GameObject*> m_GameObject[3];

public:
	virtual ~Scene() {}

	virtual void Init() 
	{
		
	}

	template <typename T> //テンプレート関数
	T* AddGameObject(int Layer) 
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject[Layer].push_back(gameObject);

		return gameObject;
	}

	template <typename T> //テンプレート関数
	T* GetGameObject()
	{
		for (int i = 0; i < 3 ; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				T* ret = dynamic_cast<T*>(object);

				if (ret != nullptr)
					return ret;
			}
		}

		return nullptr;
	}

	template <typename T> //テンプレート関数
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objectList;

		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				T* ret = dynamic_cast<T*>(object);

				if (ret != nullptr)
					objectList.push_back(ret);
			}
		}

		return objectList;
	}

	virtual void Uninit() 
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Uninit();	// ポリモーフィズム
				delete object;
			}

			m_GameObject[i].clear();
		}
	}
	virtual void Update() 
	{
		for(int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Update();	// ポリモーフィズム
			}

			m_GameObject[i].remove_if([](GameObject* object)
				{return object->Destroy(); });		//ラムダ式
		}
	}
	virtual void Draw() 
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Draw();	// ポリモーフィズム
			}
		}
	}
};
