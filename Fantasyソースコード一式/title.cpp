#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "title.h"
#include "game.h"
#include "input.h"
#include "fade.h"
#include "result.h"
#include "audio.h"
#include "meshField.h"
#include "wave.h"
#include "fireeffect.h"
#include "enemy.h"

void Title::Init()
{
	SsCamera* ssCamera = AddGameObject<SsCamera>(0);
	{
		MeshField* meshField = AddGameObject<MeshField>(1);
		meshField->SetPostion(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
	{
		Wave* wave = AddGameObject<Wave>(1);
		wave->SetPostion(XMFLOAT3(0.0f, 2.0f, 0.0f));
	}
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPostion(XMFLOAT3(0.0f, 7.0f, 0.0f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(15.0f, 3.0f, 15.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(-15.0f, 3.0f, 15.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(15.0f, 3.0f, -15.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(-15.0f, 3.0f, -15.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(30.0f, 3.0f, 30.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(-30.0f, 3.0f, 30.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(-30.0f, 3.0f, -30.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(30.0f, 3.0f, -30.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(45.0f, 3.0f, 50.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(-50.0f, 3.0f, 60.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(-45.0f, 3.0f, -45.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(35.0f, 3.0f, -50.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(0.0f, 3.0f, 50.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(0.0f, 3.0f, -50.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(5.0f, 3.0f, 20.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	{
		FireEffect* fireeffect = AddGameObject<FireEffect>(1);
		fireeffect->SetPostion(XMFLOAT3(-5.0f, 3.0f, -20.0f));
		fireeffect->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	
	AddGameObject<TitleName>(2);
	AddGameObject<Sky>(1);
}

void Title::Update()
{
	// 遷移する前に継承元のアップデートを呼ぶ
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Game>();
	}
}
