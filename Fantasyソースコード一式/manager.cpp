#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "input.h"
#include "scene.h"
#include "game.h"
#include "title.h"
#include "fade.h"
#include "fadeout.h"
#include "fadeout2.h"
#include "fadeout3.h"
#include "gameover.h"
#include "result.h"
#include "audio.h"


Scene* Manager::m_Scene{};
Scene* Manager::m_NextScene{};

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();

	m_Scene = new Title();
	//m_Scene = new Game();
	m_Scene->Init();
}


void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	ModelRenderer::UnloadAll();

	Audio::UninitMaster();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	m_Scene->Update();

}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	Renderer::End();


	if (m_NextScene != nullptr)
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}

		ModelRenderer::UnloadAll();

		m_Scene = m_NextScene;
		m_Scene->Init();

		m_NextScene = nullptr;
	}
}



