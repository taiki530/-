#pragma once
#include"scene.h"



class Game : public Scene
{
public:
	void Init() override;
	void Update() override;

private:

	class Audio* m_BGM{};
};

