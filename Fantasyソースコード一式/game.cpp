#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "result.h"
#include "audio.h"
#include "hpbar.h"
#include "hpframe.h"
#include "meshField.h"
#include "wave.h"
#include "enemyCamera.h"

void Game::Init()
{
	AddGameObject<Camera>(0);
	AddGameObject<EnemyCamera>(0);

	AddGameObject<HpFrame>(2);
	AddGameObject<HpBar>(2);

	{
		MeshField* meshField = AddGameObject<MeshField>(1);
		meshField->SetPostion(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
	{
		Wave* wave = AddGameObject<Wave>(1);
		wave->SetPostion(XMFLOAT3(0.0f, 2.0f, 0.0f));
	}
	{
		Player* player = AddGameObject<Player>(1);
		player->SetPostion(XMFLOAT3(0.0f, 0.0f, -10.0f));
		player->SetRotation(XMFLOAT3(0.0f, 180.0f, 0.0f));
	}
	
	{
		Enemy* enemy = AddGameObject<Enemy>(1);
		enemy->SetPostion(XMFLOAT3(0.0f, 0.0f, 10.0f));
		enemy->SetRotation(XMFLOAT3(0.0f, 270.0f, 0.0f));
	}

	{
		ShadowVolum* Shadowvolum = AddGameObject<ShadowVolum>(1);
		Shadowvolum->SetPostion(XMFLOAT3(0.0f, 10.0f, 0.0f));
	}

    AddGameObject<Polygon2D>(2);
	AddGameObject<Sky>(1);
}

void Game::Update()
{
	// 遷移する前に継承元のアップデートを呼ぶ
	Scene::Update();
}


